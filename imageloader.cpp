//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
//loading and processing BMP files

#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

//to free the memory used by pixel data.
Image::~Image() {
	delete[] pixels;
}


//this namespace provides functions to convert between byte arrays and integer/short values using little-endian format.
//These utilities are used within the loadBMP function to process and interpret binary data from the BMP file and are generally helpful for handling low-level data manipulation tasks.
namespace {
	//Converts a four-character array to an integer, using little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	//Converts a two-character array to a short, using little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}

	//Reads the next four bytes as an integer, using little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	//Reads the next two bytes as a short, using little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	//Just like auto_ptr, but for arrays
	template<class T>
	class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}

		auto_array(const auto_array<T> &aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}

		T* get() const {
			return array;
		}

		T &operator*() const {
			return *array;
		}

		void operator=(const auto_array<T> &aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		T* operator->() const {
			return array;
		}

		T* release() {
			isReleased = true;
			return array;
		}

		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}

		T* operator+(int i) {
			return array + i;
		}

		T &operator[](int i) {
			return array[i];
		}
	};
}


//loads and processes a BMP file and returns an 'Image' object pointer. 
Image* loadBMP(const char* filename) {

	//opens the specified BMP image file in binary mode using an 'ifstream' object named input.
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);   //reads the first two bytes of the file into a buffer  
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file"); //checks if they correspond to the characters 'B' and 'M'.
	input.ignore(8);
	int dataOffset = readInt(input); //This value represents the offset in the file where the actual image data begins.
	//The above section reads the offset to the image data, which is needed to correctly position the file pointer for reading the image pixels.
	
	
	//Read the header (header helps determine format and version of the BMP being read from files
	int headerSize = readInt(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		//V3
		width = readInt(input);
		height = readInt(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		assert(readShort(input) == 0 || !"Image is compressed");
		break;
	case 12:
		//OS/2 V1
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
		break;
	case 64:
		//OS/2 V2
		assert(!"Can't load OS/2 V2 bitmaps");
		break;
	case 108:
		//Windows V4
		assert(!"Can't load Windows V4 bitmaps");
		break;
	case 124:
		//Windows V5
		assert(!"Can't load Windows V5 bitmaps");
		break;
	default:
		assert(!"Unknown bitmap format");
	}

	//Read the data
	/* BMP images are often padded to ensure that each row's data occupies a multiple of 4 bytes. 
	The formula used calculates the padded size based on the image width (width). 
	Each pixel in the image has 3 bytes (one for each of red, green, and blue), and the +3 and /4 operations ensure that the result is rounded up to the nearest multiple of 4.
	*/
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4); //represents the number of bytes needed to store one row of pixel data in the BMP image
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	//Get the data into the right format
	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	/*This section of the code calculates the necessary size for storing pixel data, 
	allocates memory for pixel storage, positions the file pointer to the start of the image data,
	and reads the pixel data from the BMP file into the allocated memory. 
	This prepares the raw pixel data for further processing and manipulation.
	*/
	input.close();
	return new Image(pixels2.release(), width, height);
}