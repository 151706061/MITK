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

#ifndef OptiTrackErrorMessages_H_HEADER_INCLUDED_
#define OptiTrackErrorMessages_H_HEADER_INCLUDED_

/**
* \brief Maximum number of attempts for Initialization, Shutdown and CleanUp
*/
#ifndef OPTITRACK_ATTEMPTS
#define OPTITRACK_ATTEMPTS 10
#endif

/**
* \brief Time to refresh the tools location (ms)
*/
#ifndef OPTITRACK_FRAME_RATE
#define OPTITRACK_FRAME_RATE 25
#endif



namespace mitk
{
/**
  * \brief Helper function to get the error messages from Optitrack API
  * \author E. Marinetto (emarinetto@hggm.es) Instituto de Investigaci�n Sanitaria Gregorio Mara��n, Madrid, Spain. & M. Noll (matthias.noll@igd.fraunhofer.de) Cognitive Computing & Medical Imaging | Fraunhofer IGD
  */

  static std::string GetOptitrackErrorMessage(NPRESULT result)
  {
      std::string message = "";
      switch(result)
      {
        case NPRESULT_SUCCESS: message = "[Optitrack API] Successful Result"; break;
        case NPRESULT_FILENOTFOUND: message = "[Optitrack API] File Not Found"; break;
        case NPRESULT_LOADFAILED:  message = "[Optitrack API] Load Failed"; break;
        case NPRESULT_FAILED:    message = "[Optitrack API] Failed"; break;
        case NPRESULT_INVALIDFILE:  message = "[Optitrack API] Invalid File"; break;
        case NPRESULT_INVALIDCALFILE:  message = "[Optitrack API] Invalid Calibration File"; break;
        case NPRESULT_UNABLETOINITIALIZE:  message = "[Optitrack API] Unable To Initialize"; break;
        case NPRESULT_INVALIDLICENSE:  message = "[Optitrack API] Invalid License"; break;
        case NPRESULT_NOFRAMEAVAILABLE:  message = "[Optitrack API] No Frames Available"; break;
        default: message = "[Optitrack API] Unknown error occured"; break;
      }
      return message;
    }
}


#endif
