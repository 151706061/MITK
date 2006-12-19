#ifndef __ContourInteractor_H
#define __ContourInteractor_H

#include "mitkCommon.h"
#include <mitkInteractor.h>
#include <mitkVector.h>

namespace mitk {

  //##Documentation
  //## @brief Interactor for the creation of an mitk::Contour
  //## @ingroup Interaction
  class ContourInteractor : public mitk::Interactor
  {
  public:
    mitkClassMacro(ContourInteractor, Interactor);
    mitkNewMacro2Param(Self, const char*, DataTreeNode*);


  protected:
    ContourInteractor(const char * type, DataTreeNode* dataTreeNode);
    virtual ~ContourInteractor();

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
  };

}
#endif //__ContourInteractor_H
