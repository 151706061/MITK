#ifndef _MITK_IMAGE_TO_LOOKUP_TABLE_FILTER__H_
#define _MITK_IMAGE_TO_LOOKUP_TABLE_FILTER__H_

#include <mitkLookupTableSource.h>
#include <mitkImage.h>

namespace mitk
{

/**
* Creates a mitk::LookupTable from an image. This may be used to 
* create a default coloring scheme for vector images...
*/
class ImageToLookupTableFilter : public LookupTableSource
{
public:

  mitkClassMacro( ImageToLookupTableFilter, LookupTableSource );    

  itkNewMacro( Self );

  // typedefs for setting an image as input
  typedef mitk::Image                    InputImageType;
  typedef InputImageType::Pointer        InputImagePointer;
  typedef InputImageType::ConstPointer   InputImageConstPointer;

  virtual void SetInput( const InputImageType *image);

  virtual void SetInput( unsigned int, const InputImageType * image);

  const InputImageType * GetInput(void);

  const InputImageType * GetInput(unsigned int idx);

protected:   

  virtual void GenerateData();

  ImageToLookupTableFilter();

  virtual ~ImageToLookupTableFilter();

};


} //end of namespace mitk

#endif
