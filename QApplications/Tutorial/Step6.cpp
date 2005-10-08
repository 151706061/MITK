#include "Step6.h"

#include "QmitkRenderWindow.h"
#include "QmitkSliceWidget.h"

#include "mitkDataTreeNodeFactory.h"
#include "mitkProperties.h"
#include "mitkRenderingManager.h"

#include "mitkGlobalInteraction.h"
#include "mitkPointSet.h"
#include "mitkPointSetInteractor.h"

#include <itkConfidenceConnectedImageFilter.h>
#include "mitkImageAccessByItk.h"
#include "mitkDataTreeHelper.h"

#include "mitkRenderingManager.h"

#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>

//##Documentation
//## @brief Start region-grower at interactively added points
Step6::Step6( int argc, char* argv[], QWidget *parent, const char *name )
  : QMainWindow ( parent, name ), m_FirstImage(NULL), m_ResultImage(NULL), m_ResultNode(NULL)
{
  // create the tree: this is now a member
  m_Tree=mitk::DataTree::New();

  // load data as in the previous steps; a reference to the first loaded
  // image is kept in the member m_FirstImage and used as input for the
  // region growing
  Load(argc, argv);
}

void Step6::Initialize()
{
  // setup the widgets as in the previous steps, but with an additional
  // QVBox for a button to start the segmentation
  SetupWidgets();

  // create button to start the segmentation and connect its clicked()
  // signal to method StartRegionGrowing
  QPushButton* startButton = new QPushButton("start region growing", m_TopLevelWidget, "start button");
  connect(startButton, SIGNAL(clicked()), this, SLOT(StartRegionGrowing()));
  if(m_FirstImage.IsNull()) startButton->setEnabled(false);

  // as in Step5, create PointSet (now as a member m_Seeds) and
  // associate a interactor to it
  mitk::DataTreePreOrderIterator it(m_Tree);
  m_Seeds = mitk::PointSet::New();
  mitk::DataTreeNode::Pointer pointSetNode = mitk::DataTreeNode::New();
  pointSetNode->SetData(m_Seeds);
  pointSetNode->SetProperty("layer", new mitk::IntProperty(2));
  it.Add(pointSetNode);
  mitk::GlobalInteraction::GetInstance()->AddInteractor(
    new mitk::PointSetInteractor("pointsetinteractor", pointSetNode)
  );
}

void Step6::StartRegionGrowing()
{
  AccessByItk_1(m_FirstImage, RegionGrowing, this);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void Step6::Load(int argc, char* argv[])
{
  //Part I: Basic initialization
  // create a tree
  m_Tree=mitk::DataTree::New();
  // create an iterator on the tree
  mitk::DataTreePreOrderIterator it(m_Tree);

  //Part II: Create some data by reading files
  unsigned int i;
  for(i=1; i<argc; ++i)
  {
    // create a DataTreeNodeFactory to read a data format supported
    // by the DataTreeNodeFactory (many image formats, surface formats, etc.)
    mitk::DataTreeNodeFactory::Pointer nodeReader=mitk::DataTreeNodeFactory::New();
    const char * filename = argv[i];
    try
    {
      nodeReader->SetFileName(filename);
      nodeReader->Update();
      //Part III: Put the data into the tree
      // Since the DataTreeNodeFactory directly creates a node,
      // use the iterator to add the read node to the tree
      mitk::DataTreeNode::Pointer node = nodeReader->GetOutput();
      it.Add(node);

      mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(node->GetData());
      if((m_FirstImage.IsNull()) && (image.IsNotNull()))
        m_FirstImage = image;
    }
    catch(...)
    {
      fprintf( stderr, "Could not open file %s \n\n", filename );
      exit(2);
    }
  }
}

void Step6::SetupWidgets()
{
  // create an iterator on the tree
  mitk::DataTreePreOrderIterator it(m_Tree);

  //Part I: Create windows and pass the tree to it
  // create toplevel widget with vertical layout
  m_TopLevelWidget = new QVBox(this);
  // create viewParent widget with horizontal layout
  QHBox* viewParent = new QHBox(m_TopLevelWidget);
  // Part Ia: 3D view
  // create a renderwindow
  QmitkRenderWindow* renderWindow = new QmitkRenderWindow(viewParent);
  // tell the renderwindow which (part of) the tree to render
  renderWindow->GetRenderer()->SetData(&it);
  // use it as a 3D view
  renderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

  // Part Ib: 2D view for slicing transversally
  // create QmitkSliceWidget, which is based on the class
  // QmitkRenderWindow, but additionally provides sliders
  QmitkSliceWidget *view2=new QmitkSliceWidget(viewParent);
  // tell the QmitkSliceWidget which (part of) the tree to render.
  // By default, it slices the data transversally
  view2->SetData(&it);
  // We want to see the position of the slice in 2D and the
  // slice itself in 3D: add it to the tree!
  it.Add(view2->GetRenderer()->GetCurrentWorldGeometry2DNode());

  // Part Ic: 2D view for slicing sagitally
  // create QmitkSliceWidget, which is based on the class
  // QmitkRenderWindow, but additionally provides sliders
  QmitkSliceWidget *view3=new QmitkSliceWidget(viewParent);
  // tell the QmitkSliceWidget which (part of) the tree to render
  // and to slice sagitally
  view3->SetData(&it, mitk::SliceNavigationController::Sagittal);
  // We want to see the position of the slice in 2D and the
  // slice itself in 3D: add it to the tree!
  it.Add(view3->GetRenderer()->GetCurrentWorldGeometry2DNode());

  // Part II: handle updates: To avoid unnecessary updates, we have to
  // define when to update. The RenderingManager serves this purpose, and
  // each RenderWindow has to be registered to it.
  mitk::RenderingManager *renderingManager =
    mitk::RenderingManager::GetInstance();
  renderingManager->AddRenderWindow( renderWindow );
  renderingManager->AddRenderWindow( view2->GetRenderWindow() );
  renderingManager->AddRenderWindow( view3->GetRenderWindow() );

  setCentralWidget(m_TopLevelWidget);
}

/**
\example Step6.cpp
*/
