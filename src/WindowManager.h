#pragma once


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
#include "PictureManager.h"

/** 
 * @brief Class used to display images as GUI windows. it's used to show the image from the pixels, so that the input filters also affect the image as well as the output ASCII
 */
class WindowManager {
private:
    PictureManager * pictureManager;

public:
    /** @param pm is a pointer to  PictureManager of the image we work with */
    WindowManager(PictureManager * pm) :  pictureManager(pm) { }

    ~WindowManager() { }

    /** @brief display an image in a window */
    void displayImage(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, std::string outfile);
        
};

