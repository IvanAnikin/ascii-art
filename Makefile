CXX = g++
LD = g++
AR = ar
CXXFLAGS = -std=c++17 -Wall -pedantic

SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs)

SDL2_IMAGE_CFLAGS := $(shell pkg-config --cflags SDL2_image)
SDL2_IMAGE_LIBS := $(shell pkg-config --libs SDL2_image)
PNG_LIBS := -lpng
JPEG_LIBS := -ljpeg

INCLUDE_DIRS = -I./include $(SDL2_CFLAGS) $(SDL2_IMAGE_CFLAGS)
LIB_DIRS = -L./lib

HEADERS = $(wildcard src/*.h)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, build/%.o, $(SOURCES))

EXECUTABLE = asciiart

.PHONY: all compile clean run doc

all: $(EXECUTABLE) doc

run: $(EXECUTABLE)
	./$(EXECUTABLE)

compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(SDL2_LIBS) $(SDL2_IMAGE_LIBS) $(PNG_LIBS) $(JPEG_LIBS) -lSDL2 -lSDL2_image -pthread

build/%.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
