#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include "AssetPacker.h"
#include <functional>
#include <tuple>

namespace fs = std::filesystem;

void printFileStats(fs::path p)
{
	std::cout << " [" << fs::file_size(p) << " bytes]\t" << p << std::endl;
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

void compileImage(int argc, char **argv)
{
	AssetPacker::pathList_t files;

	for (int i = 2; i < argc; ++i)
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
	std::cout << "written image.bin" << std::endl;
}

void listImage(int argc, char **argv)
{
	if (fs::exists(argv[2]) && fs::path(argv[2]).extension().compare(".bin") == 0)
	{
		AssetPacker::fileImageMap_t files;
		AssetPacker::loadImage(argv[2], files);
		std::cout << "Image Bin Listing: " << argv[2] << std::endl;
		for (auto& i : files)
		{
			std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
		}
		exit(0);
	}
}

void findImage(int argc, char **argv)
{
	if(fs::exists(argv[2]))
	{
		AssetPacker::fileImageMap_t files;
		if (AssetPacker::findImageInFile(argv[2], files))
		{
			std::cout << "Image Bin Listing: " << argv[2] << std::endl;
			for (auto& i : files)
			{
				std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
			}
			exit(0);
		}
		else
		{
			std::cout << "Could not find image in file" << std::endl;
			exit(0);
		}
	}
	else
	{
		std::cout << "input file " << argv[2] << " does not exist" << std::endl;
	}
}

void extractImage(int argc, char **argv)
{
	if(fs::exists(argv[2]))
	{
		AssetPacker::fileImageMap_t files;
		if (AssetPacker::findImageInFile(argv[2], files))
		{
			std::cout << "Image Bin Listing: " << argv[2] << std::endl;
			for (auto& i : files)
			{
				std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
			}

			if (files.size())
			{
				std::string path = "";
				std::cout << "Enter extraction path (blank for current folder): ";
				std::cin >> path;
				
				if (fs::create_directory(path))
				{
					for (auto& i : files)
					{
						//std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
						fs::create_directory(fs::path(path + "/" + i.second.path).parent_path());
						std::ofstream fileW(path + "/" + i.second.path, std::ios::binary);
						if(fileW)
						{
							fileW.write(i.second.data, i.second.size);
						}
						else
						{
							std::cout << "Failed to write extracted file: " << path + "/" + i.second.path << std::endl;
						}

						fileW.close();
					}
					std::cout << "Done." << std::endl;
				}
				else
				{
					std::cout << "Failed to create directory: " << path << ", extraction failed." << std::endl;
				}
			}
			exit(0);
		}
		else
		{
			std::cout << "Could not find image in file" << std::endl;
			exit(0);
		}
	}
	else
	{
		std::cout << "input file " << argv[2] << " does not exist" << std::endl;
	}
}

void appendImage(int argc, char **argv) 
{
	if (fs::exists(argv[2]) && fs::path(argv[2]).extension().compare(".bin") == 0 && 
			fs::exists(argv[3]))
	{
		AssetPacker::appendImageToFile(argv[2], argv[3]);
	}
}

int main(int argc, char **argv)
{
	std::map<std::string, std::tuple<std::string, std::function<void(int, char**)>>> commands = {
		{"-compile", {"Compiles a directory into an image.bin\n\tUsage: assetpacker -compile ./directory <multiple files/directories>", compileImage}},
		{"-list", {"lists files inside an image\n\tUsage: assetpacker -list image.bin", listImage}},
		{"-find", {"finds and lists an image embedded within another file\n\tUsage: assetpacker -find test.exe", findImage}},
		{"-extract", {"extracts an image file into a directory (prompts for directory)\n\tUsage: assetpacker -extract image.bin", extractImage}},
		{"-append", {"appends an image file to another file\n\tUsage: assetpacker -append image.bin test.exe", appendImage}},
		{"-help", {"Displays this help text", [&](int argc, char **argv) {
			for(auto &i : commands) {
				std::cout << i.first << " : " << std::get<0>(i.second) << std::endl;
			}
		}}},
	};
	if(argc >= 3)
	{
		if(commands.find(argv[1]) != commands.end()) {
			std::get<1>(commands.at(argv[1]))(argc, argv);
			std::cout << "Done." << std::endl;
		} else {
			std::cout << "Unknown command: " << argv[1] << std::endl;
			std::get<1>(commands.at("-help"))(argc, argv);
		}
	} else {
		std::get<1>(commands.at("-help"))(argc, argv);
	}
}