#include "PictureManager.h"

/** @brief simple constructor 
     * @param filename is a name of the input image
    */
PictureManager::PictureManager(std::string filename){
        
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // load image using SDL_Image
    SDL_Surface* tmpSurface = IMG_Load(filename.c_str());
    if (tmpSurface == nullptr) {
        std::cerr << "Error loading image: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // convert surface to 24-bit RGB format
    SDL_Surface * surface = SDL_ConvertSurfaceFormat(tmpSurface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(tmpSurface);
    if (surface == nullptr) {
        std::cerr << "Error converting surface: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // get the pixel array and image dimensions
    pixels = static_cast<unsigned char*>(surface->pixels);
    width = surface->w;
    height = surface->h;

    SDL_Quit();

}

/** @brief  save image*/
void PictureManager::saveImage(unsigned char* pixels, std::string outfile){
    // get file extension
        std::string ext = outfile.substr(outfile.find_last_of(".") + 1);

        // set default quality for JPEG output
        int quality = 90;

        // create file pointer for output file
        FILE* fp = fopen(outfile.c_str(), "wb");
        if (fp == nullptr) {
            std::cerr << "Error: cannot open file " << outfile << " for writing." << std::endl;
            return;
        }

        // save image based on file extension
        if (ext == "jpg" || ext == "jpeg") {
            // initialize JPEG compression structure
            struct jpeg_compress_struct cinfo;
            struct jpeg_error_mgr jerr;
            cinfo.err = jpeg_std_error(&jerr);
            jpeg_create_compress(&cinfo);
            jpeg_stdio_dest(&cinfo, fp);

            // set image dimensions and format
            cinfo.image_width = getWidth();
            cinfo.image_height = getHeight();
            cinfo.input_components = 3;
            cinfo.in_color_space = JCS_RGB;

            // set compression parameters
            jpeg_set_defaults(&cinfo);
            jpeg_set_quality(&cinfo, quality, TRUE);

            // start compression
            jpeg_start_compress(&cinfo, TRUE);

            // write pixel data to JPEG file
            JSAMPROW row_pointer[1];
            int row_stride = getWidth() * 3;
            while (cinfo.next_scanline < cinfo.image_height) {
                row_pointer[0] = &pixels[cinfo.next_scanline * row_stride];
                jpeg_write_scanlines(&cinfo, row_pointer, 1);
            }

            // finish compression
            jpeg_finish_compress(&cinfo);
            jpeg_destroy_compress(&cinfo);
        }
        else if (ext == "png") {
            // initialize PNG write structure
            png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            if (png_ptr == nullptr) {
                std::cerr << "Error: cannot create PNG write structure." << std::endl;
                fclose(fp);
                return;
            }

            // initialize PNG info structure
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr == nullptr) {
                std::cerr << "Error: cannot create PNG info structure." << std::endl;
                fclose(fp);
                png_destroy_write_struct(&png_ptr, nullptr);
                return;
            }

            // set up error handling
            if (setjmp(png_jmpbuf(png_ptr))) {
                std::cerr << "Error: error occurred while writing PNG file." << std::endl;
                fclose(fp);
                png_destroy_write_struct(&png_ptr, &info_ptr);
                return;
            }

            // set output file
            png_init_io(png_ptr, fp);

            // set image dimensions and format
            png_set_IHDR(png_ptr, info_ptr, getWidth(), getHeight(), 8, PNG_COLOR_TYPE_RGB,
                        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

            // write header info
            png_write_info(png_ptr, info_ptr);

            // write pixel data
            int row_stride = getWidth() * 3;
            for (int y = 0; y < getHeight(); ++y){
                png_bytep row_pointer = &pixels[y * row_stride];
                png_write_row(png_ptr, row_pointer);

                // end write
                png_write_end(png_ptr, nullptr);

                // clean up
                png_destroy_write_struct(&png_ptr, &info_ptr);
            }
        }
        else {
            std::cerr << "Error: unsupported file format " << ext << std::endl;
            fclose(fp);
            return;
        }

        // close output file
        fclose(fp);
        // std::cout << "Image saved as " << outfile << std::endl;
}

/** @brief Save image converted to blue scale into an output PNG file */
void PictureManager::saveWithBlueScale(const std::string& filename, int blueAmount){
    // create an SDL_Surface from the pixel array
    SDL_Surface* tmpSurface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 24, width * 3, 0, 0, 0, 0);
    if (tmpSurface == nullptr) {
        std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;
        return;
    }

    // loop through the pixels and modify the blue channel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char* pixel = static_cast<unsigned char*>(tmpSurface->pixels) + (y * tmpSurface->pitch + x * 3);
            pixel[2] += blueAmount; // modify the blue channel
            if (pixel[2] > 255) {
                pixel[2] = 255; // clamp the value to 255
            }
        }
    }

    // save the modified surface as PNG
    if (IMG_SavePNG(tmpSurface, filename.c_str()) != 0) {
        std::cerr << "Error saving PNG: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(tmpSurface);
}