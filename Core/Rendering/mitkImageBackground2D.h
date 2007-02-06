#ifndef _mitk_Image_Background2D_h_
#define _mitk_Image_Background2D_h_

#include "mitkVideoSource.h"

#include "mitkConfig.h"

class vtkRenderer;
class vtkRenderWindow;
class vtkImageActor;
class vtkImageImport;
class vtkVideoSizeCallback;

namespace mitk 
{

  class RenderWindow;

  /**
  * Displays a image (currently unsigned char) in the background
  * of a vtkRenderWindow.
  * Caution: As the data is not being copied, a user is responsible for a valid 
  * pointer to the image. Also the image dimensions needs to be set correctly before enabling the 
  * background.
  */
  class ImageBackground2D : public itk::Object
  {
    public:
     mitkClassMacro( ImageBackground2D, itk::Object );
     itkNewMacro( Self );

    ////##Documentation
    ////## @brief returns the mitkRenderWindow in which the video is displayed.
    mitk::RenderWindow* GetRenderWindow();
    ////##Documentation
    ////## @brief sets the mitkRenderWindow in which the video is displayed.
    ////## must be initialized before enabling the background.
    void SetRenderWindow(const mitk::RenderWindow* renderWindow);
    ////##Documentation
    ////## @brief sets the width,height and number of scalar components of the image.
    ////## must be correctly initialized before enabling the background.
    void SetImageDimensions(int x, int y, int nrOfScalarComponents)
    {
      m_ImageWidth  = x;
      m_ImageHeight = y;
      m_ImageScalarComponents = nrOfScalarComponents;
      InitVtkImageImport();
    }

    void SetParallelScale(int scale);
    int  GetParallelScale();
    
    void Update(char * dataPointer);
    void Update(char * dataPointer, int imageScalarComponents, int height, int width);
    
    ////##Documentation
    ////## @brief visualizes the video. Requires image dimensions and an active mitkvideosource to be set.
    void Enable();
    ////##Documentation
    ////## @brief disables visualization of the video.
    void Disable();
    ////##Documentation
    ////## @brief Checks, if the Video background is currently enabled (visible).
    bool IsEnabled();
     
  protected:
    ImageBackground2D();
    ~ImageBackground2D();

    void InitVtkImageImport();

    const mitk::RenderWindow*   m_RenderWindow;
    vtkRenderer*                m_ImageRenderer;
    vtkImageActor*              m_Actor;
    vtkImageImport*             m_VtkImageImport;

     // Adjust Image Size
     vtkVideoSizeCallback *     m_SizeCallback;

    unsigned char *             m_ImageData;
     
    int m_ImageWidth, m_ImageHeight, m_ImageScalarComponents, m_ParallelScale;
  };
} // namespace mitk

#endif

