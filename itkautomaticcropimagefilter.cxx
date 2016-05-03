#ifndef __itkMultiThreadedImageFilter_cxx
    #define __itkMultiThreadedImageFilter_cxx

    #include "itkAutomaticCropImageFilter.h"


    namespace itk
    {
      template< class TInImage, class TOutImage>
      void itkAutomaticCropImageFilter <TInImage, TOutImage>
      ::BeforeThreadedGenerateData() {
        m_Input = this->GetInput();
        m_ImageIndex = m_Input->GetLargestPossibleRegion().GetIndex();
        m_ImageSize = m_Input->GetLargestPossibleRegion().GetSize();
        m_ImageFirstIndex = m_ImageIndex + m_ImageSize;
        m_ImageLastIndex = m_ImageIndex;
      }

      template< class TInImage, class TOutImage>
      void itkAutomaticCropImageFilter <TInImage, TOutImage>
      :: FindFirstPixel(const OutputImageRegionType region, unsigned int i) {
        const int N = m_Input->ImageDimension;
        unsigned int j = (i+1) % N;
        unsigned int k = (i-1) % N;

        InConstIteratorType it(m_Input, region);
        it.SetFirstDirection(i);
        it.SetSecondDirection(j);

        it.GoToBegin();
        while( !it.IsAtEnd()) {
          while( !it.IsAtEndOfSlice()) {
            while( !it.IsAtEndOfLine()) {
              if (m_ImageFirstIndex[k] < it.GetIndex()[k]) {
                return;
              }

              if (it.Get()) {
                mtx.lock();
                if (m_ImageFirstIndex[k] > it.GetIndex()[k]) {
                  m_ImageFirstIndex[k] = it.GetIndex()[k];
                }
                mtx.unlock();
                return;
              }
              ++it;
            }
            it.NextLine();
          }
          it.NextSlice();
         }
      }

      template< class TInImage, class TOutImage>
      void itkAutomaticCropImageFilter <TInImage, TOutImage>
      :: FindLastPixel(const OutputImageRegionType region, unsigned int i) {
        const int N = m_Input->ImageDimension;
        unsigned int j = (i+1) % N;
        unsigned int k = (i-1) % N;

        InConstIteratorType it(m_Input, region);
        it.SetFirstDirection(i);
        it.SetSecondDirection(j);

        it.GoToReverseBegin();
        while( !it.IsAtReverseEnd()) {
          while( !it.IsAtReverseEndOfSlice()) {
            while( !it.IsAtReverseEndOfLine()) {
              if (m_ImageLastIndex[k] > it.GetIndex()[k]) {
                return;
              }

              if (it.Get()) {
                mtx.lock();
                if (m_ImageLastIndex[k] < it.GetIndex()[k]) {
                  m_ImageLastIndex[k] = it.GetIndex()[k];
                }
                mtx.unlock();
                return;
              }
              --it;
            }
            it.PreviousLine();
          }
          it.PreviousSlice();
         }
      }

      template< class TInImage, class TOutImage>
      void itkAutomaticCropImageFilter <TInImage, TOutImage>
      ::ThreadedGenerateData(const OutputImageRegionType & region, ThreadIdType threadId)
      {
        for (unsigned int i = 0; i < m_Input->ImageDimension; i++) {
          FindFirstPixel(region, i);
          FindLastPixel(region, i);
        }
      }

      template< class TInImage, class TOutImage>
      void itkAutomaticCropImageFilter <TInImage, TOutImage>
      ::AfterThreadedGenerateData() {
        typename TOutImage::Pointer output = this->GetOutput();

        m_ImageIndex = m_ImageFirstIndex;
        for (int i=0; i<m_Input->ImageDimension; i++) {
          m_ImageSize[i] =  m_ImageLastIndex[i] - m_ImageFirstIndex[i];
        }

        itk::ImageRegion<TInImage::ImageDimension> imageRegion(m_ImageFirstIndex, m_ImageSize);
        itk::ImageRegionConstIterator<TInImage> inIt(m_Input, imageRegion);
        itk::Index<TInImage::ImageDimension> outIndex;
        outIndex.Fill(0);

        itk::ImageRegion<TInImage::ImageDimension> outImageRegion(outIndex, m_ImageSize);
        output->SetRegions(outImageRegion);
        output->Allocate();
        itk::ImageRegionIterator<TOutImage> outIt(output, outImageRegion);
        //itk::ImageRegionConstIterator<ImageType> imageIterator(image,image->GetLargestPossibleRegion());

        inIt.GoToBegin();
        outIt.GoToBegin();
        while(!inIt.IsAtEnd()||!outIt.IsAtEnd())
        {
          outIt.Set(inIt.Get());
          ++inIt;++outIt;
        }
      }
    }// end namespace


#endif
