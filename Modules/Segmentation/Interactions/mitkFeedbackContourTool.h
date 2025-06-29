/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkFeedbackContourTool_h
#define mitkFeedbackContourTool_h

#include "mitkCommon.h"
#include "mitkContourModelUtils.h"
#include "mitkContourUtils.h" //TODO remove legacy support
#include "mitkImage.h"
#include "mitkSegTool2D.h"
#include <MitkSegmentationExports.h>

#include "mitkDataNode.h"

#include "mitkImageCast.h"

namespace mitk
{
  /**
    \brief Base class for tools that use a contour for feedback

    \sa Tool
    \sa ContourModel

    \ingroup Interaction
    \ingroup ToolManagerEtAl

    Implements helper methods, that might be of use to all kind of 2D segmentation tools that use a contour for user
    feedback.
     - Providing a feedback contour that might be added or removed from the visible scene (SetFeedbackContourVisible).
     - Filling of a contour into a 2D slice

     These helper methods are actually implemented in ContourUtils now. FeedbackContourTool only forwards such requests.

    \warning Only to be instantiated by mitk::ToolManager.

    $Author: nolden $
  */
  class MITKSEGMENTATION_EXPORT FeedbackContourTool : public SegTool2D
  {
  public:
    mitkClassMacro(FeedbackContourTool, SegTool2D);

  protected:
    FeedbackContourTool();             // purposely hidden
    FeedbackContourTool(const char *); // purposely hidden
    ~FeedbackContourTool() override;

    const ContourModel *GetFeedbackContour() const;

    /** (Re)initialize the feedback contour by creating a new instance.
     * It is assured that the new instance as the same time geometry than
     * the working image.*/
    void InitializeFeedbackContour(bool isClosed);

    /** Clears the current time step of the feedback contour and resets its closed state.*/
    void ClearsCurrentFeedbackContour(bool isClosed);

    /** Updates the feedback contour of the currently selected time point. The update will be done
     * by clearing all existing vertices at the current time point and copying the vertics of the
     * source model at the specified source time step.*/
    void UpdateCurrentFeedbackContour(const ContourModel* sourceModel, TimeStepType sourceTimeStep = 0);
    /** Updates the feedback contour at the time step specified by feedbackTimeStep. The update will be done
     * by clearing all existing vertices at feedbackTimeStep and copying the vertics of the
     * source model at the specified source time step.*/
    void UpdateFeedbackContour(const ContourModel* sourceModel, TimeStepType feedbackTimeStep, TimeStepType sourceTimeStep = 0);

    /** Adds a vertex to the feedback contour for the current time point. */
    void AddVertexToCurrentFeedbackContour(const Point3D& point);

    /** Adds a vertex to the feedback contour for the passed time step. If time step is invalid, nothing will be added.*/
    void AddVertexToFeedbackContour(const Point3D& point, TimeStepType feedbackTimeStep);

    void SetFeedbackContourVisible(bool);

    /// Provide values from 0.0 (black) to 1.0 (full color)
    void SetFeedbackContourColor(float r, float g, float b);
    void SetFeedbackContourColor(const Color& color);
    void SetFeedbackContourColorDefault();
    void SetFeedbackContourWidth(float width);

    void Deactivated() override;

    void Activated() override;

    /**
      \brief Projects a contour onto an image point by point. Converts from world to index coordinates.

      \param slice
      \param contourIn3D
    */
    ContourModel::Pointer ProjectContourTo2DSlice(const Image *slice,
                                                  const ContourModel *contourIn3D);

    /**
      \brief Projects a slice index coordinates of a contour back into world coordinates.

      \param sliceGeometry
      \param contourIn2D
    */
    ContourModel::Pointer BackProjectContourFrom2DSlice(const BaseGeometry *sliceGeometry,
                                                        const ContourModel *contourIn2D);

    /** Helper methods that checks all precondition and if they are fulfilled does the following:
     * 1. Gets the contour of the time point specified by positionEvent.
     * 2. Gets the affected working slice of the time point specified by positionEvent.
     * 3. projects the contour onto the working slice and then fills it with the passed paintingPixelValue (adjusted by the current active label value)
     * to the slice.
     * 4. writes the slice back into the working image using SegTool2D::WriteBackSegmentationResult().
     * @param positionEvent The position event that indicates that triggers the update of the segmentation.
     * It i.a. encodes the slice position that should be updated.
     * @param labelValue The value of the label that should be updated.
     * @param setInvisibleAfterSuccess Indicates if the feedback contour should be set invisible after the update of the seg result.
     * @param addMode If false is passed the content of the contour will be set to background (thus label will be erased).
     * if set to true, the label will be added to the slice in the provided contour.*/
    void WriteBackFeedbackContourAsSegmentationResult(const InteractionPositionEvent* positionEvent, MultiLabelSegmentation::LabelValueType labelValue, bool addMode, bool setInvisibleAfterSuccess = true);

    /** Helper methods that generates an updated slice image.
     * @param seg Pointer to the segmentation that should be the template for the updated slice (slice will be extracted from the segmentation.
     * @param sliceGeometry Pointer to the slice geometry that defines the slice that should be extracted and updated.
     * @param contour Pointer to the contour that should be updated in the slice.
     * @param labelValue The value of the label that should be updated.
     * @param timePoint The time point that should be used for slice extraction and update.
     * @param addMode If false is passed the content of the contour will be set to background (thus label will be erased).
     * if set to true, the label will be added to the slice in the provided contour.*/
    mitk::Image::Pointer GenerateSliceWithContourUpdate(const MultiLabelSegmentation* seg, const PlaneGeometry* sliceGeometry,
      const ContourModel* contour, MultiLabelSegmentation::LabelValueType labelValue, TimePointType timePoint, bool addMode);

  private:
    ContourModel::Pointer m_FeedbackContour;
    DataNode::Pointer m_FeedbackContourNode;
    bool m_FeedbackContourVisible;
  };

} // namespace

#endif
