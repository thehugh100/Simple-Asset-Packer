#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace AssetPacker
{
	struct FileImage
	{
		FileImage();
		FileImage(std::string path, uint32_t size, const char* data);
		void free();
		std::string path;
		uint32_t size;
		const char* data;
	};

	typedef std::map<std::string, FileImage> fileImageMap_t;
	typedef std::vector<std::filesystem::path> pathList_t;

	extern void deleteFileImageMap(fileImageMap_t* fileImageMap);
	extern bool makeImage(fs::path imageName, pathList_t files);
	extern bool findImageInFile(fs::path filePath, fileImageMap_t& files, bool findLastImageInFile = 0);
	extern bool loadImageFromMemory(const char* imageData, uint32_t imageSize, fileImageMap_t& files);
	extern bool loadImage(fs::path imagePath, fileImageMap_t& files);
	extern bool appendImageToFile(fs::path imagePath, fs::path filePath);
}