#include <iostream>

#include "itkautomaticcropimagefilter.h"
#include "itkRGBPixel.h"
#include <itkImageFileWriter.h>

typedef itk::Image<unsigned char, 2>   ImageType;
typedef itk::Image<unsigned char, 3>   Image3DType;


template <typename TImage>
void CreateImage(TImage* const image)
{
  typename TImage::IndexType corner;
  corner.Fill(0);

  unsigned int NumRows = 512;
  unsigned int NumCols = 768;
  unsigned int NumSlds;
  typename TImage::SizeType size;
  if (image->ImageDimension == 2) {
    size = {{NumRows, NumCols}};
  } else if (image->ImageDimension == 3) {
    NumSlds = 256;
    size[0] = NumRows;
    size[1] = NumCols;
    size[2] = NumSlds;
  }

  typename TImage::RegionType region(corner, size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  typename TImage::IndexType pixelIndex;
  if (image->ImageDimension == 2) {
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
    image->SetPixel({{481, 600}}, 255);
    image->SetPixel({{450, 701}}, 255);
  } else if (image->ImageDimension == 3) {
    for(unsigned int r = 200; r <= 280; r++) {
      for(unsigned int c = 230; c <= 300; c++) {
        for(unsigned int s = 25; s <= 75; s++) {
          pixelIndex[0] = r;
          pixelIndex[1] = c;
          pixelIndex[2] = s;

          image->SetPixel(pixelIndex, 255);
        }
      }
    }
    pixelIndex[0] = 199;
    pixelIndex[1] = 250;
    pixelIndex[2] = 24;
    image->SetPixel(pixelIndex, 255);
    pixelIndex[0] = 250;
    pixelIndex[1] = 229;
    pixelIndex[2] = 76;
    image->SetPixel(pixelIndex, 255);

    for(unsigned int r = 400; r <= 480; r++) {
      for(unsigned int c = 530; c <= 700; c++) {
        for(unsigned int s = 100; s <= 200; s++) {
          pixelIndex[0] = r;
          pixelIndex[1] = c;
          pixelIndex[2] = s;

          image->SetPixel(pixelIndex, 255);
        }
      }
    }
    pixelIndex[0] = 481;
    pixelIndex[1] = 600;
    pixelIndex[2] = 99;
    image->SetPixel(pixelIndex, 255);
    pixelIndex[0] = 450;
    pixelIndex[1] = 700;
    pixelIndex[2] = 201;
    image->SetPixel(pixelIndex, 255);
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
  typedef itk::itkAutomaticCropImageFilter<ImageType, ImageType>  CropFilter;
  typedef itk::itkAutomaticCropImageFilter<Image3DType, Image3DType>  Crop3DFilter;

  ImageType::Pointer image = ImageType::New();
  CreateImage(image.GetPointer());

    auto start = std::chrono::high_resolution_clock::now();
  CropFilter::Pointer filter = CropFilter::New();
  filter->SetInput(image);
  //filter->SetNumberOfThreads(1); // There is no need to specify this, it is automatically determined
  filter->Update();
  auto finish = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()/1000 << " microsecond\n";
  OutputImage(image.GetPointer(), "input2DImage.tif");
  OutputImage(filter->GetOutput(), "filtered2DImage.tif");

  Image3DType::Pointer image3D = Image3DType::New();
  CreateImage(image3D.GetPointer());

  start = std::chrono::high_resolution_clock::now();
  Crop3DFilter::Pointer filter3D = Crop3DFilter::New();
  filter3D->SetInput(image3D);
  //filter->SetNumberOfThreads(1); // There is no need to specify this, it is automatically determined
  filter3D->Update();
  finish = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()/1000 << " microsecond\n";
  OutputImage(image3D.GetPointer(), "input3DImage.tif");
  OutputImage(filter3D->GetOutput(), "filtered3DImage.tif");

  return EXIT_SUCCESS;
}

