/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef __MITK_EXPONENTIAL_SATURATION_MODEL_H_
#define __MITK_EXPONENTIAL_SATURATION_MODEL_H_

#include "mitkModelBase.h"

#include "MitkModelFitExports.h"

namespace mitk
{

  /** @class ExponentialSaturationModel
  * @brief This genric model has the form: if x<BAT f(x) = S0 , else f(x) = A * (1-exp(-*B(x-BAT)))
  */
  class MITKMODELFIT_EXPORT ExponentialSaturationModel : public mitk::ModelBase
  {

  public:
    typedef ExponentialSaturationModel Self;
    typedef mitk::ModelBase Superclass;
    typedef itk::SmartPointer< Self >                            Pointer;
    typedef itk::SmartPointer< const Self >                      ConstPointer;

    typedef Superclass::ParameterNameType  ParameterNameType;
    typedef Superclass::ParametersSizeType ParametersSizeType;

    /** Method for creation through the object factory. */
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ExponentialSaturationModel, ModelBase);

    std::string GetModelDisplayName() const override;

    std::string GetModelType() const override;

    FunctionStringType GetFunctionString() const override;

    std::string GetXName() const override;

    ParameterNamesType GetParameterNames() const override;

    ParametersSizeType  GetNumberOfParameters() const override;

    ParameterNamesType GetStaticParameterNames() const override;

    ParametersSizeType GetNumberOfStaticParameters() const override;

  protected:
    ExponentialSaturationModel() {};
    ~ExponentialSaturationModel() override {};

    /**
     * Actual implementation of the clone method. This method should be reimplemeted
     * in subclasses to clone the extra required parameters.
     */
    itk::LightObject::Pointer InternalClone() const override;

    ModelResultType ComputeModelfunction(const ParametersType& parameters) const override;

    void SetStaticParameter(const ParameterNameType& name,
                                    const StaticParameterValuesType& values) override;
    StaticParameterValuesType GetStaticParameterValue(const ParameterNameType& name) const override;

  private:

    //No copy constructor allowed
    ExponentialSaturationModel(const Self& source);
    void operator=(const Self&);  //purposely not implemented

  };
}

#endif