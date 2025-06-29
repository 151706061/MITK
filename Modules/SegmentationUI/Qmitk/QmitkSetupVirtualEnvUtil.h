/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.s

============================================================================*/

#ifndef QmitkSetupVirtualEnvUtil_h_Included
#define QmitkSetupVirtualEnvUtil_h_Included

#include "mitkLog.h"
#include "mitkProcessExecutor.h"
#include <MitkSegmentationUIExports.h>
#include <QString>
#include <functional>

/**
 * @brief Abstract Class to Setup a python virtual environment and pip install required packages.
 * Derive this class for creating installer for the respective tool.
 */
class MITKSEGMENTATIONUI_EXPORT QmitkSetupVirtualEnvUtil
{
public:
  using CallbackType = void (*)(itk::Object *, const itk::EventObject &, void *);

  QmitkSetupVirtualEnvUtil(const QString& baseDir);
  QmitkSetupVirtualEnvUtil();

  /**
   * @brief Sets up a python virtual environment in the DKFZ directory with given
   * @param venvName : Name of the virtual env folder
   * @param packages : List of packages to be installed except Pytorch
   * @param validator : Functor to run and validate the virtual env setup
   * @param printCallback : ITK functor to custom print the virtual en setup log.
   * @param torchVersion: Exact version of pytorch version (optional)
   */
  bool SetupVirtualEnv(const QString &venvName,
                       const QStringList &packages,
                       std::function<bool()> validator,
                       CallbackType printCallback,
                       const QString &torchVersion = "");

  /**
   * @brief Get the Virtual Env Path object. Override this method in the respective
   * tool installer class.
   * 
   * @return QString 
   */
  virtual QString GetVirtualEnvPath();

  /**
   * @brief Function to Pip install a library package given the location of 
   * pip3 executable.
   * Any callback function can be passed to process the output.
   * 
   * @param library 
   * @param workingDir 
   * @param callback 
   * @param command 
   */
  void PipInstall(const std::string &library,
                  const std::string &workingDir,
                  CallbackType callback,
                  const std::string &command = "pip3");

  /**
   * @brief Overloaded function to Pip install a library function.
   * 
   * @param library 
   * @param callback 
   * @param command 
   */
  void PipInstall(const std::string &library, CallbackType callback,
                  const std::string &command = "pip3");


  /**
   * @brief Function to execute any python code given a python path.
   * Any callback function can be passed to process the output.
   * 
   * @param args 
   * @param pythonPath 
   * @param callback 
   * @param command 
   */
  void ExecutePython(const std::string &args,
                     const std::string &pythonPath,
                     CallbackType callback,
                     const std::string &command = "python");

  /**
   * @brief Overloaded function to Execute Python code.
   * Any callback function can be passed to process the output.
   * 
   * @param args 
   * @param callback 
   * @param command 
   */
  void ExecutePython(const std::string &args, CallbackType callback,
                     const std::string &command = "python");

  /**
   * @brief Installs pytorch using light-the-torch package, correctly identifying cuda version.
   * Requires location of pip3 executable.
   * Any callback function can be passed to process the output.
   * 
   * @param workingDir 
   * @param callback 
   */
  void InstallPytorch(const QString &workingDir, CallbackType callback, const QString &torchVersion = "");

  /**
   * @brief Overloaded function to install pytorch using light-the-torch package, correctly 
   * identifying cuda version.
   */
  void InstallPytorch(const QString& torchVersion = "");

  /**
   * @brief Overloaded function to install pytorch using light-the-torch package, correctly
   * identifying cuda version.
   */
  void InstallPytorch(CallbackType callback, const QString& torchVersion = "");


  /**
   * @brief Get the Base Dir object
   * 
   * @return QString& 
   */
  QString& GetBaseDir();

  /**
   * @brief Get the System Python Path object
   * 
   * @return QString& 
   */
  QString& GetSystemPythonPath();

  /**
   * @brief Get the Python Path object
   * 
   * @return QString& 
   */
  QString& GetPythonPath();

  /**
   * @brief Get the Pip Path object
   * 
   * @return QString& 
   */
  QString& GetPipPath();

  /**
   * @brief Set the System Python Path object
   * 
   * @param path 
   */
  void SetSystemPythonPath(const QString& path);

  /**
   * @brief Set the Python Path object
   * 
   * @param path 
   */
  void SetPythonPath(const QString& path);

  /**
   * @brief Set the Pip Path object
   * 
   * @param path 
   */
  void SetPipPath(const QString& path);

  /**
   * @brief Set the Virtual Env Path object
   * 
   * @param path 
   */
  void SetVirtualEnvPath(const QString &path);

  /**
   * @brief Check if the path provide has python executable or not.
   * 
   * @param pythonPath 
   * @return true 
   * @return false 
   */
  bool IsPythonPath(const QString &pythonPath);

  /**
   * @brief Checks if venv module is available for the python.
   * 
   */
  static bool IsVenvInstalled(const QString &pythonPath);

  /**
   * @brief Returns version of the pip installed package
   *
   */
  static QString GetPipPackageVersion(const QString &pythonPath, const QString &packageName);

  /**
   * @brief Function can be used as callback to simply print out all the process execution output
   * parsed out from itk::EventObject.
   * 
   */
  static void PrintProcessEvent(itk::Object *, const itk::EventObject &e, void *);

  /**
  * @brief Get the exact Python path and version for any OS from the virtual environment path.
  * @return A pair of the exact python path and its Python version or empty, if an supported
  * version of Python could not be found.
  */
  static std::pair<QString, QString> GetExactPythonPath(const QString &pyEnv);
  
  /**
   * @brief Searches and parses paths of python virtual environments
   * from predefined lookout locations
   */
  static QStringList AutoParsePythonPaths();

private:
  QString m_PythonPath;
  QString m_PipPath;
  QString m_BaseDir;
  QString m_venvPath;
  QString m_SysPythonPath;
};
#endif
