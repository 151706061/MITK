/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "mitkMovieGenerator.h"
#include "mitkConfig.h"
#include <mitkRenderingManager.h>
#include "vtk_glad.h"

#if WIN32
#ifndef __GNUC__
//#if ! (_MSC_VER >= 1400)
#include "mitkMovieGeneratorWin32.h"
//#endif
#else
#include "GL/glext.h"
#endif
#endif
#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

mitk::MovieGenerator::MovieGenerator()
  : m_stepper(nullptr), m_renderer(nullptr), m_width(0), m_height(0), m_initialized(false), m_FrameRate(20)
{
  m_fileName[0] = 0;
}

mitk::MovieGenerator::Pointer mitk::MovieGenerator::New()
{
  Pointer smartPtr;
  MovieGenerator *rawPtr = ::itk::ObjectFactory<MovieGenerator>::Create();
  if (rawPtr == nullptr)
  {
#ifdef WIN32
#ifndef __GNUC__
    //#if ! (_MSC_VER >= 1400)
    mitk::MovieGenerator::Pointer wp = static_cast<mitk::MovieGenerator *>(mitk::MovieGeneratorWin32::New());
    return wp;
//#endif
#endif
#endif
  }
  smartPtr = rawPtr;
  if (rawPtr != nullptr)
    rawPtr->UnRegister();
  return smartPtr;
}

bool mitk::MovieGenerator::WriteMovie()
{
  bool ok = false;
  if (m_stepper)
  {
    if (m_renderer)
      m_renderer->GetRenderWindow()->MakeCurrent();
    // m_stepper->First();
    RenderingManager::GetInstance()->ForceImmediateUpdate(m_renderer->GetRenderWindow());

    ok = InitGenerator();
    if (!ok)
    {
      TerminateGenerator();
      return false;
    }
    int imgSize = 3 * m_width * m_height;
    printf("Video size = %i x %i\n", m_width, m_height);
    auto *data = new GLbyte[imgSize];

    // duplicate steps if pingPong option is switched to on.
    unsigned int numOfSteps = m_stepper->GetSteps();
    if (m_stepper->GetPingPong())
      numOfSteps *= 2;

    for (unsigned int i = 0; i < numOfSteps; i++)
    {
      if (m_renderer)
        m_renderer->GetRenderWindow()->MakeCurrent();
      RenderingManager::GetInstance()->ForceImmediateUpdate(m_renderer->GetRenderWindow());
      glReadPixels(5, 5, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, (void *)data);
      AddFrame(data);
      m_stepper->Next();
    }
    ok = TerminateGenerator();
    delete[] data;
  }
  return ok;
}

bool mitk::MovieGenerator::WriteCurrentFrameToMovie()
{
  if (m_renderer)
  {
    m_renderer->GetRenderWindow()->MakeCurrent();

    if (!m_initialized)
    {
      RenderingManager::GetInstance()->ForceImmediateUpdate(m_renderer->GetRenderWindow());
      m_initialized = InitGenerator();
    }
    if (!m_initialized)
    {
      TerminateGenerator();
      return false;
    }
    int imgSize = 3 * m_width * m_height;
    auto *data = new GLbyte[imgSize];

    RenderingManager::GetInstance()->ForceImmediateUpdate(m_renderer->GetRenderWindow());
    glReadPixels(5, 5, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, (void *)data);
    AddFrame(data);
    delete[] data;
  }
  return true;
}

void mitk::MovieGenerator::ReleaseMovieWriter()
{
  TerminateGenerator();
  m_initialized = false;
}

void mitk::MovieGenerator::SetFrameRate(unsigned int rate)
{
  m_FrameRate = rate;
}

unsigned int mitk::MovieGenerator::GetFrameRate()
{
  return m_FrameRate;
}
