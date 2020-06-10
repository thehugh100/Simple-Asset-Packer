#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace AssetPacker
{
	struct FileImage
	{
		FileImage();
		FileImage(std::string path, uint32_t size, const char* data);
		std::string path;
		uint32_t size;
		const char* data;
	};

	typedef std::map<std::string, FileImage> fileImageMap_t;
	typedef std::vector<std::filesystem::path> pathList_t;

	extern bool makeImage(fs::path imageName, pathList_t files);
	extern bool loadImage(fs::path imagePath, fileImageMap_t& files);
}