#ifndef IMAGESOURCE_H_HEADER_INCLUDED_C1E7D6EC
#define IMAGESOURCE_H_HEADER_INCLUDED_C1E7D6EC

#include "mitkCommon.h"
#include "BaseProcess.h"
#include "mitkImage.h"

namespace mitk {

//##ModelId=3E1878F90199
//##Documentation
//## Superclass of all classes generating Images as output. 
//## 
//## In itk and vtk the generated result of a ProcessObject is only guaranteed
//## to be up-to-date, when Update() of the ProcessObject or the generated
//## DataObject is called immediately before access of the data stored in the
//## DataObject. This is also true for subclasses of mitk::BaseProcess and thus
//## for mitk::ImageSource. But there are also three access methods provided
//## that guarantee an up-to-date result (by first calling Update and then
//## returning the result of GetOutput()): GetData(), GetPic() and
//## GetVtkImageData().
class ImageSource : public BaseProcess
{
public:
  /** Standard class typedefs. */
    //##ModelId=3E1886F0037F
  typedef ImageSource         Self;
    //##ModelId=3E1886F003B2
  typedef BaseProcess         Superclass;
    //##ModelId=3E1886F003C6
  typedef itk::SmartPointer<Self>  Pointer;
    //##ModelId=3E1886F003E4
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Smart Pointer type to a DataObject. */
    //##ModelId=3E1886F1001A
  typedef itk::DataObject::Pointer DataObjectPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageSource,BaseProcess);

  /** Some convenient typedefs. */
    //##ModelId=3E1886F10038
  typedef mitk::Image OutputImageType;
    //##ModelId=3E1886F10056
  typedef OutputImageType::Pointer OutputImagePointer;
    //##ModelId=3E1886F1006A
  typedef SlicedData::RegionType OutputImageRegionType;

  /** Get the image output of this process object.  */
    //##ModelId=3E1886F101A2
  OutputImageType * GetOutput(void);
    //##ModelId=3E1886F101F0
  OutputImageType * GetOutput(unsigned int idx);
  
  /** Set the image output of this process object. This call is slated
   * to be removed from ITK. You should GraftOutput() and possible
   * DataObject::DisconnectPipeline() to properly change the output. */
    //##ModelId=3E1886F1024A
  void SetOutput(OutputImageType *output);

  /** Graft the specified DataObject onto this ProcessObject's output.
   * This method grabs a handle to the specified DataObject's bulk
   * data to used as its output's own bulk data. It also copies the
   * region ivars (RequestedRegion, BufferedRegion,
   * LargestPossibleRegion) and meta-data (Spacing, Origin) from the
   * specified data object into this filter's output data object. Most
   * importantly, however, it leaves the Source ivar untouched so the
   * original pipeline routing is intact. This method is used when a
   * process object is implemented using a mini-pipeline which is
   * defined in its GenerateData() method.  The usage is:
   *
   * \code
   *    // setup the mini-pipeline to process the input to this filter
   *    firstFilterInMiniPipeline->SetInput( this->GetInput() );
   
   *    // setup the mini-pipeline to calculate the correct regions
   *    // and write to the appropriate bulk data block
   *    lastFilterInMiniPipeline->GraftOutput( this->GetOutput() );
   *
   *    // execute the mini-pipeline
   *    lastFilterInMiniPipeline->Update();
   *
   *    // graft the mini-pipeline output back onto this filter's output.
   *    // this is needed to get the appropriate regions passed back.
   *    this->GraftOutput( lastFilterInMiniPipeline->GetOutput() );
   * \endcode
   *
   * For proper pipeline execution, a filter using a mini-pipeline
   * must implement the GenerateInputRequestedRegion(),
   * GenerateOutputRequestedRegion(), GenerateOutputInformation() and
   * EnlargeOutputRequestedRegion() methods as necessary to reflect
   * how the mini-pipeline will execute (in other words, the outer
   * filter's pipeline mechanism must be consistent with what the
   * mini-pipeline will do).
   *  */
    //##ModelId=3E1886F102A5
  virtual void GraftOutput(OutputImageType *output);

  /** Graft the specified data object onto this ProcessObject's idx'th
   * output. This is the similar to GraftOutput method except is
   * allows you specify which output is affected. The specified index
   * must be a valid output number (less than
   * ProcessObject::GetNumberOfOutputs()). See the GraftOutput for
   * general usage information. */
    //##ModelId=3E1886F10313
  virtual void GraftNthOutput(unsigned int idx, OutputImageType *output);

  /** Make a DataObject of the correct type to used as the specified
   * output.  Every ProcessObject subclass must be able to create a
   * DataObject that can be used as a specified output. This method
   * is automatically called when DataObject::DisconnectPipeline() is
   * called.  DataObject::DisconnectPipeline, disconnects a data object
   * from being an output of its current source.  When the data object
   * is disconnected, the ProcessObject needs to construct a replacement
   * output data object so that the ProcessObject is in a valid state.
   * So DataObject::DisconnectPipeline eventually calls
   * ProcessObject::MakeOutput. Note that MakeOutput always returns a
   * SmartPointer to a DataObject. If a subclass of ImageSource has
   * multiple outputs of different types, then that class must provide
   * an implementation of MakeOutput(). */
    //##ModelId=3E1886F103DB
  virtual DataObjectPointer MakeOutput(unsigned int idx);
    //##ModelId=3E3BCBD4000C
    virtual void* GetData();

    //##ModelId=3E3BCBD0024B
    virtual ipPicDescriptor* GetPic();

    //##ModelId=3E3BCBD502FD
    virtual vtkImageData* GetVtkImageData();

protected:
    //##ModelId=3E1886F20075
  ImageSource();
    //##ModelId=3E1886F20093
  virtual ~ImageSource() {}
  
  /** A version of GenerateData() specific for image processing
   * filters.  This implementation will split the processing across
   * multiple threads. The buffer is allocated by this method. Then
   * the BeforeThreadedGenerateData() method is called (if
   * provided). Then, a series of threads are spawned each calling
   * ThreadedGenerateData(). After all the threads have completed
   * processing, the AfterThreadedGenerateData() method is called (if
   * provided). If an image processing filter cannot be threaded, the
   * filter should provide an implementation of GenerateData(). That
   * implementation is responsible for allocating the output buffer.
   * If a filter an be threaded, it should NOT provide a
   * GenerateData() method but should provide a ThreadedGenerateData()
   * instead.
   *
   * \sa ThreadedGenerateData() */
    //##ModelId=3E1886F200CF
  virtual void GenerateData();

  /** If an imaging filter can be implemented as a multithreaded
   * algorithm, the filter will provide an implementation of
   * ThreadedGenerateData().  This superclass will automatically split
   * the output image into a number of pieces, spawn multiple threads,
   * and call ThreadedGenerateData() in each thread. Prior to spawning
   * threads, the BeforeThreadedGenerateData() method is called. After
   * all the threads have completed, the AfterThreadedGenerateData()
   * method is called. If an image processing filter cannot support
   * threading, that filter should provide an implementation of the
   * GenerateData() method instead of providing an implementation of
   * ThreadedGenerateData().  If a filter provides a GenerateData()
   * method as its implementation, then the filter is responsible for
   * allocating the output data.  If a filter provides a
   * ThreadedGenerateData() method as its implementation, then the
   * output memory will allocated automatically by this superclass.
   * The ThreadedGenerateData() method should only produce the output
   * specified by "outputThreadRegion"
   * parameter. ThreadedGenerateData() cannot write to any other
   * portion of the output image (as this is responsibility of a
   * different thread).
   *
   * \sa GenerateData(), SplitRequestedRegion() */
    //##ModelId=3E1886F2010B
  virtual
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );


  /** This method is intentionally left blank. ImageSource's need not
   * Initialize their containers. The Image::Allocate() method (called
   * from GenerateData()) will resize the container if more memory is
   * needed.  Otherwise, the memory can be reused.
   */
    //##ModelId=3E1886F201E8
  virtual void PrepareOutputs() {};

  /** The GenerateData method normally allocates the buffers for all of the
   * outputs of a filter. Some filters may want to override this default
   * behavior. For example, a filter may have multiple outputs with
   * varying resolution. Or a filter may want to process data in place by
   * grafting its input to its output.*/
    //##ModelId=3E1886F2022E
  virtual void AllocateOutputs();
  
  /** If an imaging filter needs to perform processing after the buffer
   * has been allocated but before threads are spawned, the filter can
   * can provide an implementation for BeforeThreadedGenerateData(). The
   * execution flow in the default GenerateData() method will be:
   *      1) Allocate the output buffer
   *      2) Call BeforeThreadedGenerateData()
   *      3) Spawn threads, calling ThreadedGenerateData() in each thread.
   *      4) Call AfterThreadedGenerateData()
   * Note that this flow of control is only available if a filter provides
   * a ThreadedGenerateData() method and NOT a GenerateData() method. */
    //##ModelId=3E1886F2026A
  virtual void BeforeThreadedGenerateData() {};
  
  /** If an imaging filter needs to perform processing after all
   * processing threads have completed, the filter can can provide an
   * implementation for AfterThreadedGenerateData(). The execution
   * flow in the default GenerateData() method will be:
   *      1) Allocate the output buffer
   *      2) Call BeforeThreadedGenerateData()
   *      3) Spawn threads, calling ThreadedGenerateData() in each thread.
   *      4) Call AfterThreadedGenerateData()
   * Note that this flow of control is only available if a filter provides
   * a ThreadedGenerateData() method and NOT a GenerateData() method. */
    //##ModelId=3E1886F202B0
  virtual void AfterThreadedGenerateData() {};
  
  /** Split the output's RequestedRegion into "num" pieces, returning
   * region "i" as "splitRegion". This method is called "num" times. The
   * regions must not overlap. The method returns the number of pieces that
   * the routine is capable of splitting the output RequestedRegion,
   * i.e. return value is less than or equal to "num". */
    //##ModelId=3E1886F202F6
  virtual
  int SplitRequestedRegion(int i, int num, OutputImageRegionType& splitRegion);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
    //##ModelId=3E1886F30063
  static ITK_THREAD_RETURN_TYPE ThreaderCallback( void *arg );

  /** Internal structure used for passing image data into the threading library */
    //##ModelId=3E1886F100A6
  struct ThreadStruct
  {
        //##ModelId=3E1886F10197
   Pointer Filter;
  };
  
private:
    //##ModelId=3E3BCCCD030D
  void operator=(const Self&); //purposely not implemented
};

} // namespace mitk

#endif /* IMAGESOURCE_H_HEADER_INCLUDED_C1E7D6EC */
