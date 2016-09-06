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


#ifndef MITKPumpLaserController_H_HEADER_INCLUDED
#define MITKPumpLaserController_H_HEADER_INCLUDED

#include "itkObject.h"
#include "mitkCommon.h"

#include <chrono>
#include <thread>
#include <mutex>

#include "mitkSerialCommunication.h"
#include "MitkPhotoacousticsExports.h"

namespace mitk {
    
    class MITKPHOTOACOUSTICS_EXPORT PumpLaserController : public itk::LightObject
    {
    public:
      mitkClassMacroItkParent(PumpLaserController, itk::LightObject);
      itkFactorylessNewMacro(Self);

      enum PumpLaserState { UNCONNECTED, STATE0, STATE1, STATE2, STATE3, STATE4, STATE5 };   ///< Type for STATE variable. The LaserDevice is always in one of these states
      /**
       * \brief Opens a connection to the device
       *
       * This may only be called if there is currently no connection to the device.
       * If OpenConnection() is successful, the object will change from UNCONNECTED state to a STATE state
       */
      virtual bool OpenConnection();

      /**
       * \brief Closes the connection to the device
       *
       * This may only be called if there is currently a connection to the device. (e.g. object is in a STATE state)
       */
      virtual bool CloseConnection(); ///< Closes the connection with the device

      virtual std::string Send(const std::string* input);

      virtual std::string ReceiveLine(std::string* answer);

      virtual void ClearSendBuffer();

      virtual void ClearReceiveBuffer();
      virtual void StayAlive();
      virtual bool StartFlashlamps();
      virtual bool StopFlashlamps();

      virtual bool StartQswitch();
      virtual bool StopQswitch();

      virtual PumpLaserState GetState();

      typedef mitk::SerialCommunication::PortNumber PortNumber; ///< Port number of the serial connection
      typedef mitk::SerialCommunication::BaudRate BaudRate;     ///< Baud rate of the serial connection
      typedef mitk::SerialCommunication::DataBits DataBits;     ///< Number of data bits used in the serial connection
      typedef mitk::SerialCommunication::Parity Parity;         ///< Parity mode used in the serial connection
      typedef mitk::SerialCommunication::StopBits StopBits;     ///< Number of stop bits used in the serial connection
      typedef mitk::SerialCommunication::HardwareHandshake HardwareHandshake; ///< Hardware handshake mode of the serial connection

    private:
      PumpLaserState m_State; ///< current Laser state
    protected:

      PumpLaserController();
      virtual ~PumpLaserController();

      bool m_KeepAlive = false;
      bool m_FlashlampRunning = false;
      bool m_ShutterOpen = false;
      bool m_LaserEmission = false;
      
      std::string m_DeviceName;///< Device Name
      PortNumber m_PortNumber; ///< COM Port Number
      BaudRate m_BaudRate;     ///< COM Port Baud Rate
      DataBits m_DataBits;     ///< Number of Data Bits per token
      Parity m_Parity;         ///< Parity mode for communication
      StopBits m_StopBits;     ///< number of stop bits per token
      HardwareHandshake m_HardwareHandshake; ///< use hardware handshake for serial port connection

      mitk::SerialCommunication::Pointer m_SerialCommunication;    ///< serial communication interface
      std::thread m_StayAliveMessageThread;
      std::mutex m_SerialCommunicationMutex; ///< mutex for coordinated access of serial communication interface
    };
} // namespace mitk

#endif /* MITKPumpLaserController_H_HEADER_INCLUDED */
