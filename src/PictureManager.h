#pragma once


#include <algorithm>
#include <cmath>
#include <vector>

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <png.h>
#include <jpeglib.h>

#include <memory>

#include "Filters.h"


/** @brief Manager of pictures for loading, modifying and saving them
*/
class PictureManager {
private:
    // SDL_Surface* surface;
    unsigned char* pixels;
    int width;
    int height;

public:
    
    PictureManager(std::string filename);

    ~PictureManager() = default;

    /** @return pixels array ( unsigned char*) */
    unsigned char* getPixels(){ return pixels;}

    /** @return  width ( int ) */
    int getWidth() { return width;}

    /** @return  height ( int ) */
    int getHeight() { return height;}

    void saveImage(unsigned char* pixels, std::string outfile = "output.jpg");
    
    void saveWithBlueScale(const std::string& filename, int blueAmount);
};
