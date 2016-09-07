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


#ifndef MITKHummelProtocolEvaluation_H_HEADER_INCLUDED_
#define MITKHummelProtocolEvaluation_H_HEADER_INCLUDED_

#include <mitkPointSet.h>
#include <itkMatrix.h>


namespace mitk
{

  /**Documentation
  * \brief Static methods for evaluations according to the assessment protocol
  * for EM trackers published by Hummel et al. 2005 [1].
  *
  * [1] Hummel, J. et al. - Design and application of an assessment protocol for electromagnetic tracking systems. Med Phys 32(7), July 2005
  *
  * \ingroup IGT
  */
  class HummelProtocolEvaluation

  {
  public:
    /** Distance error with description. */
    struct HummelProtocolDistanceError {double distanceError; std::string description;};
    /** Tracking volumes for evaluation.
     *  standard: The standard volume of 9 x 10 measurment points as described in [1]
     *  small: A small volume in the center 3 x 4 measurement points, for smaller field generators [2]
     *  [2] Maier-Hein, L. et al. - Standardized assessment of new electromagnetic field generators in an interventional radiology setting. Med Phys 39(6), June 2012
     */
    enum HummelProtocolMeasurementVolume { small, standard };
    /** Evaluates the 5 cm distances as defined by the Hummel protocol [1,2].
     * @return Returns true if evaluation was successfull, false if not.
     * @param[out] Results Please give an empty vector. The results will be added to this vector.
     */
    static bool Evaluate5cmDistances(mitk::PointSet::Pointer p, HummelProtocolMeasurementVolume m, std::vector<HummelProtocolDistanceError> &Results);

    /** Evaluates the 15 cm distances as defined by the Hummel protocol [1,2].
    * @return Returns true if evaluation was successfull, false if not.
    * @param[out] Results Please give an empty vector. The results will be added to this vector.
    */
    static bool Evaluate15cmDistances(mitk::PointSet::Pointer p, HummelProtocolMeasurementVolume m, std::vector<HummelProtocolDistanceError> &Results);

    /** Evaluates the 30 cm distances as defined by the Hummel protocol [1,2].
    * @return Returns true if evaluation was successfull, false if not.
    * @param[out] Results Please give an empty vector. The results will be added to this vector.
    */
    static bool Evaluate30cmDistances(mitk::PointSet::Pointer p, HummelProtocolMeasurementVolume m, std::vector<HummelProtocolDistanceError> &Results);

    /** Evaluates the accumulated distances as defined by the Hummel protocol [1,2].
    * @return Returns true if evaluation was successfull, false if not.
    * @param[out] Results Please give an empty vector. The results will be added to this vector.
    */
    static bool EvaluateAccumulatedDistances(mitk::PointSet::Pointer p, HummelProtocolMeasurementVolume m, std::vector<HummelProtocolDistanceError> &Results);

  protected:
    /** Converts a pointset holding all measurement points of the hummel protocol in line-by-line order
     *  to an array representing the hummel board.
     */
    static itk::Matrix<itk::Point<double, 3>, 9, 10> ParseMatrixStandardVolume(mitk::PointSet::Pointer p);

  };
} // namespace mitk

#endif /* MITKHummelProtocolEvaluation_H_HEADER_INCLUDED_ */
