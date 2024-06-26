/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkPlaneFit_h
#define mitkPlaneFit_h

#include "MitkAlgorithmsExtExports.h"
#include "mitkGeometryDataSource.h"
#include "mitkPlaneGeometry.h"
#include "mitkPointSet.h"
#include "mitkTimeGeometry.h"

namespace mitk
{
  //!
  //  kind regards to dr. math!
  // function [x0, a, d, normd] = lsplane(X)
  // ---------------------------------------------------------------------
  // LSPLANE.M   Least-squares plane (orthogonal distance
  //             regression).
  //
  // Version 1.0
  // Last amended   I M Smith 27 May 2002.
  // Created        I M Smith 08 Mar 2002
  // ---------------------------------------------------------------------
  // Input
  // X        Array [x y z] where x = vector of x-coordinates,
  //          y = vector of y-coordinates and z = vector of
  //          z-coordinates.
  //          Dimension: m x 3.
  //
  // Output
  // x0       Centroid of the data = point on the best-fit plane.
  //          Dimension: 3 x 1.
  //
  // a        Direction cosines of the normal to the best-fit
  //          plane.
  //          Dimension: 3 x 1.
  //
  // <Optional...
  // d        Residuals.
  //          Dimension: m x 1.
  //
  // normd    Norm of residual errors.
  //          Dimension: 1 x 1.
  // ...>
  //
  // [x0, a <, d, normd >] = lsplane(X)
  // ---------------------------------------------------------------------

  class MITKALGORITHMSEXT_EXPORT PlaneFit : public GeometryDataSource
  {
  public:
    mitkClassMacro(PlaneFit, GeometryDataSource);
    itkNewMacro(Self);

    typedef mitk::PointSet::PointDataType PointDataType;
    typedef mitk::PointSet::PointDataIterator PointDataIterator;

    void GenerateOutputInformation() override;

    void GenerateData() override;

    /*!Getter for point set.
     *
     */
    const mitk::PointSet *GetInput();

    /*! filter initialisation.
     *
     */
    using mitk::GeometryDataSource::SetInput;
    virtual void SetInput(const mitk::PointSet *ps);

    /*! returns the center of gravity of the point set.
     *
     */
    virtual const mitk::Point3D &GetCentroid(int t = 0) const;

    /*! returns the plane geometry which represents the point set.
     *
     */
    virtual mitk::PlaneGeometry::Pointer GetPlaneGeometry(int t = 0);

    /*! returns the normal of the plane which represents the point set.
     *
     */
    virtual const mitk::Vector3D &GetPlaneNormal(int t = 0) const;

  protected:
    PlaneFit();
    ~PlaneFit() override;

    /*! Calculates the centroid of the point set.
     * the center of gravity is calculated  through the mean value of the whole point set
     */
    void CalculateCentroid(int t = 0);

    /*! working with an SVD algorithm form matrix dataM.
     * ITK supplies the vnl_svd to solve an plan fit eigentvector problem
     * points are processed in the SVD matrix. The normal vector is the
     * singular vector of dataM corresponding to its smalest singular value.
     * The method uses VNL library from ITK and at least the method nullvector()
     * to extract the normalvector.
     */
    void ProcessPointSet(int t = 0);

    /*! Initialize Plane and configuration.
     *
     */
    void InitializePlane(int t = 0);

  private:
    /*!keeps a copy of the pointset.*/
    const mitk::PointSet *m_PointSet;

    /* output object - a time sliced geometry.*/
    mitk::TimeGeometry::Pointer m_TimeGeometry;

    std::vector<mitk::PlaneGeometry::Pointer> m_Planes;

    /*! the calculatet center point of all points in the point set.*/
    std::vector<mitk::Point3D> m_Centroids;

    /* the normal vector to describe a plane geometry.*/
    std::vector<mitk::Vector3D> m_PlaneVectors;
  };
} // namespace mitk
#endif
