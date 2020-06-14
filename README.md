
# Simple-Asset-Packer
Simple cross platform asset packer for use with games &amp; standalone applications

The benefit of packing assets with this library vs resource files or binary blobs is still maintaining a virtual filesystem and cross platform compatibility.

# CLI Usage
Create a packed image containing a list of files and directories:
```console
assetpacker <list of input files / directories>
```
List all the packed files in a bin:
```console
assetpacker image_name.bin
```
List the contents of an embedded image:
```console
assetpacker find <filepath to embedded image>
```
extract the contents of an embedded/standalone image:
```console
assetpacker extract <filepath to embedded image>
```
Embed an image bin into an executable:
```console
assetpacker <standalone image bin> <target file>
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
