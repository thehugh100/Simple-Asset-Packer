
# Simple-Asset-Packer
Simple cross platform asset packer for use with games &amp; standalone applications

The benefit of packing assets with this library vs resource files or binary blobs is still maintaining a virtual filesystem and cross platform compatibility.

# CLI Usage
```console
-append : appends an image file to another file
        Usage: assetpacker -append image.bin test.exe
-compile : Compiles a directory into an image.bin
        Usage: assetpacker -compile ./directory <multiple files/directories>
-extract : extracts an image file into a directory (prompts for directory)
        Usage: assetpacker -extract image.bin
-find : finds and lists an image embedded within another file
        Usage: assetpacker -find test.exe
-help : Displays this help text
-list : lists files inside an image
        Usage: assetpacker -list image.bin
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
AssetPacker::pathList_t files; // an std::vector of file paths
AssetPacker::makeImage("image.bin", files);
```
Find an image embedded in a file:
```C++
#include "AssetPacker.h"
...
AssetPacker::fileImageMap_t files;
if (AssetPacker::findImageInFile("myEmbeddedImage.exe", files))
{
	for (auto& i : files)
	{
		std::cout << i.second.path << " (" << second.size << " bytes)" << std::endl;
	}
	exit(0);
}
else
{
	std::cout << "Could not find image in file" << std::endl;
	exit(0);
}
```
