#ifndef __CPNGLOADER_H__
#define __CPNGLOADER_H__

#include "CCommonTypes.h"

using namespace Types;

#include "png.h"

/* Performs the load of a PNG file and stores its content locally 
* for multiple files, recommend to instantiate many instances of this class
*/
class CPngLoader
{
public:

	/*
	* structure holding a single PNG file after deflacting
	*/
	struct SPngFile
	{
		// width attribute of this file image
		Int32 width;
		// height attribute of this file image
		Int32 height;
		// a single byte for the color type (usually gray scale for height map)
		png_byte color_type;
		// not sure yet
		png_byte bit_depth;
		// array of pixels with size of width*height*4 (4 for RGBA)
		png_bytep *pPixels;
		// status - was the load succesfull?
		bool status;
	};

	// constructor
	CPngLoader();
	// destructor and deallocates dynamic memory
	~CPngLoader();
	const bool Load(const Byte* filePath);
	// retrieve a reference of the loaded content
	SPngFile* GetFileContent();

private:
	// perform the loading of a file
	bool loadPngFile(const Byte* filePath);
	// content of the file
	SPngFile m_fileContent;

};

inline CPngLoader::SPngFile*
CPngLoader::GetFileContent()
{
	return &m_fileContent;
}

inline const bool
CPngLoader::Load(const Byte* filePath)
{
	// is the file loaded yet?
	if (!m_fileContent.status)
	{
		loadPngFile(filePath);
		m_fileContent.status = true;
	}

	return m_fileContent.status;
}

#endif // __CPNGLOADER_H__

