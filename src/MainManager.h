#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <map>
#include <memory>
#include <string>
#include <cstring>

#include <iterator>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <algorithm>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <png.h>
#include <jpeglib.h>

#include <filesystem>

#include "Filters.h"
#include "WindowManager.h"
#include "PictureManager.h"

namespace fs = std::filesystem;


/** @brief Manager containing common functions necessary for main.cpp */
class MainManager {

public:
    MainManager() = default;

    ~MainManager() = default;

    void loadFilters(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, int index);

    void saveFilters(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, int index);        

    bool isnumber(std::string in  );
    
    void logError(const std::string & error);

    std::string getImageNameByIndex(std::string & format, int activeIndex);
    
    void getImageNameFromUser(std::string & inputFilename, std::string & format);

    std::string loadCharMap();

    bool isImageFile(const std::string& filename);

    bool isDirectory(const std::string& path);

    std::string removeLeadingWhitespaces(const std::string& input);

    std::string removeTrailingSpaces(const std::string& input);

};

