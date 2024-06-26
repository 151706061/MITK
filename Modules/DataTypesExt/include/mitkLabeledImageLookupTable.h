/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkLabeledImageLookupTable_h
#define mitkLabeledImageLookupTable_h

#include "MitkDataTypesExtExports.h"
#include "mitkLevelWindow.h"
#include "mitkLookupTable.h"
#include <iostream>
#include <string>

namespace mitk
{
  /**
   * A lookup table for 2D mapping of labeled images. The lookup table supports
   * images with up to 256 unsigned labels. Negative labels are not supported.
   * Please use the level/window settings as given by the GetLevelWindow() method
   * to make sure, that the colors are rendered correctly.
   * The colors are initialized with random colors as default. As background
   * the label 0 is assumed. The color for the background is set to fully transparent
   * as default.
   */
  class MITKDATATYPESEXT_EXPORT LabeledImageLookupTable : public LookupTable
  {
  public:
    /**
     * Standard mitk typedefs are generated by the mitkClassMacro
     */
    mitkClassMacro(LabeledImageLookupTable, LookupTable);

    /**
     * Make this object constructable by the ::%New() Method.
     */
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

      /**
       * The data type for a label. Currently only images with labels
       * in the range [0,255] are supported.
       */
      typedef unsigned char LabelType;

    LabeledImageLookupTable &operator=(const LookupTable &other) override;

    /**
     * Sets the color for a given label
     * @param label The pixel value used as a label in the image
     * @param r The red component of the rgba color value. Values should be given in the range [0,1]
     * @param g The green component of the rgba color value. Values should be given in the range [0,1]
     * @param b The blue component of the rgba color value. Values should be given in the range [0,1]
     * @param a The alpha component of the rgba color value. Values should be given in the range [0,1]. Default is 1.
     */
    virtual void SetColorForLabel(
      const LabelType &label, const double &r, const double &g, const double &b, const double a = 1.0);

    /**
     * Determines the color which will be used for coloring a given label.
     * @param label the label for which the color should be returned
     * @returns an rgba array containing the color information for the given label.
     *          Color components are expressed as [0,1] double values.
     */
    virtual double *GetColorForLabel(const LabelType &label);

    /**
     * Provides access to level window settings, which should be used
     * in combination with the LUTs generated by this filter (at lease for
     * 2D visualization. If you use other level/window settings, it is not
     * guaranteed, that scalar values are mapped to the correct colors.
     */
    mitk::LevelWindow GetLevelWindow() { return m_LevelWindow; }
  protected:
    /**
     * Default constructor. Protected to prevent "normal" creation
     */
    LabeledImageLookupTable();

    LabeledImageLookupTable(const LabeledImageLookupTable &other);

    /**
     * Virtual destructor
     */
    ~LabeledImageLookupTable() override;

    /**
     * Generates a random rgb color value. Values for rgb are in the range
     * [0,1]
     */
    virtual void GenerateRandomColor(double &r, double &g, double &b);
    /**
     * Generates a radnom number drawn from a uniform
     * distribution in the range [0,1].
     */
    virtual double GenerateRandomNumber();

    mitk::LevelWindow m_LevelWindow;

  private:
    itk::LightObject::Pointer InternalClone() const override;
  };
}

#endif
