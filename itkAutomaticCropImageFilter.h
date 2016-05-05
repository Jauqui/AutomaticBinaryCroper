#ifndef ITKAUTOMATICCROPIMAGEFILTER_H
    #define ITKAUTOMATICCROPIMAGEFILTER_H

    #include <mutex>          // std::mutex
    #include <queue>

    #include <itkImage.h>
    #include <itkImageRegionIterator.h>
    #include <itkImageRegionConstIterator.h>
    #include <itkImageToImageFilter.h>
    #include <itkImageRegionConstIterator.h>
    #include <itkImageRegionIterator.h>
    #include <itkImageSliceConstIteratorWithIndex.h>


    namespace itk {
        template <class TInImage, class TOutImage> class itkAutomaticCropImageFilter : public ImageToImageFilter <TInImage, TOutImage> {
            public:
                /** Standard class typedefs. */
                typedef itkAutomaticCropImageFilter             Self;
                typedef ImageToImageFilter <TInImage, TOutImage> Superclass;
                typedef SmartPointer <Self>        Pointer;

                typedef itk::Index<TInImage::ImageDimension> IndexType;
                typedef itk::Size<TInImage::ImageDimension> SizeType;
                typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

                typedef itk::ImageSliceConstIteratorWithIndex<TInImage> InConstIteratorType;


                /** Method for creation through the object factory. */
                itkNewMacro(Self);

                /** Run-time type information (and related methods). */
                itkTypeMacro(ImageFilter, ImageToImageFilter);

                /** Method for getting output image index */
                itkGetMacro(ImageIndex, IndexType);
                /** Method for getting output image size */
                itkGetMacro(ImageSize, SizeType);


            protected:
                itkAutomaticCropImageFilter(){}
                ~itkAutomaticCropImageFilter(){}
                void BeforeThreadedGenerateData();
                void AfterThreadedGenerateData();

                virtual void ThreadedGenerateData(const OutputImageRegionType &, ThreadIdType);

                typename TInImage::ConstPointer m_Input;


            private:
                itkAutomaticCropImageFilter(const Self &); //purposely not implemented
                void operator=(const Self &);  //purposely not implemented
                void FindFirstPixel(const OutputImageRegionType, unsigned int i);
                void FindLastPixel(const OutputImageRegionType, unsigned int i);

                std::mutex mtx;           // mutex for critical section

                IndexType m_ImageIndex;
                SizeType m_ImageSize;

                IndexType m_ImageFirstIndex;
                IndexType m_ImageLastIndex;

        };
    } //namespace ITK


    #ifndef ITK_MANUAL_INSTANTIATION
        #include "itkAutomaticCropImageFilter.cxx"
    #endif


#endif // ITKAUTOMATICCROPIMAGEFILTER_H*/
