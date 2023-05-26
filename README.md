# ASCII art
Purpose of this project is to generate ascii art in console from images.

## Installation
To install the required libraries for the software run these commands on your linux:
    
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-image-dev
    sudo apt install libpng-dev
    sudo apt install libjpeg-dev

and other if you don't have the c++ standard libraries  

## Compilation - make
You can compile and run the application immediately using 

    make run

To compile the application and generate the documentation run

    make all

To generate the documentation only 

    make doc

To clean up software files

    make clean

## Usage - examples

Run the code using 

    ./asciiart

or 

    make run

as mentioned earlier

### Ascii image
My project can output a visual representation of the input images.

 To do so just type 

    ascii

 or 

    ascii filename

where filename is the path to your image. For example: 
     
    ascii examples/images/image_04.jpeg



### Ascii animation
If a directory is used for the input, output is an animation of all the images in the directory.
  To do so just type 

    ascii directory_name  

where directory_name is the path to your directoryFor example: 
     
    ascii examples/images/parkour3


### Show
User can see the image with applied filters
 To do so simply type 

    show

or 

    show filename

where filename is the path to your image. For example: 
     
    show examples/images/image_04.jpeg



## About the software:


### Input:
It accepts PNG and JPEG image formats. 

### Filters:
Conversion process is affected by filters set loaded from configuration file.

  Such filters are:
   - width/heigth ratio
   - size
   - brightness
   - negative
   - convolution
   - greyscale
   - speed of animation
   - color
   