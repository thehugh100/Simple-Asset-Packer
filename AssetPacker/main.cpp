#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include "AssetPacker.h"

namespace fs = std::filesystem;

void printFileStats(fs::path p)
{
	std::wcout << " [" << fs::file_size(p) << " bytes]\t" << p.native() << std::endl;
}

void scanDirectory(fs::directory_entry directory, AssetPacker::pathList_t& files)
{
	for (auto& i : fs::directory_iterator(directory))
	{
		if (fs::is_directory(i))
		{
			std::cout << "Entered: " << i.path().generic_string() << std::endl;
			scanDirectory(i, files);
		}
		else if (fs::is_regular_file(i))
		{
			files.push_back(i.path());
			std::cout << "\t";
			printFileStats(i);
		}
	}
}

int main(int argc, char **argv)
{
	std::vector<std::filesystem::path> files;
	if (argc == 2)
	{
		if (fs::exists(argv[1]) && fs::path(argv[1]).extension().compare(".bin") == 0)
		{
			AssetPacker::fileImageMap_t files;
			AssetPacker::loadImage(argv[1], files);
			std::cout << "Image Bin Listing: " << argv[1] << std::endl;
			for (auto& i : files)
			{
				std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
			}
			exit(0);
		}
	}
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (fs::exists(argv[i]))
			{
				if (fs::is_directory(argv[i]))
				{
					scanDirectory(fs::directory_entry(argv[i]), files);
				}
				else
				{
					files.push_back(argv[i]);
					printFileStats(files.back());
				}
			}
			else
			{
				std::cout << "Error: " << argv[i] << " does not exist." << std::endl;
				exit(0);
			}
		}

		AssetPacker::makeImage("image.bin", files);
	}
		
	std::cout << "Done." << std::endl;
	std::cin.get();
}