# Simple-Asset-Packer
Simple asset packer for use with games &amp; standalone applications

# CLI Usage
Create a packed imagine containing a list of files and directories:
```
assetpacker.exe <list of input files / directories>
```
List all the packed files in a bin:
```
assetpacker.exe image_name.bin
```
# C++ Usage

``
#include "AssetPacker.h"
...
AssetPacker::fileImageMap_t files;
AssetPacker::loadImage("myImage.bin", files);
for (auto& i : files)
{
  std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
}
```
