#include "gtest/gtest.h"

#include "../itkautomaticcropimagefilter.h"
#include <itkImageFileWriter.h>


class IndexCompleteTester : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }


  virtual void TearDown() {
  }
};

class ImageCompleteTester : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }


  virtual void TearDown() {
  }
};

template <typename TImage>
void CreateSimpleImage(TImage* const image)
{
  typedef typename TImage::RegionType TRegion;
  typedef typename TImage::IndexType  TIndex;
  typedef typename TImage::SizeType   TSize;

  TIndex index;
  index.Fill(0);
  TSize size;
  size.Fill(1024);
  TRegion region = TRegion(index, size);

  image->SetRegions(region);
  image->Allocate();
  image->FillBuffer(0);

  index = size / 4;
  size  = size / 2;
  region = TRegion(index, size);
  itk::ImageRegionIterator<TImage> it(image, region);
  for (it.GoToBegin(); !it.IsAtEnd(); it++) {
    it.Set(255);
  }
}

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

template <typename TImage>
void CreateImageAndTestIndexAndSize(TImage* image, bool multithreaded) {
  CreateImage(image);

  //auto start = std::chrono::high_resolution_clock::now();
  typedef typename itk::itkAutomaticCropImageFilter<TImage, TImage>  CropFilter;
  typename CropFilter::Pointer filter = CropFilter::New();
  filter->SetInput(image);
  if (!multithreaded) {
    filter->SetNumberOfThreads(1);
  }
  filter->Update();
  //auto finish = std::chrono::high_resolution_clock::now();

  //std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count()/1000 << " microsecond\n";
  //OutputImage(image.GetPointer(), "input2DImage.tif");
  //OutputImage(filter->GetOutput(), "filtered2DImage.tif");
  itk::Index<TImage::ImageDimension> index;
  itk::Size<TImage::ImageDimension>  size;
  if (TImage::ImageDimension==2) {
    index = {{199, 229}};
    size  = {{283, 473}};
  } else if (TImage::ImageDimension==3) {
    index[0] = 199;
    index[1] = 229;
    index[2] = 24;
    size[0]  = 283;
    size[1]  = 472;
    size[2]  = 178;
  }
  ASSERT_EQ(filter->GetImageIndex(), index);
  ASSERT_EQ(filter->GetImageSize(), size);
}

TEST(IndexCompleteTester, unsignedCharImage2D) {
  typedef itk::Image<unsigned char, 2>   ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImageAndTestIndexAndSize(image.GetPointer(), 1);
}

TEST(IndexCompleteTester, unsignedCharImage3D) {
  typedef itk::Image<unsigned char, 3>   Image3DType;
  Image3DType::Pointer image3D = Image3DType::New();
  CreateImageAndTestIndexAndSize(image3D.GetPointer(), 1);
}

TEST(IndexCompleteTester, signedCharImage2D) {
  typedef itk::Image<signed char, 2>   ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImageAndTestIndexAndSize(image.GetPointer(), 1);
}

TEST(IndexCompleteTester, signedCharImage3D) {
  typedef itk::Image<signed char, 3>   Image3DType;
  Image3DType::Pointer image3D = Image3DType::New();
  CreateImageAndTestIndexAndSize(image3D.GetPointer(), 1);
}

TEST(IndexCompleteTester, unsignedShortImage2D) {
  typedef itk::Image<unsigned short, 2>   ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImageAndTestIndexAndSize(image.GetPointer(), 1);
}

TEST(IndexCompleteTester, unsignedShortImage3D) {
  typedef itk::Image<unsigned short, 3>   Image3DType;
  Image3DType::Pointer image3D = Image3DType::New();
  CreateImageAndTestIndexAndSize(image3D.GetPointer(), 1);
}

TEST(IndexCompleteTester, signedShortImage2D) {
  typedef itk::Image<signed short, 2>   ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImageAndTestIndexAndSize(image.GetPointer(), 1);
}

TEST(IndexCompleteTester, signedShortImage3D) {
  typedef itk::Image<signed short, 3>   Image3DType;
  Image3DType::Pointer image3D = Image3DType::New();
  CreateImageAndTestIndexAndSize(image3D.GetPointer(), 1);
}

TEST(IndexCompleteTester, notMultithreaedUnsignedCharImage2D) {
  typedef itk::Image<unsigned char, 2>   ImageType;
  ImageType::Pointer image = ImageType::New();
  CreateImageAndTestIndexAndSize(image.GetPointer(), 0);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
