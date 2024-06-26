/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef QmitkImageAndRoiDataGeneratorBase_h
#define QmitkImageAndRoiDataGeneratorBase_h

#include "QmitkDataGeneratorBase.h"
#include <MitkImageStatisticsUIExports.h>

/*!
Base class that can be used for generators that should allow the image nodes and the ROI nodes as vectors (like generated by node selection widgets).
This class ensures that data for every combination of images and ROIs (basically a folding) will be processed.
@sa QmitkDataGeneratorBase
*/
class MITKIMAGESTATISTICSUI_EXPORT QmitkImageAndRoiDataGeneratorBase : public QmitkDataGeneratorBase
{
public:
  using Superclass = QmitkDataGeneratorBase;

  using ConstNodeVectorType = std::vector<mitk::DataNode::ConstPointer>;
  using NodeVectorType = std::vector<mitk::DataNode::Pointer>;

  ConstNodeVectorType GetImageNodes() const;
  ConstNodeVectorType GetROINodes() const;


public slots:
    /*! @brief Setter for image nodes
    */
    void SetImageNodes(const ConstNodeVectorType& imageNodes);
    /*! Convenience overload*/
    void SetImageNodes(const NodeVectorType& imageNodes);

    /*! @brief Setter for roi nodes
    */
    void SetROINodes(const ConstNodeVectorType& roiNodes);
    /*! Convenience overload*/
    void SetROINodes(const NodeVectorType& roiNodes);

protected:
  QmitkImageAndRoiDataGeneratorBase(mitk::DataStorage::Pointer storage, QObject* parent = nullptr) : QmitkDataGeneratorBase(storage, parent) {};
  QmitkImageAndRoiDataGeneratorBase(QObject* parent = nullptr) : QmitkDataGeneratorBase(parent) {};

  using InputPairVectorType = Superclass::InputPairVectorType;
  
  bool ChangedNodeIsRelevant(const mitk::DataNode *changedNode) const override;
  InputPairVectorType GetAllImageROICombinations() const override;

  ConstNodeVectorType m_ImageNodes;
  ConstNodeVectorType m_ROINodes;

  QmitkImageAndRoiDataGeneratorBase(const QmitkImageAndRoiDataGeneratorBase&) = delete;
  QmitkImageAndRoiDataGeneratorBase& operator = (const QmitkImageAndRoiDataGeneratorBase&) = delete;
};

#endif
