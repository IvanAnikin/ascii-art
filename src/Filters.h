

#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include <map>

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <png.h>
#include <jpeglib.h>

#include <png.h>
#include <jpeglib.h>


/** @brief Enums for organisation of filters map*/
enum class FiltersEnums {
    WidthMaximiser,
    HeightMaximiser,
    Step,
    Brightness,
    Negative,
    Saving,
    Convolution,
    GreyScale,
    ActiveIndex,
    AnimationDelay,
    Color
};

/** @brief Enums for colors*/
enum class ColorsEnums {
    Red,
    Green,
    Blue,
    Standard
};


/** @brief  A class for filters modifying the image before its shown or conerted to ascii */
class Filter {
public:
    Filter(bool active) : isActive(active) {}
    virtual ~Filter() {}

    /** @brief applies the current filter (function is virtual and each filter has it's own way of applying itself) */
    virtual void apply(unsigned char* pixels, int width, int height) = 0;

    /** @brief Filter is used only if the isActive is true  */
    bool isActive;

    /** @brief Printout of the filter details */
    virtual std::ostream& print(std::ostream& os) const = 0;

    /** @brief Changes the isActive state to its negative */
    void switchActive();

    /** @brief Updates the configuration value */
    virtual void update(const std::string& command) = 0;

    /** @return the filter value (double) */
    virtual double get_value() = 0;

    /** @brief Prints the starting sequence for changing color */
    virtual void printStartCode(std::ostream& os) const = 0;

    /** @brief Prints the endting sequence for changing color */
    virtual void printEndCode(std::ostream& os) const = 0;

    /** @brief function to save filter values into configuration file provided as a stream*/
    virtual void save(std::ofstream& stream) const = 0;
private:
    friend std::ostream& operator<<(std::ostream& os, const Filter& filter);
};

std::ostream& operator<<(std::ostream& os, const Filter& filter);


/** @brief   changes the active image index, the image iwth the active index gets displayed and converted by default when no path is provided */
class ActiveIndexFilter : public Filter {
public:
    ActiveIndexFilter(bool active, int index) : Filter(active), my_index(index) {}

    void apply(unsigned char* pixels, int width, int height) override {}

    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    std::ostream& print(std::ostream& os) const override {
        os << "Active index:\t\033[95m" << my_index<< "\033[0m\t-\tModify using:\ta number in range 1-10";
        return os;
    }

    double get_value() override {
        return my_index;
    }

    void update(const std::string & command) override { //double index
        int vall = stoi(command);
        if(vall > 0 && vall < 100)
            my_index = vall;
    }

    void save(std::ofstream& stream) const override {
        stream << "ActiveIndexFilter " << std::boolalpha << isActive << " " << my_index << std::endl;
    }

private:
    int my_index;
};


/** @brief   changes the output width */
class WidthMaximiserFilter : public Filter {
public:
    WidthMaximiserFilter(bool active, double widthmaximiser) : Filter(active), widthMaximiser(widthmaximiser) {}

    void apply(unsigned char* pixels, int width, int height) override {
        
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    void update(const std::string & command) override{
        if(command == "q" && widthMaximiser > 1 ) widthMaximiser --;
        else if(command == "w" && widthMaximiser < 3) widthMaximiser ++;
    }

    double get_value() override{
        return widthMaximiser;
    }

    std::ostream& print(std::ostream& os) const override {
        os << "Width:\t\t\033[95m" << widthMaximiser << "\033[0m\t-\tModify using:\t'q' -> (-)\t'w' -> (+)";
        return os;
    }

    void save(std::ofstream& stream) const override {
        stream << "WidthMaximiserFilter " << std::boolalpha << isActive << " " << widthMaximiser << std::endl;
    }

private:
    double widthMaximiser;
};


/** @brief   changes the output height */
class HeightMaximiserFilter : public Filter {
public:
    HeightMaximiserFilter(bool active, double heightMaximiser) : Filter(active), heightMaximiser(heightMaximiser) {}

    void apply(unsigned char* pixels, int width, int height) override {
        
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    // void update(double newheightMaximiser){  heightMaximiser = newheightMaximiser; }

    void update(const std::string & command) override{
        if(command == "e" && heightMaximiser > 1) heightMaximiser --;
        else if(command == "r" && heightMaximiser < 5) heightMaximiser ++;
    }

    double get_value() override{
        return heightMaximiser;
    }

    std::ostream& print(std::ostream& os) const override {
        os << "Height:\t\t\033[95m" << heightMaximiser << "\033[0m\t-\tModify using:\t'e' -> (-)\t'r' -> (+)";
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "HeightMaximiserFilter " << std::boolalpha << isActive << " " << heightMaximiser << std::endl;
    }


private:
    double heightMaximiser;
};

/** @brief   changes the output size */
class StepFilter : public Filter {
public:
    StepFilter(bool active, int step) : Filter(active), my_step(step) {}

    void apply(unsigned char* pixels, int width, int height) override {
        
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    void update(const std::string & command) override {
        if(command == "t" && my_step > 1) my_step --;
        else if(command == "y" && my_step < 50) my_step ++;
    }

    double get_value() override { return my_step;}


    std::ostream& print(std::ostream& os) const override {
        os << "Step:\t\t\033[95m" << my_step << "\033[0m\t-\tModify using:\t't' -> (-)\t'y' -> (+)";
        return os;
    }

    void save(std::ofstream& stream) const override {
        stream << "StepFilter " << std::boolalpha << isActive << " " << my_step << std::endl;
    }

    private:
        int my_step;

};


/** @brief  changes the output brightness */
class BrightnessFilter : public Filter {
public:
    BrightnessFilter(bool active, int brightness) : Filter(active), brightness(brightness) {}

    void apply(unsigned char* pixels, int width, int height) override {
        if (brightness != 5) {
            double scale = static_cast<double>(brightness) / 5.0;
            for (int i = 0; i < width * height * 3; i++) {
                pixels[i] = std::max(0.0, std::min(255.0, scale * (pixels[i] - 128) + 128));
            }
        }
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}


    double get_value() override { return brightness;}

    void update(const std::string & command) override {
        if(command == "u" && brightness>1) brightness --;
        else if(command == "i" && brightness<10) brightness ++;
    }

    void save(std::ofstream& stream) const override {
        stream << "BrightnessFilter " << std::boolalpha << isActive << " " << brightness << std::endl;
    }

    std::ostream& print(std::ostream& os) const override {
        os << "Brightness:\t\033[95m" << brightness << "\033[0m\t-\tModify using:\t'u' -> (-)\t'i' -> (+)";
        return os;
    }

private:
    int brightness;
};


/** @brief   Filter converting imnage to it's negative color */
class NegativeFilter : public Filter {
public:
    NegativeFilter(bool active) : Filter(active) {}

    void apply(unsigned char* pixels, int width, int height) override {
        for (int i = 0; i < width * height * 3; i++) {
            pixels[i] = 255 - pixels[i];
        }
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}
    
    void update(const std::string& command) override { }
    double get_value() override { return 0.0; }

    std::ostream& print(std::ostream& os) const override {
        os << "Negative\t\t-\tModify using:\t'negative'"; //<< (isActive ? "on" : "off") << std::endl;
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "NegativeFilter " << std::boolalpha << isActive << std::endl;
    }
};

/** @brief   If isActive, the output of the 'show' and 'ascii' is saved into file */
class SavingFilter : public Filter {
public:
    SavingFilter(bool active) : Filter(active) {}

    void apply(unsigned char* pixels, int width, int height) override {}
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    void update(const std::string& command) override { }
    double get_value() override { return 0.0; }

    std::ostream& print(std::ostream& os) const override {
        os << "Saving\t\t\t-\tModify using:\t'save'";//<< (isActive ? "on" : "off") << std::endl;
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "SavingFilter " << std::boolalpha << isActive << std::endl;
    }
};


/** @brief   If isActive converts image pixels based on convolution algorithm to the shapes of the surface lines */
class ConvolutionFilter : public Filter {
public:
    ConvolutionFilter(bool active) : Filter(active) {}

    void apply(unsigned char* m_pixels, int width, int height) override {
        int kernelSize = 3;
        double kernel[3][3] = {{-1, -1, -1},
                            {-1,  8, -1},
                            {-1, -1, -1}};

        // create a temporary copy of the image
        unsigned char* tempPixels = new unsigned char[width * height * 3];
        std::copy(m_pixels, m_pixels + width * height * 3, tempPixels);

        // loop through all pixels in the image
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // apply the convolution kernel to the pixel
                double sumR = 0, sumG = 0, sumB = 0;
                for (int j = 0; j < kernelSize; j++) {
                    for (int i = 0; i < kernelSize; i++) {
                        int pixelX = std::min(std::max(x + i - kernelSize / 2, 0), width - 1);
                        int pixelY = std::min(std::max(y + j - kernelSize / 2, 0), height - 1);
                        int pixelIndex = (pixelY * width + pixelX) * 3;
                        double kernelValue = kernel[j][i];
                        sumR += tempPixels[pixelIndex] * kernelValue;
                        sumG += tempPixels[pixelIndex + 1] * kernelValue;
                        sumB += tempPixels[pixelIndex + 2] * kernelValue;
                    }
                }

                // write the convolved pixel back to the image
                int pixelIndex = (y * width + x) * 3;
                m_pixels[pixelIndex] = std::max(0.0, std::min(255.0, sumR));
                m_pixels[pixelIndex + 1] = std::max(0.0, std::min(255.0, sumG));
                m_pixels[pixelIndex + 2] = std::max(0.0, std::min(255.0, sumB));
            }
        }

        // free the temporary copy of the image
        delete[] tempPixels;
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    void update(const std::string& command) override { }
    double get_value() override { return 0.0; }

    std::ostream& print(std::ostream& os) const override {
        os << "Convolution\t\t-\tModify using:\t'convolution'";// << (isActive ? "on" : "off") << std::endl;
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "ConvolutionFilter " << std::boolalpha << isActive << std::endl;
    }
};

/** @brief  if isActive converts image to greyscale*/
class GreyScaleFilter : public Filter {
public:
    GreyScaleFilter(bool active) : Filter(active) {}

    void apply(unsigned char* m_pixels, int width, int height) override {
        for (int i = 0; i < width * height * 3; i += 3) {
            // calculate the average of the three color values
            int avgColor = (m_pixels[i] + m_pixels[i + 1] + m_pixels[i + 2]) / 3;

            // set each color value to the average value
            m_pixels[i] = avgColor;
            m_pixels[i + 1] = avgColor;
            m_pixels[i + 2] = avgColor;
        }
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}

    void update(const std::string& command) override { }
    double get_value() override { return 0.0; }
    
    std::ostream& print(std::ostream& os) const override {
        os << "Grey\t\t\t-\tModify using:\t'grey'";
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "GreyScaleFilter " << std::boolalpha << isActive << std::endl;
    }
};

/** @brief   if isActive converts image to greyscale */
class AnimationDelayFilter : public Filter {
public:
    AnimationDelayFilter(bool active, double delay) : Filter(active), animation_delay(delay) {}

    void apply(unsigned char* m_pixels, int width, int height) override {
        
    }
    void printStartCode(std::ostream& os) const override {}
    void printEndCode(std::ostream& os) const override {}


    double get_value() override { return animation_delay;}

    void update(const std::string & command) override {
        if(command == "o" && animation_delay>1) animation_delay --;
        else if(command == "p" && animation_delay<100) animation_delay ++;
    }

    std::ostream& print(std::ostream& os) const override {
        os << "Delay:\t\t\033[95m" << animation_delay << "\033[0m\t-\tModify using:\t'o' -> (-)\t'p' -> (+)";
        return os;
    }
    void save(std::ofstream& stream) const override {
        stream << "AnimationDelayFilter " << std::boolalpha << isActive << " " << animation_delay << std::endl;
    }
    private:
        int animation_delay;
};


/** @brief   changes printout color of ascii */
class ColorFilter : public Filter {
public:
    ColorFilter(bool active, ColorsEnums in_color) : Filter(active), color(in_color) {}

    void apply(unsigned char* m_pixels, int width, int height) override {
        
    }

    double get_value() override { return 0.0;}

    void update(const std::string & command) override {
        if(command == "red" )  color = ColorsEnums::Red;
        else if(command == "green") color = ColorsEnums::Green;
        else if(command == "blue") color = ColorsEnums::Blue;
        else if(command == "default") color = ColorsEnums::Standard;
    }

    std::ostream& print(std::ostream& os) const override {
        std::string color_name = "";
        if(color == ColorsEnums::Blue) color_name = "blue";
        else if(color == ColorsEnums::Red) color_name = "red";
        else if(color == ColorsEnums::Green) color_name = "green"; 
        else if(color == ColorsEnums::Standard) color_name = "default"; 
        os << "Color:\t\t\033[95m" << color_name << "\033[0m\t-\tModify using:\t'red/green/blue/default'";
        return os;
    }

    void printStartCode(std::ostream& os) const override {
        if (color == ColorsEnums::Red) {
            os << "\033[31m"; // Red color start code
        } else if (color == ColorsEnums::Green) {
            os << "\033[32m"; // Green color start code
        }  else if (color == ColorsEnums::Blue) {
            os <<"\033[34m"; // Blue color start code
        } 
    }

    void printEndCode(std::ostream& os) const override {
        if (color == ColorsEnums::Red || color == ColorsEnums::Green) {
            os << "\033[0m"; // Reset color code
        }
    }
    void save(std::ofstream& stream) const override {
        int color_int = 1;
        if(color == ColorsEnums::Red) color_int = 2;
        if(color == ColorsEnums::Green) color_int = 3;
        if(color == ColorsEnums::Blue) color_int = 4;
        stream << "ColorFilter " << std::boolalpha << isActive << " " << color_int << std::endl;
    }

    private:
        ColorsEnums color;
};


template<typename T, typename... Args>
void updateFilter(T& filter, Args&&... args) {
    filter.update(std::forward<Args>(args)...);
}
