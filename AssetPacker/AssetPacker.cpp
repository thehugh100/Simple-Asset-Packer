#include "AssetPacker.h"

namespace AssetPacker
{
	bool makeImage(fs::path imageName, pathList_t files)
	{
		if (files.size() == 0)
			return false;

		std::ofstream image(imageName, std::ios::binary);
		for (auto& i : files)
		{
			std::ifstream file(i, std::ios::binary, std::ios::binary);

			uintmax_t size = fs::file_size(i);

			std::cout << "Packing: " << i.generic_string().c_str() << " [" << size << " bytes]" << std::endl;
			char* fData = new char[size];
			file.read(fData, size);
			uintmax_t pathLen = i.generic_string().size();
			image.write((const char*)&pathLen, 4);
			image.write((const char*)i.generic_string().c_str(), i.generic_string().size());
			image.write((const char*)&size, 4);
			image.write(fData, size);
			file.close();
			delete[] fData;
		}
		image.close();

		return true;
	}

	bool loadImage(fs::path imagePath, fileImageMap_t& files)
	{
		std::ifstream file(imagePath, std::ios::binary);
		if (file)
		{
			uintmax_t imageSize = fs::file_size(imagePath);
			char* fileData = new char[imageSize];
			file.read(fileData, imageSize);
			file.close();

			uint32_t filePointer = 0;
			while (filePointer < imageSize)
			{
				uint32_t titleLen = 0;
				uint32_t fileSize = 0;
				char title[256];
				memset(title, 0, 256);

				memcpy(&titleLen, &fileData[filePointer], 4);
				filePointer += 4;
				memcpy(title, &fileData[filePointer], titleLen);
				filePointer += titleLen;
				memcpy(&fileSize, &fileData[filePointer], 4);
				filePointer += 4;
				char* LocalFileData = new char[fileSize];
				memcpy(LocalFileData, &fileData[filePointer], fileSize);
				filePointer += fileSize;

				files[title] = FileImage(title, fileSize, LocalFileData);
			}
			delete[] fileData;
			return true;
		}
		std::cout << "Failed to load image." << std::endl;
		file.close();
		return false;
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