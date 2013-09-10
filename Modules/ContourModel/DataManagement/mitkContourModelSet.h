/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#ifndef _mitkContourModelSet_H_
#define _mitkContourModelSet_H_

#include "mitkCommon.h"
#include "ContourModelExports.h"

#include "mitkContourModel.h"

#include <deque>

namespace mitk
{

  /** \brief
  */
  class ContourModel_EXPORT ContourModelSet : public mitk::BaseData
  {

  public:

    mitkClassMacro(ContourModelSet, mitk::BaseData);

    itkNewMacro(Self);

    mitkCloneMacro(Self);

    typedef std::deque<mitk::ContourModel::Pointer> ContourModelListType;
    typedef ContourModelListType::iterator ContourModelSetIterator;

    //  start of inline methods

    /** \brief Return an iterator a the front.
    */
    virtual ContourModelSetIterator Begin()
    {
      return this->m_Contours.begin();
    }

    /** \brief Return an iterator a the front.
    */
    virtual ContourModelSetIterator End()
    {
      return this->m_Contours.end();
    }

    /** \brief Returns the number of contained contours.
    */
    virtual int GetSize()
    {
      return this->m_Contours.size();
    }
    //   end of inline methods

    /** \brief Add a ContourModel to the container.
    */
    virtual void AddContourModel(mitk::ContourModel &contourModel);

    /** \brief Add a ContourModel to the container.
    */
    virtual void AddContourModel(mitk::ContourModel::Pointer contourModel);

    /** \brief Returns the ContourModel a given index
    \param index
    */
    virtual mitk::ContourModel* GetContourModelAt(int index);

    /** \brief Returns the container of the contours.
    */
    ContourModelListType* GetContourModelList();

    /** \brief Returns a bool whether the container is empty or not.
    */
    bool IsEmpty();

    /** \brief Remove the given ContourModel from the container if exists.
    \param ContourModel - the ContourModel to be removed.
    */
    virtual bool RemoveContourModel(mitk::ContourModel* contourModel);

    /** \brief Remove a ContourModel at given index within the container if exists.
    \param index - the index where the ContourModel should be removed.
    */
    virtual bool RemoveContourModelAt(int index);

    /** \brief Clear the storage container.
    */
    virtual void Clear();

    //////////////// inherit  from mitk::BaseData ////////////////////

    /*                  NO support for regions !                    */

    void SetRequestedRegionToLargestPossibleRegion(){}

    bool RequestedRegionIsOutsideOfTheBufferedRegion(){return false;}

    bool VerifyRequestedRegion(){return true;}

    void SetRequestedRegion(const itk::DataObject* ){}

    //////////////// END inherit  from mitk::BaseData ////////////////////


  protected:

    ContourModelSet();
    ContourModelSet(const mitk::ContourModelSet &other);
    virtual ~ContourModelSet();

    ContourModelListType m_Contours;

  };
} // namespace mitk

#endif // _mitkContourModelSet_H_
