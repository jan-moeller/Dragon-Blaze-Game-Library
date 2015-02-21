//////////////////////////////////////////////////////////////////////
/// Dragon Blaze Game Library
///
/// Copyright (c) 2015 by Jan Moeller
///
/// This software is provided "as-is" and does not claim to be
/// complete or free of bugs in any way. It should work, but
/// it might also begin to hurt your kittens.
//////////////////////////////////////////////////////////////////////

#include <cstring>
#include <string>
#include <algorithm>
#include <fstream>
#include "DBGL/Resources/Texture/IImageFormatModule.h"
#include "DBGL/Platform/Platform.h"
#include "DBGL/Platform/Texture/ITextureCommands.h"

namespace dbgl
{
	class BMPModule: public IImageFormatLibrary
	{
	private:
		void flipVertical(char* img, unsigned int width, unsigned int height) const
		{
			unsigned int bufsize = width * 3;
			if (bufsize % 4 != 0)
				bufsize += bufsize % 4;
			char* lineBuffer = new char[bufsize];
			for (unsigned int i = 0; i < height / 2; i++)
			{
				int frontOffset = i * bufsize;
				int backOffset = ((height - 1) - i) * bufsize;
				memcpy(lineBuffer, img + frontOffset, bufsize);
				memcpy(img + frontOffset, img + backOffset, bufsize);
				memcpy(img + backOffset, lineBuffer, bufsize);
			}
			delete[] lineBuffer;
		}

	public:
		struct FileHeaderBMP
		{
			uint16_t id = 0x4D42;
			uint32_t fileSize = 0;
			uint32_t res = 0;
			uint32_t off = 54;
		};

		struct InfoHeaderBMP
		{
			uint32_t size = 40;
			int32_t width = 0;
			int32_t height = 0;
			uint16_t panes = 1;
			uint16_t bpp = 24;
			uint32_t compr = 0;
			uint32_t imgSize = 0;
			int32_t xPixPerMeter = 0;
			int32_t yPixPerMeter = 0;
			uint32_t indexClr = 0;
			uint32_t clr = 0;
		};

		virtual ~BMPModule() = default;

		virtual bool canLoad() const
		{
			return true;
		}

		virtual bool canWrite() const
		{
			return true;
		}

		virtual bool matchExtension(std::string const& extension) const
		{
			std::string lowercaseExt { };
			std::transform(extension.begin(), extension.end(), std::back_inserter(lowercaseExt), ::tolower);
			return lowercaseExt == ".bmp" || lowercaseExt == "bmp";
		}

		virtual ITexture* load(std::string const& path) const
		{
			return load(Filename { path });
		}

		virtual ITexture* load(Filename const& path) const
		{
			// Open file stream
			std::ifstream file(path.get(), std::fstream::in | std::fstream::binary);
			if (!file.good())
				return nullptr;
			// Read file header
			file.seekg(0, std::ios::beg);
			FileHeaderBMP fileHeader { };
			char fHeader[14];
			file.read(&fHeader[0], 14);
			fileHeader.id = *reinterpret_cast<uint16_t*>(&fHeader[0]);
			fileHeader.fileSize = *reinterpret_cast<uint32_t*>(&fHeader[2]);
			fileHeader.res = *reinterpret_cast<uint32_t*>(&fHeader[6]);
			fileHeader.off = *reinterpret_cast<uint32_t*>(&fHeader[10]);
			if (fileHeader.id != 0x4D42 || fileHeader.fileSize <= 0)
				return nullptr;
			// Read info header
			InfoHeaderBMP infoHeader { };
			char iHeader[40];
			file.read(&iHeader[0], 40);
			infoHeader.size = *reinterpret_cast<uint32_t*>(&iHeader[0]);
			infoHeader.width = *reinterpret_cast<uint32_t*>(&iHeader[4]);
			infoHeader.height = *reinterpret_cast<uint32_t*>(&iHeader[8]);
			infoHeader.panes = *reinterpret_cast<uint16_t*>(&iHeader[10]);
			infoHeader.bpp = *reinterpret_cast<uint16_t*>(&iHeader[12]);
			infoHeader.compr = *reinterpret_cast<uint32_t*>(&iHeader[16]);
			infoHeader.imgSize = *reinterpret_cast<uint32_t*>(&iHeader[20]);
			infoHeader.xPixPerMeter = *reinterpret_cast<uint32_t*>(&iHeader[24]);
			infoHeader.yPixPerMeter = *reinterpret_cast<uint32_t*>(&iHeader[28]);
			infoHeader.indexClr = *reinterpret_cast<uint32_t*>(&iHeader[32]);
			infoHeader.clr = *reinterpret_cast<uint32_t*>(&iHeader[36]);
			if (infoHeader.size != 40 || infoHeader.width <= 0 || infoHeader.height <= 0 || infoHeader.compr != 0)
				return nullptr;
			// Read image data
			if (fileHeader.off > 0)
				file.seekg(fileHeader.off, std::ios::beg);
			const unsigned int imgSize { fileHeader.fileSize - fileHeader.off };
			char img[imgSize];
			file.read(&img[0], imgSize);
			// Close file
			file.close();

			// Flip vertically since bmps are layed out the wrong way
//			flipVertical(&img[0], infoHeader.width, infoHeader.height);
			// Create texture
			auto tex = Platform::get()->createTexture(ITexture::Type::TEX2D);
			tex->bind();
			Platform::get()->curTexture()->setRowAlignment(ITextureCommands::RowAlignment::UNPACK, 4);
			Platform::get()->curTexture()->write(0, infoHeader.width, infoHeader.height,
					ITextureCommands::PixelFormat::BGR, ITextureCommands::PixelType::UBYTE, &img[0]);
			return tex;
		}

		virtual bool write(ITexture* tex, std::string const& path) const
		{
			return write(tex, Filename { path });
		}

		virtual bool write(ITexture* tex, Filename const& path) const
		{
			tex->bind();
			unsigned int width = Platform::get()->curTexture()->getWidth();
			unsigned int height = Platform::get()->curTexture()->getHeight();
			unsigned int imgDataSize { width * height * 3 };
			char* bmpBuf = new char[imgDataSize];
			if (!bmpBuf)
				return false;
			// Copy pixel data to buffer
			Platform::get()->curTexture()->setRowAlignment(ITextureCommands::RowAlignment::PACK, 4);
			Platform::get()->curTexture()->getPixelData(ITextureCommands::PixelFormat::BGR,
					ITextureCommands::PixelType::UBYTE, bmpBuf);
			// Open file stream
			std::ofstream file(path.get(), std::fstream::out | std::fstream::binary | std::fstream::trunc);
			if (!file.good())
				return false;
			// Write file header
			FileHeaderBMP fileHeader { };
			fileHeader.fileSize = imgDataSize;
			file.write(reinterpret_cast<const char*>(&fileHeader.id), sizeof(uint16_t)); // "BM"
			file.write(reinterpret_cast<const char*>(&fileHeader.fileSize), sizeof(uint32_t)); // File size
			file.write(reinterpret_cast<const char*>(&fileHeader.res), sizeof(uint32_t)); // Reserved
			file.write(reinterpret_cast<const char*>(&fileHeader.off), sizeof(uint32_t)); // Offset to image data
			// Write info header
			InfoHeaderBMP infoHeader { };
			infoHeader.width = static_cast<int32_t>(width);
			infoHeader.height = static_cast<int32_t>(height);
			file.write(reinterpret_cast<const char*>(&infoHeader.size), sizeof(uint32_t)); // Size of info header
			file.write(reinterpret_cast<const char*>(&infoHeader.width), sizeof(int32_t)); // Width in pixels
			file.write(reinterpret_cast<const char*>(&infoHeader.height), sizeof(int32_t)); // Height in pixels
			file.write(reinterpret_cast<const char*>(&infoHeader.panes), sizeof(uint16_t)); // Color planes (unused)
			file.write(reinterpret_cast<const char*>(&infoHeader.bpp), sizeof(uint16_t)); // BPP
			file.write(reinterpret_cast<const char*>(&infoHeader.compr), sizeof(uint32_t)); // Not compressed
			file.write(reinterpret_cast<const char*>(&infoHeader.imgSize), sizeof(uint32_t)); // Size of image data (unused)
			file.write(reinterpret_cast<const char*>(&infoHeader.xPixPerMeter), sizeof(int32_t)); // X pixel per meter (unused)
			file.write(reinterpret_cast<const char*>(&infoHeader.yPixPerMeter), sizeof(int32_t)); // Y pixel per meter (unused)
			file.write(reinterpret_cast<const char*>(&infoHeader.indexClr), sizeof(uint32_t)); // Amount of indexed colors (no indexing)
			file.write(reinterpret_cast<const char*>(&infoHeader.clr), sizeof(uint32_t)); // Use all colors
			// Write actual image
			file.write(bmpBuf, imgDataSize);
			file.close();
			delete[] bmpBuf;

			return true;
		}
	};
}

extern "C" dbgl::IImageFormatLibrary* create()
{
	return new dbgl::BMPModule { };
}

extern "C" void destroy(dbgl::IImageFormatLibrary* mod)
{
	delete mod;
}
