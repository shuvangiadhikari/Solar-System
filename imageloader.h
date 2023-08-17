//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
// This header file helps organise the image data 
#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

//Represents an image
//This class encapsulates the image's pixel data, width, and height.
class Image {
public:
	Image(char* ps, int w, int h);
	~Image();

	/* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
	* color of each pixel in image.  Color components range from 0 to 255.
	* The array starts the bottom-left pixel, then moves right to the end
	* of the row, then moves up to the next column, and so on.  This is the
	* format in which OpenGL likes images.
	*/
	char* pixels;
	int width;
	int height;
};

//Reads a BMP image file and converts its data into an Image object.
// Returns an instance of the 'Image' class
Image* loadBMP(const char* filename);

#endif
