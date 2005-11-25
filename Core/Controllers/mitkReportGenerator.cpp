/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <fstream>
#include <ios>
#include "mitkReportGenerator.h"

void mitk::ReportGenerator::AddKey(std::string key,std::string value) {
  m_KeyValueMap.insert(KeyValueMapType::value_type(key,value));
}

void mitk::ReportGenerator::Generate() {
  std::ifstream templateFile(m_TemplateFileName.c_str(), std::ios::in);
  std::string line;
  int i=0;
  if (templateFile) {
    while (!std::getline(templateFile,line).eof()) {
      std::cout << "orig " << ++i << ":" << line << std::endl;
      for (KeyValueMapType::iterator it = m_KeyValueMap.begin(); it != m_KeyValueMap.end() ; it++) {
        std::string key = it->first ;    
        int idx = line.find(key);
        if (idx >=0 && idx < line.size() ) {
          std::string value = m_KeyValueMap[key];
          line.replace(idx, value.size(), value);
        }
      }
      std::cout << "new  " << ++i << ":" << line << std::endl;
    } 
  }
}
