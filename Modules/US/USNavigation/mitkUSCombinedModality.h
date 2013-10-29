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

#ifndef MITKUSCombinedModality_H_HEADER_INCLUDED_
#define MITKUSCombinedModality_H_HEADER_INCLUDED_

#include <MitkUSNavigationExports.h>
#include <mitkNavigationDataSmoothingFilter.h>
#include <mitkNavigationDataDelayFilter.h>

#include "mitkUSDevice.h"

namespace itk {
  template<class T> class SmartPointer;
}

namespace mitk {
  class NavigationDataSource;

  class MitkUSNavigation_EXPORT USCombinedModality : public USDevice
  {
  public:
    static const std::string DeviceClassIdentifier;

    mitkClassMacro(USCombinedModality, USDevice);
    mitkNewMacro4Param(USCombinedModality, USDevice::Pointer, NavigationDataSource::Pointer, std::string, std::string);

    itkGetMacro(UltrasoundDevice, itk::SmartPointer<USDevice>);
    //itkGetMacro(TrackingDevice, itk::SmartPointer<NavigationDataSource>);

    itkSetMacro(UltrasoundDevice, itk::SmartPointer<USDevice>);
    itkSetMacro(TrackingDevice, itk::SmartPointer<NavigationDataSource>);

    /**
    * \brief Getter for calibration data of the currently active probe and depth.
    *
    * \return Transformation for calibration or null if no calibration is available.
    */
    AffineTransform3D::Pointer GetCalibration();

    /**
    * \brief Sets a transformation as calibration data.
    * Calibration data is set for the currently activated probe and their current
    * zoom factor. It also marks the device as calibrated.
    */
    void SetCalibration(AffineTransform3D::Pointer calibration);

    /**
    * \brief Returns the Class of the Device.
    */
    virtual std::string GetDeviceClass();

    /**
    * \brief Wrapper for returning USImageSource of the UltrasoundDevice.
    */
    virtual USImageSource::Pointer GetUSImageSource();

    /**
    * \brief Wrapper for returning B mode control interface of the UltrasoundDevice.
    */
    virtual USControlInterfaceBMode::Pointer GetControlInterfaceBMode();

    /**
    * \brief Wrapper for returning probes control interface of the UltrasoundDevice.
    */
    virtual USControlInterfaceProbes::Pointer GetControlInterfaceProbes();

    /**
    * \brief Wrapper for returning doppler control interface of the UltrasoundDevice.
    */
    virtual USControlInterfaceDoppler::Pointer GetControlInterfaceDoppler();

    virtual mitk::NavigationDataSource::Pointer GetNavigationDataSource();

      /**
    * \brief Remove this device from the micro service.
    * This method is public for mitk::USCombinedModality, because this devices
    * can be completly removed. This is not possible for API devices, which
    * should be available while their sub module is loaded.
    */
  void UnregisterOnService();

  protected:
    USCombinedModality(USDevice::Pointer usDevice, NavigationDataSource::Pointer trackingDevice, std::string manufacturer = "", std::string model = "");
    virtual ~USCombinedModality();

    /**
    * \brief Initializes UltrasoundDevice.
    */
    virtual bool OnInitialization();

    /**
    * \brief Connects UltrasoundDevice.
    */
    virtual bool OnConnection();

    /**
    * \brief Disconnects UltrasoundDevice.
    */
    virtual bool OnDisconnection();

    /**
    * \brief Activates UltrasoundDevice.
    */
    virtual bool OnActivation();

    /**
    * \brief Deactivates UltrasoundDevice.
    */
    virtual bool OnDeactivation();

    /**
    * \brief Grabs the next frame from the input.
    * This method is called internally, whenever Update() is invoked by an Output.
    */
    void GenerateData();

    std::string GetIdentifierForCurrentCalibration();

    USDevice::Pointer                                   m_UltrasoundDevice;
    itk::SmartPointer<NavigationDataSource>             m_TrackingDevice;
    std::map<std::string, AffineTransform3D::Pointer>   m_Calibrations;

    mitk::NavigationDataSmoothingFilter::Pointer m_SmoothingFilter;
    mitk::NavigationDataDelayFilter::Pointer m_DelayFilter;

  private:
    /**
    *  \brief The device's ServiceRegistration object that allows to modify it's Microservice registraton details.
    */
    us::ServiceRegistration<Self>           m_ServiceRegistration;

    /**
    * \brief Properties of the device's Microservice.
    */
    us::ServiceProperties                   m_ServiceProperties;
  };
} // namespace mitk

#endif // MITKUSCombinedModality_H_HEADER_INCLUDED_