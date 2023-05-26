
#include "MainManager.h"

/** @brief Function loads filters from conrfiguration file into the filters map */

void MainManager::loadFilters(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, int index = 0){ 
    
    filters.clear();
    
    std::string filename = "";
    if(index > 0){
        filename = "./examples/config/config" + std::to_string(index) + ".txt"; 
        std::ifstream config_file1(filename);
        if (!config_file1.is_open()) {
            index = 0;
            logError("Configuration file '" + filename + "' not found");
        }
    }
    
    if(index == 0){
        filename = "./examples/config.txt"; 
    }
    std::ifstream config_file(filename);
    
    if (!config_file.is_open()) {
        std::cerr << "Failed to open configuration file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(config_file, line)) {

        std::istringstream iss(line);
        std::string filter_type;
        bool enabled;
        double parameter = 1.0; // default value
        double default_param = 1.0;
        bool parameter_entered = false;
        iss >> filter_type >> std::boolalpha >> enabled;
        if (iss.fail()) {
            std::cerr << "Failed to parse line: " << line << std::endl;
            continue;
        }
        if (iss >> parameter) {// line contains parameter value
            parameter_entered = true;
        }
        if (filter_type == "WidthMaximiserFilter"){
            if(!parameter_entered) logError("Missing parameter for 'width maximiser' filter");
            if(parameter > 0 && parameter < 4)
                filters.insert({FiltersEnums::WidthMaximiser, std::make_unique<WidthMaximiserFilter>(enabled, parameter)});
            else
                filters.insert({FiltersEnums::WidthMaximiser, std::make_unique<WidthMaximiserFilter>(enabled, default_param)});
        }
        else if (filter_type == "HeightMaximiserFilter") {
            if(!parameter_entered) logError("Missing parameter for 'height maximiser' filter");
            if(parameter > 0 && parameter < 4)
                filters.insert({FiltersEnums::HeightMaximiser, std::make_unique<HeightMaximiserFilter>(enabled, parameter)});
            else
                filters.insert({FiltersEnums::HeightMaximiser, std::make_unique<HeightMaximiserFilter>(enabled, default_param)});
        }
        else if (filter_type == "StepFilter") { 
            if(!parameter_entered) logError("Missing parameter for 'step' filter");
            if(parameter > 0 && parameter < 50)
                filters.insert({FiltersEnums::Step, std::make_unique<StepFilter>(enabled, parameter)}); 
            else
                filters.insert({FiltersEnums::Step, std::make_unique<StepFilter>(enabled, 15)}); 
        } 
        else if (filter_type == "BrightnessFilter") { 
            if(!parameter_entered) logError("Missing parameter for 'brightness' filter");
            if(parameter > 0 && parameter < 11)
                filters.insert({FiltersEnums::Brightness, std::make_unique<BrightnessFilter>(enabled, parameter)}); 
            else
                filters.insert({FiltersEnums::Brightness, std::make_unique<BrightnessFilter>(enabled, 5)}); 
        } 
        else if (filter_type == "NegativeFilter") {
            filters.insert({FiltersEnums::Negative, std::make_unique<NegativeFilter>(enabled)});} 
        else if (filter_type == "SavingFilter") { 
            filters.insert({FiltersEnums::Saving, std::make_unique<SavingFilter>(enabled)});}
        else if (filter_type == "ConvolutionFilter") { 
            filters.insert({FiltersEnums::Convolution, std::make_unique<ConvolutionFilter>(enabled)});} 
        else if (filter_type == "GreyScaleFilter") {
            filters.insert({FiltersEnums::GreyScale, std::make_unique<GreyScaleFilter>(enabled)}); } 
        else if (filter_type == "ActiveIndexFilter") { 
            if(!parameter_entered) logError("Missing parameter for 'active index' filter");
            if(parameter > 0 && parameter < 100)
                filters.insert({FiltersEnums::ActiveIndex, std::make_unique<ActiveIndexFilter>(enabled, parameter)}); 
            else
                filters.insert({FiltersEnums::ActiveIndex, std::make_unique<ActiveIndexFilter>(enabled, default_param)}); 
        }
        else if (filter_type == "AnimationDelayFilter") { 
            if(!parameter_entered) logError("Missing parameter for 'animation delay' filter");
            if(parameter > 0 && parameter < 100)
                filters.insert({FiltersEnums::AnimationDelay, std::make_unique<AnimationDelayFilter>(enabled, parameter)}); 
            else
                filters.insert({FiltersEnums::AnimationDelay, std::make_unique<AnimationDelayFilter>(enabled, 10)}); 
        } 
        else if (filter_type == "ColorFilter") { 
            if(!parameter_entered) logError("Missing parameter for 'color' filter");
            ColorsEnums parameter1 = ColorsEnums::Standard;
            if(parameter == 2.0) parameter1 = ColorsEnums::Red;
            else if(parameter == 3.0) parameter1 = ColorsEnums::Green;
            if(parameter == 4.0) parameter1 = ColorsEnums::Blue;
            filters.insert({FiltersEnums::Color, std::make_unique<ColorFilter>(enabled, parameter1)}); } 
        else { 
            std::cerr << "Unknown filter type: " << filter_type << std::endl; }
    }

    // Fill up filters missing in the config file
    if(filters.find(FiltersEnums::ActiveIndex) == filters.end()){
        filters.insert({FiltersEnums::ActiveIndex, std::make_unique<ActiveIndexFilter>(true, 1)}); 
    }if(filters.find(FiltersEnums::WidthMaximiser) == filters.end()){
        filters.insert({FiltersEnums::WidthMaximiser, std::make_unique<WidthMaximiserFilter>(true, 2)}); 
    }if(filters.find(FiltersEnums::HeightMaximiser) == filters.end()){
        filters.insert({FiltersEnums::HeightMaximiser, std::make_unique<HeightMaximiserFilter>(true, 1)}); 
    }if(filters.find(FiltersEnums::Step) == filters.end()){
        filters.insert({FiltersEnums::Step, std::make_unique<StepFilter>(true, 15)}); 
    }if(filters.find(FiltersEnums::Brightness) == filters.end()){
        filters.insert({FiltersEnums::Brightness, std::make_unique<BrightnessFilter>(true, 5)}); 
    }if(filters.find(FiltersEnums::Negative) == filters.end()){
        filters.insert({FiltersEnums::Negative, std::make_unique<NegativeFilter>(false)}); 
    }if(filters.find(FiltersEnums::Saving) == filters.end()){
        filters.insert({FiltersEnums::Saving, std::make_unique<SavingFilter>(false)}); 
    }if(filters.find(FiltersEnums::Convolution) == filters.end()){
        filters.insert({FiltersEnums::Convolution, std::make_unique<ConvolutionFilter>(false)}); 
    }if(filters.find(FiltersEnums::GreyScale) == filters.end()){
        filters.insert({FiltersEnums::GreyScale, std::make_unique<GreyScaleFilter>(false)}); 
    }if(filters.find(FiltersEnums::AnimationDelay) == filters.end()){
        filters.insert({FiltersEnums::AnimationDelay, std::make_unique<AnimationDelayFilter>(true, 15)}); 
    }if(filters.find(FiltersEnums::Color) == filters.end()){
        filters.insert({FiltersEnums::Color, std::make_unique<ColorFilter>(true, ColorsEnums::Standard)});
    }
}

/** @brief Load characters used for ascii generation from a file */
std::string MainManager::loadCharMap() {
    std::string filename =  "./examples/input_characters.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    file.close();
    if(content == "" || content.empty()){
        content = ".@";
        logError("Input character map empty, using characters '@.' by default");
    }
    return content;
}


void MainManager::saveFilters(std::map<FiltersEnums, std::unique_ptr<Filter>> & filters, int index = 0){
    
    std::string filename = "";
    if(index > 0){
        filename = "./examples/config/config" + std::to_string(index) + ".txt"; 
        std::ifstream config_file1(filename);
        if (!config_file1.is_open()) {
            index = 0;
            logError("Configuration file '" + filename + "' not found");
        }
    }
    
    if(index == 0){
        filename = "./examples/config.txt"; 
    }
    std::ofstream config_file(filename);
    
    if (!config_file.is_open()) {
        std::cerr << "Failed to open configuration file" << std::endl;
        return;
    }

    
     for (const auto& pair : filters) {
        (*(pair.second)).save(config_file);
    }
}

/** @brief Checks whether number is a number or not */
bool MainManager::isnumber(std::string in ){
    for(char const & ch : in){
        if(!std::isdigit(ch)) return false;
    }
    return true;
}

/** @brief logs error string in red color */
void MainManager::logError(const std::string & error) {
    std::cout << "\033[31m[Error] " << error << "\t\033[0m\n";
}

/** @brief loads image filename for specific index */
std::string MainManager::getImageNameByIndex(std::string & format, int activeIndex){
    SDL_Surface* tmpSurface;
    format = "png";
    std::string inputFilename = "./examples/images/image_0" + std::to_string(activeIndex) + "." + format;
    tmpSurface = IMG_Load(inputFilename.c_str());
    if (tmpSurface == nullptr) {
        std::cout << inputFilename << " not found :(\n";
        format = "jpeg";
        inputFilename = "./examples/images/image_0" + std::to_string(activeIndex) + "." + format;
        tmpSurface = IMG_Load(inputFilename.c_str());
        if (tmpSurface == nullptr) { 
            std::cout << inputFilename << " not found :(\n";    
            return "";
        }
    }
    return inputFilename;
} 

/** @brief loads image input name based on the user input */
void MainManager::getImageNameFromUser(std::string & inputFilename, std::string & format){

    SDL_Surface* tmpSurface;
    inputFilename.erase(0, inputFilename.find_first_not_of(" "));
    inputFilename.erase(inputFilename.find_last_not_of(" ") + 1);

    tmpSurface = IMG_Load(inputFilename.c_str());

    if (tmpSurface == nullptr){ 
        logError("Image '" + inputFilename + "' not found");
        inputFilename = "";
        return;
        //continue;
    }

    size_t dot_pos = inputFilename.find_last_of('.');
    if (dot_pos == std::string::npos) {
       //  std::cout << "Error: No file extension found!" << std::endl;

        inputFilename = "";
        logError("No file extension found!");
        return;

    }
    format = inputFilename.substr(dot_pos + 1);
    // std::cout << "File extension: " << format << std::endl;
}

/** @brief checks whether file with filename passed in is a png or jpg image
 * @return bool value, true for a correct image file
*/
bool MainManager::isImageFile(const std::string& filename) {
    std::string extension = fs::path(filename).extension().string();
    return (extension == ".jpeg" || extension == ".jpg" || extension == ".png");
}

/** @brief removes starting whitespaces from the string */
std::string MainManager::removeLeadingWhitespaces(const std::string& input) {
    size_t pos = input.find_first_not_of(" \t\n\r"); // Find the first non-whitespace character
    
    if (pos == std::string::npos) {
        // No non-whitespace character found
        return "";
    }
    
    return input.substr(pos); // Extract the substring starting from the non-whitespace character
}

/** @brief removes ending whitespaces from the string */
std::string MainManager::removeTrailingSpaces(const std::string& str) {
    size_t endPos = str.find_last_not_of(" \t\n\r");
    if (endPos == std::string::npos) {
        // String contains only spaces or is empty
        return "";
    }
    return str.substr(0, endPos + 1);
}