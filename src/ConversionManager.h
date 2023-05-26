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
#include "PictureManager.h"

/** @brief Manager for conversion of image to ascii art*/
class ConversionManager {

public:
    PictureManager* pictureManager;
    std::string charMap;
    int charMapLength;
    int intensityStep;

    /**
     * @brief Default constructor
     * @param pm is a pointer to PictureManager of the image we work with
     * @param map is a string with characters the output ascii is generated with
     * @param step affects the output ascii size 
    */
    ConversionManager(PictureManager* pm, std::string map, int step)
        : pictureManager(pm), charMap(map), charMapLength(map.length()), intensityStep(step) {}

    /** @brief simple destructor */
    ~ConversionManager() = default;
    
    void convertToAscii(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, std::string outfile);    // virtual

    int getIntensity();

};
