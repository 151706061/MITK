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

#include <mitkNavigationDataRecorder.h>
#include <mitkNavigationDataSequentialPlayer.h>
#include <mitkNavigationDataSet.h>
#include <mitkStandardFileLocations.h>
#include <mitkTestingMacros.h>

#include <mitkNavigationDataReaderXML.h>
#include <mitkNavigationDataSetWriterXML.h>

//for exceptions
#include "mitkIGTException.h"
#include "mitkIGTIOException.h"

class mitkNavigationDataRecorderTestClass
{
public:

  static void PlayAndRecord()
  {
    // Aim is to read an xml into a pointset, play that set with a sequentialplayer, record it
    // again, write the result to xml , and compare the output

    mitk::NavigationDataReaderXML::Pointer reader = mitk::NavigationDataReaderXML::New();
    std::string path = "F://Build//MITK-Data//IGT-Data//NavigationDataSet.xml";
    mitk::NavigationDataSet::Pointer set = reader->Read(path);

    mitk::NavigationDataSequentialPlayer::Pointer player = mitk::NavigationDataSequentialPlayer::New();
    player->SetNavigationDataSet(set);

    mitk::NavigationDataRecorder::Pointer recorder = mitk::NavigationDataRecorder::New();
    recorder->SetStandardizeTime(false);

    // connect player to recorder
    for (int i = 0; i < player->GetNumberOfIndexedOutputs(); i++)
    {
      recorder->SetInput(i, player->GetOutput(i));
    }

    recorder->StartRecording();
    while (!player->IsAtEnd())
    {
      recorder->Update();
    }

    mitk::NavigationDataSetWriterXML writer;

    writer.Write("F://Build//MITK-Data//IGT-Data//NavigationDataSet2.xml", recorder->GetNavigationDataSet());

    //MITK_TEST_CONDITION_REQUIRED(mitkNavigationDataRecorderTestClass::CompareFiles("F://Build//MITK-Data//IGT-Data//NavigationDataSet.xml",
    //  "F://Build//MITK-Data//IGT-Data//NavigationDataSet2.xml"), "Asserting that played back file has been recorded correctly");

    recorder->StopRecording();
    MITK_TEST_CONDITION_REQUIRED(! recorder->GetRecording(), "Test if StopRecording is working");
    recorder->ResetRecording();
    MITK_TEST_CONDITION_REQUIRED(recorder->GetNavigationDataSet()->Size() == 0, "Test correct reset of recorder");

    //Reset Player
    //player = mitk::NavigationDataSequentialPlayer::New();
    player->SetNavigationDataSet(set);

    // Check if Limiting recording works
    //recorder->SetRecordCountLimit(100);
    recorder->StartRecording();
    while (!player->IsAtEnd())
    {
      recorder->Update();
    }

    MITK_TEST_CONDITION_REQUIRED(recorder->GetNavigationDataSet()->Size() == 152, "Test if SetRecordCountLimit works as intended.");
  }

  static bool CompareFiles(std::string file1, std::string file2)
  {
    FILE* f1 = fopen (file1.c_str() , "r");
    FILE* f2 = fopen (file2.c_str() , "r");
    char buf1[10000];
    char buf2[10000];

    do {
      size_t r1 = fread(buf1, 1, 10000, f1);
      size_t r2 = fread(buf2, 1, 10000, f2);

      if (r1 != r2 ||
        memcmp(buf1, buf2, r1)) {
          return false;  // Files are not equal
      }
    } while (!feof(f1) && !feof(f2));

    return feof(f1) && feof(f2);
  }
};

/**Documentation
*  test for the class "NavigationDataRecorder".
*/
int mitkNavigationDataRecorderTest(int /* argc */, char* /*argv*/[])
{
  MITK_TEST_BEGIN("NavigationDataRecorder");

  mitkNavigationDataRecorderTestClass::PlayAndRecord();
  // always end with this!
  MITK_TEST_END();
}