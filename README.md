# Simple-Asset-Packer
Simple cross platform asset packer for use with games &amp; standalone applications

# CLI Usage
Create a packed imagine containing a list of files and directories:
```console
assetpacker.exe <list of input files / directories>
```
List all the packed files in a bin:
```console
assetpacker.exe image_name.bin
```
# C++ Usage
Loading an asset image from a file:

```C++
#include "AssetPacker.h"
...
AssetPacker::fileImageMap_t files;
AssetPacker::loadImage("myImage.bin", files);
for (auto& i : files)
{
  std::cout << i.second.path << " (" << i.second.size << " bytes)" << std::endl;
}
```

Create an asset image:

```C++
#include "AssetPacker.h"
...
pathList_t files; // an std::vector of file paths
AssetPacker::makeImage("image.bin", files);
```
