#include "AssetPacker.h"

namespace AssetPacker
{
	bool makeImage(fs::path imageName, pathList_t files)
	{
		if (files.size() == 0)
			return false;

		std::ofstream image(imageName, std::ios::binary);

		if (!image)
		{
			std::cout << "Could not open file for writing: " << imageName << std::endl;
			return 0;
		}

		image.write("ASSET", 5);
		image.write("\x00\x00\x00\x00", 4);
		for (auto& i : files)
		{
			std::ifstream file(i, std::ios::binary, std::ios::binary);

			uint32_t size = fs::file_size(i);

			std::cout << "Packing: " << i.generic_string().c_str() << " [" << size << " bytes]" << std::endl;
			char* fData = new char[size];
			file.read(fData, size);
			uint32_t pathLen = i.generic_string().size();
			image.write((const char*)&pathLen, 4);
			image.write((const char*)i.generic_string().c_str(), i.generic_string().size());
			image.write((const char*)&size, 4);
			image.write(fData, size);
			file.close();
			delete[] fData;
		}
		uint32_t writtenSize = image.tellp();
		image.seekp(5);
		image.write((const char*)&writtenSize, 4);
		image.close();

		return true;
	}

	bool loadImageFromMemory(const char* imageData, uint32_t imageSize, fileImageMap_t& files)
	{
		uint32_t totalSize = 0;
		uint32_t filePointer = 0;
		if (memcmp("ASSET", &imageData[filePointer], 5) != 0)
		{
			std::cout << "Malformed Image File" << std::endl;
			return 0;
		}

		filePointer += 5;
		memcpy(&totalSize, &imageData[filePointer], 4);
		std::cout << "Image Size: " << totalSize << " bytes given, " << imageSize << " read." << std::endl;

		if (imageSize == 0)
			imageSize = totalSize;

		if (imageSize != totalSize)
		{
			std::cout << "Image size mismatch between header and buffer size" << std::endl;
			return 0;
		}

		filePointer += 4;

		while (filePointer < imageSize && filePointer < totalSize)
		{
			uint32_t titleLen = 0;
			uint32_t fileSize = 0;
			char title[256];
			memset(title, 0, 256);

			memcpy(&titleLen, &imageData[filePointer], 4);
			if (titleLen > 256)
			{
				std::cout << "Image filename exceeds limit" << std::endl;
				return 0;
			}
			filePointer += 4;
			memcpy(title, &imageData[filePointer], titleLen);
			filePointer += titleLen;
			memcpy(&fileSize, &imageData[filePointer], 4);
			if (fileSize > imageSize - filePointer)
			{
				std::cout << "Image file size exceeds container" << std::endl;
				return 0;
			}
			filePointer += 4;
			char* LocalFileData = new char[fileSize];
			memcpy(LocalFileData, &imageData[filePointer], fileSize);
			filePointer += fileSize;

			files[title] = FileImage(title, fileSize, LocalFileData);
		}
		return true;
	}

	bool findImageInFile(fs::path filePath, fileImageMap_t& files)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file)
		{
			uint32_t fileSize = fs::file_size(filePath);
			char* fileData = new char[fileSize];
			file.read(fileData, fileSize);
			file.close();

			for (int i = 0; i < fileSize - 16; ++i)
			{
				if (memcmp(fileData + i, "ASSET", 5) == 0)
				{
					if (loadImageFromMemory(fileData + i, 0, files))
						return true;
				}
			}

			delete[] fileData;
			return false;
		}
		std::cout << "Failed to load image." << std::endl;
		file.close();
		return false;
	}

	bool loadImage(fs::path imagePath, fileImageMap_t& files)
	{
		std::ifstream file(imagePath, std::ios::binary);
		if (file)
		{
			uint32_t imageSize = fs::file_size(imagePath);
			char* fileData = new char[imageSize];
			file.read(fileData, imageSize);
			file.close();
			bool ret = loadImageFromMemory(fileData, imageSize, files);
			delete[] fileData;
			return ret;
		}
		std::cout << "Failed to load image." << std::endl;
		file.close();
		return false;
	}

	bool appendImageToFile(fs::path imagePath, fs::path filePath)
	{
		std::ofstream file(filePath, std::ios::binary | std::ios::ate | std::ios::app);
		if (file)
		{
			uint32_t fileSize = fs::file_size(filePath);
			uint32_t imageFileSize = fs::file_size(imagePath);
			std::ifstream imageFile(imagePath, std::ios::binary);
			if (imageFile)
			{
				char* imageFileData = new char[imageFileSize];
				imageFile.read(imageFileData, imageFileSize);
				imageFile.close();
				file.write(imageFileData, imageFileSize);
				file.write((char*)&fileSize, 4);
				file.close();

				delete[] imageFileData;
				return true;
			}
			else
			{
				std::cout << "Cannot open file: " << imagePath << std::endl;
				imageFile.close();
				file.close();
				return 0;
			}
		}
		else
		{
			std::cout << "Cannot open file: " << filePath << std::endl;;
			file.close();
			return 0;
		}
		file.close();
		return true;
	}

	FileImage::FileImage()
	{
		data = nullptr;
		size = 0;
		path = "";
	}

	FileImage::FileImage(std::string path, uint32_t size, const char* data)
		:path(path), size(size), data(data)
	{
	}
}