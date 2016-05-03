# AutomaticBinaryCroper
Automatically crops a binary image using itk

# Description
The main application creates a phantom image and calls the filter for aplying the automatic croping. This filter uses a multithreaded approach to find the minimum box that contains all the image data (except the zeros). This works for 2D or 3D images with any pixel type that it can be used as binary.

# Required
CMake
Itk