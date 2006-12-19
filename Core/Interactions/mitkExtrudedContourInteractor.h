#ifndef __ExtrudedContourInteractor_H
#define __ExtrudedContourInteractor_H

#include "mitkCommon.h"
#include <mitkInteractor.h>
#include <mitkVector.h>
#include <mitkContour.h>
#include <mitkDataTreeNode.h>

namespace mitk {

  //##Documentation
  //## @brief Interactor for the creation of an mitk::Contour
  //## @ingroup Interaction
  class ExtrudedContourInteractor : public mitk::Interactor
  {
  public:
    mitkClassMacro(ExtrudedContourInteractor, Interactor);
    mitkNewMacro2Param(Self, const char*, DataTreeNode*);

    itkGetObjectMacro(Contour, mitk::Contour);
    itkGetObjectMacro(ContourNode, mitk::DataTreeNode);

  protected:
    ExtrudedContourInteractor(const char * type, DataTreeNode* dataTreeNode);
    virtual ~ExtrudedContourInteractor();

    virtual bool ExecuteAction(Action* action, mitk::StateEvent const* stateEvent);

    /**
    * entry method for any interaction. Method is called if user 
    * presses the left mouse button down.
    */
    virtual void Press (mitk::Point3D& op);

    /**
    * this method is finally called after user release the left mouse button
    */
    virtual void Release (mitk::Point3D& op);

    /**
    * method is called when the user moves the mouse with left mouse button down
    */
    virtual void Move (mitk::Point3D& op);

  protected:
    bool m_Positive;
    bool m_Started;

    mitk::Contour::Pointer m_Contour;
    mitk::DataTreeNode::Pointer m_ContourNode;
  };

}
#endif //__ExtrudedContourInteractor_H
