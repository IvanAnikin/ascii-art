#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <unistd.h>

#include <map>
#include <memory>
#include <string>
#include <set>

#include <atomic>
#include <thread>

#include <iterator>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// include my manager and filters classes 
#include "ConversionManager.h"
#include "PictureManager.h"
#include "WindowManager.h"
#include "MainManager.h"
#include "Filters.h"
#include "Tester.h"

std::atomic<bool> continueLoop(true);
std::atomic<bool> for_running(false);

// Function to handle user input in a separate thread
void inputThread()
{   
    while(for_running){
        std::string input;
        std::getline(std::cin, input);
        continueLoop = !continueLoop;  // Set the flag to false to stop the loop
        usleep(1000);
    }
}


    // Otestovat
        // !!!!  Ctrl + D (otestovat leaky ) !!!!!
        // neplatni configurak 

    // build slozka ? 

int main() {
    MainManager mm;

    std::map<FiltersEnums, std::unique_ptr<Filter>> filters;
    mm.loadFilters(filters, 0);

    // main loop
    bool quit = false;
    while (!quit) { 
        
        // Display filters
        for (const auto& pair : filters) {
            std::cout << *(pair.second) << std::endl;
        }

        // wait for command
        std::string command;

        if( ! (std::cin >> command) ){
            if(std::cin.eof()){
                quit = true;
            }else{
                mm.logError("Incorrect command");
                continue;
            }
        }else if(command == "quit"){
            quit = true;

        }else if(mm.isnumber(command)){
            try {
                auto it = filters.find(FiltersEnums::ActiveIndex);
                if (it != filters.end()) {
                    it->second->update(command);
                }
            }
            catch(const std::invalid_argument& e){
                mm.logError("Incorrect command");
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }else if(command == "q" || command == "w"){
            auto it = filters.find(FiltersEnums::WidthMaximiser);
            if (it != filters.end()) { it->second->update(command); }
        }else if(command == "e" || command == "r"){
            auto it = filters.find(FiltersEnums::HeightMaximiser);
            if (it != filters.end()) { it->second->update(command); }
        }else if(command == "t" || command == "y"){
            auto it = filters.find(FiltersEnums::Step);
            if (it != filters.end()) { it->second->update(command); }
        }else if(command == "u" || command == "i"){
            auto it = filters.find(FiltersEnums::Brightness);
            if (it != filters.end()) { it->second->update(command); }
        }else if(command == "o" || command == "p"){
            auto it = filters.find(FiltersEnums::AnimationDelay);
            if (it != filters.end()) { it->second->update(command); }
        }else if(command == "negative"){
            auto it = filters.find(FiltersEnums::Negative);
            if (it != filters.end()) { it->second->switchActive(); }
        }else if(command == "save"){
            auto it = filters.find(FiltersEnums::Saving);
            if (it != filters.end()) { it->second->switchActive(); }
        }else if(command == "convolution"){
            auto it = filters.find(FiltersEnums::Convolution);
            if (it != filters.end()) { it->second->switchActive(); }
        }else if(command == "grey"){
            auto it = filters.find(FiltersEnums::GreyScale);
            if (it != filters.end()) { it->second->switchActive(); }
        }else if (command == "blue" || command == "red" || command == "green" || command == "default") {
            auto it = filters.find(FiltersEnums::Color);
            if (it != filters.end()) { it->second->update(command); }            
        }else if(command == "show"){ // Shows image using displayImage function

            std::string inputFilename;
            std::string format;
            
            std::getline(std::cin, inputFilename);

            int  activeIndex = 1; 
            auto it = filters.find(FiltersEnums::ActiveIndex);
            if (it != filters.end()) { activeIndex = it->second->get_value(); }

            mm.loadFilters(filters, activeIndex);

            if (!inputFilename.empty()) mm.getImageNameFromUser(inputFilename, format);
            else inputFilename = mm.getImageNameByIndex(format, activeIndex);
            if(inputFilename == ""){
                mm.logError("Image with index '" + std::to_string(activeIndex) + "' not found");
                break;
            }

            PictureManager pm(inputFilename);
            WindowManager wm(&pm);

            std::string outputFilename = "";
            auto it2 = filters.find(FiltersEnums::Saving);
            if (it2 != filters.end() && it2->second->isActive ) { 
                outputFilename = "./examples/images/output/output_image_0" + std::to_string(activeIndex) + ".jpeg";
            }
            
            wm.displayImage(filters, outputFilename);
            
            std::cout << "Image showed";
            if (it2 != filters.end() && it2->second->isActive ) {  std::cout << " and saved to " << outputFilename; }
            std::cout << std::endl;

            mm.loadFilters(filters, 0);
            std::system("clear");
        }
        else if(command.find("ascii") == 0){ // Converts image to ascii art and prints it into console (additionally it can be also saved to txt file)
            
            std::string inputFilename;
            std::string format;
            
            std::getline(std::cin, inputFilename);
            inputFilename = mm.removeLeadingWhitespaces(inputFilename);
            inputFilename = mm.removeTrailingSpaces(inputFilename);

            if(!inputFilename.empty() &&  std::filesystem::is_directory(std::filesystem::absolute(inputFilename)) ){
                std::filesystem::path absolutePath = std::filesystem::absolute(inputFilename);

                //if( std::filesystem::is_directory(absolutePath) ){
                    
                    mm.loadFilters(filters, 0);  

                    auto comparePaths = [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                        return a.path().filename() < b.path().filename();
                    };
                    std::set<std::filesystem::directory_entry, decltype(comparePaths)> imagePaths(comparePaths);

                    for (const auto& entry : std::filesystem::directory_iterator(inputFilename)) {
                        if (entry.is_regular_file()) {
                            std::string filePath = entry.path().string();
                            imagePaths.insert(entry);
                        }
                    }
                    
                    std::thread inputHandler(inputThread);
        
                    for (const auto& imagePath : imagePaths) {
                        for_running = true;
                        std::system("clear");
                        PictureManager pm(imagePath.path());
                        int  step = 10; 
                        auto it2 = filters.find(FiltersEnums::Step);
                        if (it2 != filters.end()) { step = it2->second->get_value();}
                        ConversionManager cm(&pm, mm.loadCharMap(), step);

                        cm.convertToAscii(filters, "");

                        int  delay = 10; 
                        auto it3 = filters.find(FiltersEnums::AnimationDelay);
                        if (it3 != filters.end()) { delay = it3->second->get_value();}
                    
                        while(!continueLoop)
                            usleep(10000);
                        
                        usleep(delay * 10000);
                    }  
                    for_running = false;
                    inputHandler.join();

                //}
            }else{
                int  activeIndex = 1; 
                auto it = filters.find(FiltersEnums::ActiveIndex);
                if (it != filters.end()) { activeIndex = it->second->get_value(); }

                mm.saveFilters(filters, 0);
                mm.loadFilters(filters, activeIndex);       

                if (!inputFilename.empty()) mm.getImageNameFromUser(inputFilename, format);
                else inputFilename = mm.getImageNameByIndex(format, activeIndex);
                if(inputFilename == ""){
                    mm.logError("Image not found");
                    continue;
                }
                
                PictureManager pm(inputFilename);
                int  step = 10; 
                auto it2 = filters.find(FiltersEnums::Step);
                if (it2 != filters.end()) { step = it2->second->get_value();}
                ConversionManager cm(&pm, mm.loadCharMap(), step);
                
                std::string outputFilename = "";
                
                auto it3 = filters.find(FiltersEnums::Saving);
                if (it3 != filters.end() && it3->second->isActive ) {  outputFilename = "./examples/images/output/output_image_0" + std::to_string(activeIndex) + ".txt"; }
                
                std::system("clear");
                cm.convertToAscii(filters, outputFilename);
                
                std::cout << "Image converted to ascii";
                if (it3 != filters.end() && it3->second->isActive ) { std::cout << " and saved to " << outputFilename; }
                std::cout << std::endl;

                 mm.loadFilters(filters, 0);
            }
        }else if (command == "save_blue") {

            int  activeIndex = 1; 
            auto it = filters.find(FiltersEnums::ActiveIndex);
            if (it != filters.end()) { activeIndex = it->second->get_value(); }
            
            mm.loadFilters(filters, activeIndex);

            if (activeIndex == 0) {
                std::cout << "No active image selected." << std::endl;
            } else {
                std::string outputFilename = "blue_" + std::to_string(activeIndex) + ".jpeg" ;
                PictureManager pm("./examples/images/image_0" + std::to_string(activeIndex) + ".jpeg" );
                pm.saveWithBlueScale("./examples/images/output/" + outputFilename, 50);
                std::cout << "Image converted to blue and saved to " << outputFilename << std::endl;
            }


            mm.loadFilters(filters, 0);
        }else if(command == "test"){
            std::cout << "Testing started\n";
            bool all_good = true;
            Tester my_tester;
            bool success = my_tester.testFilters();
            if(!success) all_good = false;
            success = my_tester.testSpaces();
            if(!success) all_good = false;
            if(all_good) std::cout << "All tests finished successfully\n";
        }
        else{
            mm.logError( "Unknown command: " + command);
        }
    }
}
