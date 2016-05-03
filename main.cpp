#include <iostream>

#include "itkautomaticcropimagefilter.h"
#include "itkRGBPixel.h"
#include <itkImageFileWriter.h>

typedef itk::Image<int, 2>   ImageType;
//typedef itk::ImageRegionIterator<int, 2 >


template <typename TImage>
void CreateImage(TImage* const image)
{
  // Create an image with 2 connected components
  typename TImage::IndexType corner;
  corner.Fill(0);

  unsigned int NumRows = 512;
  unsigned int NumCols = 768;
  typename TImage::SizeType size = {{NumRows, NumCols}};

  typename TImage::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  ImageType::IndexType pixelIndex;
  for(unsigned int r = 200; r <= 280; r++) {
    for(unsigned int c = 230; c <= 300; c++) {
        pixelIndex[0] = r;
        pixelIndex[1] = c;

        image->SetPixel(pixelIndex, 255);
    }
  }
  image->SetPixel({{199, 250}}, 255);
  image->SetPixel({{250, 229}}, 255);

  for(unsigned int r = 400; r <= 480; r++) {
    for(unsigned int c = 530; c <= 700; c++) {
        pixelIndex[0] = r;
        pixelIndex[1] = c;

        image->SetPixel(pixelIndex, 255);
    }
  }
}

template <typename TImage>
void OutputImage(TImage* const image, const char* fileName)
{
    typedef itk::ImageFileWriter<TImage> ImageWriter;
    typename ImageWriter::Pointer writer = ImageWriter::New();
    writer->SetInput(image);
    writer->SetFileName(fileName);
    writer->Update();
}

int main(int, char*[])
{
  // Setup types
  typedef itk::Image<unsigned char, 2>   ImageType;
  typedef itk::itkAutomaticCropImageFilter<ImageType, ImageType>  CropFilter;

  ImageType::Pointer image = ImageType::New();
  CreateImage(image.GetPointer());

    auto start = std::chrono::high_resolution_clock::now();
  // Create and the filter
  CropFilter::Pointer filter = CropFilter::New();
  filter->SetInput(image);
  //filter->SetNumberOfThreads(1); // There is no need to specify this, it is automatically determined
  filter->Update();
  auto finish = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()/1000 << " microsecond\n";
  OutputImage(image.GetPointer(), "inputImage.tif");

  OutputImage(filter->GetOutput(), "filteredImage.tif");

  return EXIT_SUCCESS;
}

