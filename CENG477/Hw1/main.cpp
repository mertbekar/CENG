#include <iostream>
#include "parser.h"
#include "ppm.h"

using namespace std;

typedef unsigned char RGB[3];


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.

    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    for(int cm = 0; cm < scene.cameras.size(); cm++)
    {
    	int width = scene.cameras[cm].image_width;
		int height = scene.cameras[cm].image_height;
	    unsigned char* image = new unsigned char [width * height * 3];
	    parser::Vec3f rgb;
	    rgb.x = scene.background_color.x;
	    rgb.y = scene.background_color.y;
	    rgb.z = scene.background_color.z;
	    int i = 0;
	    
    	for (int y = 0; y < height; ++y)
    	{
        	for (int x = 0; x < width; ++x)
        	{
	        	scene.rayEquation(x, y, cm, rgb);
	        	image[i++] = (int) rgb.x;
	            image[i++] = (int) rgb.y;
	            image[i++] = (int) rgb.z;
        	}
    	}
    write_ppm(scene.cameras[cm].image_name.c_str(), image, width, height);
	}
}
