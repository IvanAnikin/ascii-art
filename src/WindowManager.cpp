#include "WindowManager.h"


void WindowManager::displayImage(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, std::string outfile) {
    // get the pixel array from PictureManager
    unsigned char* pixels = pictureManager->getPixels();

    // calculate the width and height of the image
    int width = pictureManager->getWidth();
    int height = pictureManager->getHeight();
    int depth = 24;  // bits per pixel (e.g. 24 for RGB)
    
    // apply active filters on the pixels
     for (const auto& pair : filters) {
        if ( (pair.second)->isActive) {
             (pair.second)->apply(pixels, width, height);
        }
    }

    // swap red and blue color channels
    for (int i = 0; i < width * height; i++) {
        unsigned char temp = pixels[i * 3];
        pixels[i * 3] = pixels[i * 3 + 2];
        pixels[i * 3 + 2] = temp;
    }

    // create an SDL surface from the pixel data
    SDL_Surface* image = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, width * depth / 8, 0, 0, 0, 0);

    // check if the surface was created successfully
    if (image == nullptr) {
        std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;
        exit(1);
    }
    
    // Create a window to display the image
    SDL_Window* window = SDL_CreateWindow("ASCII art rocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, image->w, image->h, SDL_WINDOW_SHOWN);

    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Load the image into a texture for rendering
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Draw the texture on the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Render the changes to the window
    SDL_RenderPresent(renderer);

    bool m_quit = false;
    SDL_Event e;
    while (!m_quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                m_quit = true;
            }
            else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
                m_quit = true;
            }
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Render the changes to the window
        SDL_RenderPresent(renderer);

        SDL_Delay(500);
    }
    
    // Clean up resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(image);
    IMG_Quit();
    SDL_Quit();

    // swap red and blue color channels
    for (int i = 0; i < width * height; i++) {
        unsigned char temp = pixels[i * 3];
        pixels[i * 3] = pixels[i * 3 + 2];
        pixels[i * 3 + 2] = temp;
    }

    // if the output file path is provided, the output gets printed into the output file as well
    if(outfile != "" && !outfile.empty()){
        pictureManager->saveImage(pixels, outfile);
    }
    
}
