#include "ConversionManager.h"

/** @brief returns intensity of the ConversionManager*/
int ConversionManager::getIntensity(){
    return intensityStep;
} 


/** @brief main function of the ConversionManager
 converts input image to ascii art using several input filters such as
     width_maximiser
     height_maximiser
     negative
     convolution
     grey_scale
      brightness
*/
void ConversionManager::convertToAscii(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, std::string outfile){
    
    // get the pixel array from PictureManager
    unsigned char* pixels = pictureManager->getPixels();

    // calculate the width and height of the image
    int width = pictureManager->getWidth();
    int height = pictureManager->getHeight();


    // apply active filters on the pixels
     for (const auto& pair : filters) {
        if ( (pair.second)->isActive) {
             (pair.second)->apply(pixels, width, height);
        }
    }

    int widthMax = 2;
    int heightMax = 1;
    auto it = filters.find(FiltersEnums::WidthMaximiser);
    if (it != filters.end()) { widthMax = it->second->get_value(); }
    auto it2 = filters.find(FiltersEnums::HeightMaximiser);
    if (it2 != filters.end()) { heightMax = it2->second->get_value(); }

    // calculate the number of dots
    int dotsInWidth = width / intensityStep * widthMax;
    int dotsInHeight = height / intensityStep * heightMax;
    
    // initialize the output string
    std::string output;

    // loop through all dots
    for (int dotY = 0; dotY < dotsInHeight; dotY++) {
        for (int dotX = 0; dotX < dotsInWidth; dotX++) {

            // calculate the average intensity of the dot
            int sumIntensity = 0;
            for (int y = 0; y < intensityStep; y++) {
                for (int x = 0; x < intensityStep; x++) {
                    int pixelX = dotX * intensityStep/widthMax + x;
                    int pixelY = dotY * intensityStep/heightMax + y;
                    int pixelIndex = (pixelY * width + pixelX) * 3; // 3 channels (RGB)
                    unsigned char r = pixels[pixelIndex];
                    unsigned char g = pixels[pixelIndex + 1];
                    unsigned char b = pixels[pixelIndex + 2];
                    sumIntensity += (r + g + b) / 3;
                }
            }
            int avgIntensity = sumIntensity / (intensityStep * intensityStep);

            // find the closest character in the character map
            int closestCharIndex = 0;
            int closestIntensityDiff = 256; // initialize to maximum possible value
            for (int i = 0; i < charMapLength; i++) {
                int charIntensity = (255 - i * 255 / (charMapLength - 1));
                int intensityDiff = std::abs(avgIntensity - charIntensity);
                if (intensityDiff < closestIntensityDiff) {
                    closestCharIndex = i;
                    closestIntensityDiff = intensityDiff;
                }
            }

            // append the closest character to the output
            output += charMap[closestCharIndex];
        }
        // add newline after each row of dots
        output += '\n';
    }

    auto it3 = filters.find(FiltersEnums::Color);
    if (it3 != filters.end()) {  it3->second->printStartCode(std::cout); }

    // print the output into console
    std::cout << output << std::endl;

    if (it3 != filters.end()) {  it3->second->printEndCode(std::cout); }

    // if the output file path is provided, the output gets printed into the output file as well
    if(outfile != ""){
        std::cout << "Saving output in: " << outfile << std::endl;
        std::ofstream outfile_out(outfile.c_str());
        if (!outfile_out.is_open()) {
            std::cerr << "Error opening file: " << outfile << std::endl;
            return;
        }
        outfile_out << output << std::endl;
    }
}