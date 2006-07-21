#ifndef MITKVTKLAYERCONTROLLER_H_HEADER_INCLUDED_C1EDO02D
#define MITKVTKLAYERCONTROLLER_H_HEADER_INCLUDED_C1EDO02D

#include <vector>

class vtkRenderWindow;
class vtkRenderer;

namespace mitk 
{

/**
 * Manages the VTK layer hierarchy
 * of a vtkRenderWindow.
 * For simple access the layers are divided into three
 * main groups: background, scene and foreground layers.
 * Renderers can be registered via the insert... functions and 
 * removed via the RemoveRenderer function.
 */
class VtkLayerController
{
	public:
    VtkLayerController(vtkRenderWindow* renderWindow);
    ~VtkLayerController();
    
    /**
    * Connects a VTK renderer with a vtk renderwindow. The renderer will be rendered in the background.
    * With forceAbsoluteBackground set true a renderer can be placed at the absolute background of the scene.
    * Multiple calls with forceAbsoluteBackground set true will set the latest registered renderer as background.
    */
    void InsertBackgroundRenderer(vtkRenderer* renderer, bool forceAbsoluteBackground);
    
    /**
    * Connects a VTK renderer with a vtk renderwindow. The renderer will be rendered in the foreground.
    * With forceAbsoluteBackground set true a renderer can be placed at the absolute foreground of the scene.
    * Multiple calls with forceAbsoluteForeground set true will set the latest registered renderer as foreground.
    */
    void InsertForegroundRenderer(vtkRenderer* renderer, bool forceAbsoluteForeground);
    
    /**
    * Connects a VTK renderer with a vtk renderwindow. The renderer will be rendered between background renderers and
    * foreground renderers.
    */
    void InsertSceneRenderer(vtkRenderer* renderer);
    
    /**
    * Connects a VtkRenderWindow with the layer controller.
    */
    void SetRenderWindow(vtkRenderWindow* renwin);
    
    /**
    * A renderer which has been inserted via a insert... function can be removed from the vtkRenderWindow with
    * RemoveRenderer.
    */
    void RemoveRenderer(vtkRenderer* renderer);

	protected:
    vtkRenderWindow*    m_RenderWindow;
       	
	private:

    /**
    * Internally used to sort all registered renderers and to connect the with the vtkRenderWindow.
    * Mention that VTK Version 5 and above is rendering higher numbers in the background and VTK
    * Verison < 5 in the foreground.
    */
    void UpdateLayers();

    // Layer Management
    typedef std::vector<vtkRenderer*> RendererVectorType;
    RendererVectorType m_BackgroundRenderers;
    RendererVectorType m_SceneRenderers;
    RendererVectorType m_ForegroundRenderers;

		
};

} // Namespace MITK




#endif /* MITKVTKLAYERCONTROLLER_H_HEADER_INCLUDED_C1EDO02D */