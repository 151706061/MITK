MACRO(ORGANIZE_SOURCES)

  # this macro gets a filelist as input and looks
  # for corresponding h-files to add them to the project.
  
  # additionally files are grouped in source-groups.
  
  # No parameters explicitly declared here, because
  # we want to allow for variable argument lists, which
  # are later access by the keyword FOREACH(MYFILE ${ARGV})
  
  # output: after calling the macro, files that were found
  # correspondigly to the given files are stored in the
  # variable:
  # ${CORRESPONDING_H_FILES}
  # ${CORRESPONDING_TXX_FILES}
  # ${CORRESPONDING_UI_H_FILES}
  
  # Globbed can be found in the variables
  # ${GLOBBED_TXX_FILES} (CURRENTLY COMMENTED OUT)
  # ${GLOBBED_DOX_FILES}

  MACRO_PARSE_ARGUMENTS(_ORG "HEADER;SOURCE;TXX;DOC;MOC;GEN_QRC;GEN_UI;UI;QRC" "" ${ARGN})

  SET(CORRESPONDING__H_FILES "" )
  SET(GLOBBED__H_FILES "" )
  
  IF(_ORG_HEADER)
    FOREACH(_file ${_ORG_SOURCE})
      STRING(REGEX REPLACE "(.*)\\.(txx|cpp|c|cxx)$" "\\1.h" H_FILE ${_file})
      IF(EXISTS ${H_FILE})
        LIST(APPEND CORRESPONDING__H_FILES "${H_FILE}")
      ENDIF()
    ENDFOREACH()
  ELSE()
    FILE(GLOB_RECURSE GLOBBED__H_FILES *.h)
  ENDIF()
  
  IF(_ORG_GEN_QRC OR _ORG_GEN_UI OR _ORG_MOC)
    SOURCE_GROUP("Generated\\Qt QRC Source Files" FILES ${_ORG_GEN_QRC})
    SOURCE_GROUP("Generated\\Qt UI Header Files" FILES ${_ORG_GEN_UI})
    SOURCE_GROUP("Generated\\Qt MOC Source Files" FILES ${_ORG_MOC})
  ENDIF()
  
  #_MACRO_APPEND_TO_LIST(_ORG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("== Source Files ==" FILES ${_ORG_SOURCE})
  
  #_MACRO_APPEND_TO_LIST(_ORG_TXX "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("== Template Files ==" FILES ${_ORG_TXX})
  
  #_MACRO_APPEND_TO_LIST(_ORG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("== Header Files ==" FILES ${_ORG_HEADER} ${_ORG_HEADER} ${CORRESPONDING__H_FILES} ${GLOBBED__H_FILES})
  
  
  #_MACRO_APPEND_TO_LIST(_ORG_UI "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("QT UI Files" FILES ${_ORG_UI})
  
  #_MACRO_APPEND_TO_LIST(_ORG_DOC "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Doxygen Files" FILES ${_ORG_DOC})
  
  #_MACRO_APPEND_TO_LIST(_ORG_QRC "${CMAKE_CURRENT_SOURCE_DIR}/")
  SOURCE_GROUP("Qt Resource Files" FILES ${_ORG_QRC})
  
ENDMACRO(ORGANIZE_SOURCES)

MACRO(APPLY_VTK_FLAGS)
  IF(NOT MITK_VTK_FLAGS_APPLIED)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VTK_REQUIRED_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VTK_REQUIRED_CXX_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${VTK_REQUIRED_EXE_LINKER_FLAGS}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${VTK_REQUIRED_SHARED_LINKER_FLAGS}")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${VTK_REQUIRED_MODULE_LINKER_FLAGS}")
    SET(MITK_VTK_FLAGS_APPLIED 1)
  ENDIF(NOT MITK_VTK_FLAGS_APPLIED)
ENDMACRO(APPLY_VTK_FLAGS)

# increase heap limit for MSVC70. Assumes /Zm1000 is set by ITK
MACRO(INCREASE_MSVC_HEAP_LIMIT)
IF(MSVC70)
 STRING(REPLACE /Zm1000 /Zm1200 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})  
ENDIF(MSVC70)
ENDMACRO(INCREASE_MSVC_HEAP_LIMIT)

# suppress some warnings in VC8 about using unsafe/deprecated c functions
MACRO(SUPPRESS_VC8_DEPRECATED_WARNINGS)
IF(MSVC80)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
ENDIF(MSVC80)
ENDMACRO(SUPPRESS_VC8_DEPRECATED_WARNINGS)

MACRO(SUPPRESS_ALL_WARNINGS)
IF(MSVC_VERSION)
  STRING(REGEX REPLACE "/W[0-9]" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})   
  STRING(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})   
  ADD_DEFINITIONS(/w)
  # suppress also poco linker warnings
  SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4217")
  SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /ignore:4217")
ENDIF(MSVC_VERSION)
IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS(-w)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDMACRO(SUPPRESS_ALL_WARNINGS)

INCLUDE(${CMAKE_ROOT}/Modules/TestCXXAcceptsFlag.cmake)
MACRO(CHECK_AND_SET flag sourcelist )
  CHECK_CXX_ACCEPTS_FLAG(${flag} R)
  IF(R)
    SET_SOURCE_FILES_PROPERTIES(
      ${${sourcelist}}
       PROPERTIES
       COMPILE_FLAGS ${flag}
      )
  ENDIF(R)
ENDMACRO(CHECK_AND_SET)

#
# MITK_MULTIPLEX_PICTYPE: generate separated source files for different
# data types to reduce memory consumption of compiler during template 
# instantiation
#
# Param "file" should be named like mitkMyAlgo-TYPE.cpp
# in the file, every occurence of @TYPE@ is replaced by the
# datatype. For each datatype, a new file mitkMyAlgo-datatype.cpp 
# is generated and added to CPP_FILES_GENERATED. 
#
MACRO(MITK_MULTIPLEX_PICTYPE file)
  SET(TYPES "double;float;int;unsigned int;short;unsigned short;char;unsigned char")
  FOREACH(TYPE ${TYPES})
    # create filename for destination
    STRING(REPLACE " " "_" quoted_type "${TYPE}")
    STRING(REPLACE TYPE ${quoted_type} quoted_file ${file})
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${quoted_file} @ONLY)
    SET(CPP_FILES_GENERATED ${CPP_FILES_GENERATED} ${CMAKE_CURRENT_BINARY_DIR}/${quoted_file})
  ENDFOREACH(TYPE)
ENDMACRO(MITK_MULTIPLEX_PICTYPE)

INCLUDE(MacroParseArguments)


###################################################################
#
# MITK_CREATE_MODULE_CONF 
# 
# This can be called in a similar way like MITK_CREATE_MODULE
# but it just creates the module configuration files without
# actually building it. It is used for integration of legacy libraries
# into the MITK module build system
#
##################################################################
MACRO(MITK_CREATE_MODULE_CONF MODULE_NAME_IN)
  MACRO_PARSE_ARGUMENTS(MODULE "INCLUDE_DIRS;DEPENDS;PROVIDES" "QT_MODULE" ${ARGN})
  SET(MODULE_NAME ${MODULE_NAME_IN})
  SET(MODULE_IS_ENABLED 1)
  _MITK_CREATE_MODULE_CONF()
ENDMACRO(MITK_CREATE_MODULE_CONF)

MACRO(_MITK_CREATE_MODULE_CONF)
  IF(NOT MODULE_QT_MODULE OR MITK_USE_QT)
    IF(NOT MODULE_PROVIDES AND NOT MODULE_HEADERS_ONLY)
      SET(MODULE_PROVIDES ${MODULE_NAME})
    ENDIF(NOT MODULE_PROVIDES AND NOT MODULE_HEADERS_ONLY)
    SET(MODULE_INCLUDE_DIRS_ABSOLUTE "")
    FOREACH(dir ${MODULE_INCLUDE_DIRS})
      GET_FILENAME_COMPONENT(abs_dir ${dir} ABSOLUTE)
      SET(MODULE_INCLUDE_DIRS_ABSOLUTE ${MODULE_INCLUDE_DIRS_ABSOLUTE} ${abs_dir})
    ENDFOREACH(dir)

    SET(MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS_ABSOLUTE} ${CMAKE_CURRENT_SOURCE_DIR})
    
    # Qt generates headers in the binary tree
    IF(MODULE_QT_MODULE)
      SET(MODULE_INCLUDE_DIRS ${MODULE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
    ENDIF(MODULE_QT_MODULE)
    
    CONFIGURE_FILE(${MITK_SOURCE_DIR}/CMake/moduleConf.cmake.in ${MITK_MODULES_CONF_DIR}/${MODULE_NAME}Config.cmake @ONLY)
  ENDIF(NOT MODULE_QT_MODULE OR MITK_USE_QT)
ENDMACRO(_MITK_CREATE_MODULE_CONF)

##################################################################
#
# MITK_CREATE_MODULE 
#
# Creates a module for the automatic module dependeny system within MITK.
# Configurations are generated in the moduleConf directory. 
#
# USAGE:
#
# MITK_CREATE_MODULE( <moduleName>
#     [INCLUDE_DIRS <include directories>]
#     [INTERNAL_INCLUDE_DIRS <internally used include directories>]
#     [DEPENDS <modules we need>]
#     [PROVIDES <library name which is built>]
#     [PACKAGE_DEPENDS <packages we need, like ITK, VTK, QT>]
#     [EXPORT_DEFINE <declspec macro name for dll exports>]
#     [QT_MODULE]
#
##################################################################
MACRO(MITK_CREATE_MODULE MODULE_NAME_IN)
  MACRO_PARSE_ARGUMENTS(MODULE "INCLUDE_DIRS;INTERNAL_INCLUDE_DIRS;DEPENDS;DEPENDS_INTERNAL;PROVIDES;PACKAGE_DEPENDS;EXPORT_DEFINE;ADDITIONAL_LIBS;GENERATED_CPP" "QT_MODULE;FORCE_STATIC;HEADERS_ONLY" ${ARGN})
  SET(MODULE_NAME ${MODULE_NAME_IN})
  
  # assume worst case
  SET(MODULE_IS_ENABLED 0)
  # first of all we check for the dependencies
  MITK_CHECK_MODULE(_MISSING_DEP ${MODULE_DEPENDS})
  IF(_MISSING_DEP)
    MESSAGE("Warning: Module ${MODULE_NAME} won't be built, missing dependency: ${_MISSING_DEP}") 
    SET(MODULE_IS_ENABLED 0)
  ELSE(_MISSING_DEP)
    SET(MODULE_IS_ENABLED 1)
    # now check for every package if it is enabled. This overlaps a bit with
    # MITK_CHECK_MODULE ... 
    FOREACH(_package ${MODULE_PACKAGE_DEPENDS})
      IF((DEFINED MITK_USE_${_package}) AND NOT (MITK_USE_${_package}))
        MESSAGE("Module ${MODULE_NAME} won't be built. Turn on MITK_USE_${_package} if you want to use it.")
        SET(MODULE_IS_ENABLED 0)
      ENDIF()  
    ENDFOREACH()
    IF(MODULE_IS_ENABLED)
      IF(NOT MODULE_QT_MODULE OR MITK_USE_QT)
      SET(MODULE_IS_ENABLED 1)
      _MITK_CREATE_MODULE_CONF()
      IF(NOT MODULE_EXPORT_DEFINE)
        SET(MODULE_EXPORT_DEFINE ${MODULE_NAME}_EXPORT)
      ENDIF(NOT MODULE_EXPORT_DEFINE)
      CONFIGURE_FILE(${MITK_SOURCE_DIR}/CMake/moduleExports.h.in ${MITK_MODULES_CONF_DIR}/${MODULE_NAME}Exports.h @ONLY)
      
      IF(MITK_GENERATE_MODULE_DOT)
      MESSAGE("MODULEDOTNAME ${MODULE_NAME}")
      FOREACH(dep ${MODULE_DEPENDS})
        MESSAGE("MODULEDOT \"${MODULE_NAME}\" -> \"${dep}\" ; ")
      ENDFOREACH(dep)
      ENDIF(MITK_GENERATE_MODULE_DOT)
     
      SET(DEPENDS "${MODULE_DEPENDS}") 
      SET(DEPENDS_BEFORE "not initialized")
      SET(PACKAGE_DEPENDS "${MODULE_PACKAGE_DEPENDS}")
      MITK_USE_MODULE("${MODULE_DEPENDS}") 
      
      # ok, now create the module itself 
      INCLUDE_DIRECTORIES(. ${ALL_INCLUDE_DIRECTORIES})
      INCLUDE(files.cmake)
      
      IF(NOT MODULE_QT_MODULE)
        ORGANIZE_SOURCES(SOURCE ${CPP_FILES}
             HEADER  ${H_FILES}
               TXX ${TXX_FILES}
               DOC ${DOX_FILES}
               )

        IF(MODULE_FORCE_STATIC)
    SET(_STATIC ${MITK_WIN32_FORCE_STATIC})
        ENDIF(MODULE_FORCE_STATIC)

        IF(NOT MODULE_HEADERS_ONLY)
    IF(ALL_LIBRARIES)
      # LINK_DIRECTORIES applies only to targets which are added after the call to LINK_DIRECTORIES
      LINK_DIRECTORIES(${ALL_LIBRARY_DIRS})
    ENDIF(ALL_LIBRARIES)  
    ADD_LIBRARY(${MODULE_PROVIDES} ${_STATIC} ${CPP_FILES} ${H_FILES} ${CPP_FILES_GENERATED} ${GLOBBED__H_FILES} ${CORRESPONDING__H_FILES} ${TXX_FILES} ${DOX_FILES} ${UI_FILES} ${TOOL_CPPS})
    IF(ALL_LIBRARIES)
      TARGET_LINK_LIBRARIES(${MODULE_PROVIDES} ${ALL_LIBRARIES})
    ENDIF(ALL_LIBRARIES)  
        ENDIF()
      
      ELSE(NOT MODULE_QT_MODULE)

        INCLUDE(files.cmake)
      
      IF(UI_FILES)
    QT4_WRAP_UI(Q${KITNAME}_GENERATED_UI_CPP ${UI_FILES})
      ENDIF(UI_FILES)
      
      IF(MOC_H_FILES)
    QT4_WRAP_CPP(Q${KITNAME}_GENERATED_MOC_CPP ${MOC_H_FILES})
      ENDIF(MOC_H_FILES)
      
      IF(QRC_FILES)
        QT4_ADD_RESOURCES(Q${KITNAME}_GENERATED_QRC_CPP ${QRC_FILES})
      ENDIF(QRC_FILES)
      
      SET(Q${KITNAME}_GENERATED_CPP ${Q${KITNAME}_GENERATED_CPP} ${Q${KITNAME}_GENERATED_UI_CPP} ${Q${KITNAME}_GENERATED_MOC_CPP} ${Q${KITNAME}_GENERATED_QRC_CPP})
      
      ORGANIZE_SOURCES(SOURCE ${CPP_FILES} 
           HEADER ${H_FILES}
               TXX ${TXX_FILES}
               DOC ${DOX_FILES}
           UI ${UI_FILES}
           QRC ${QRC_FILES}
               MOC ${Q${KITNAME}_GENERATED_MOC_CPP}
               GEN_QRC ${Q${KITNAME}_GENERATED_QRC_CPP}
               GEN_UI ${Q${KITNAME}_GENERATED_UI_CPP})

      # MITK_GENERATE_TOOLS_LIBRARY(Qmitk${LIBPOSTFIX} "NO")
      
      IF(NOT MODULE_HEADERS_ONLY)
        ADD_LIBRARY(${MODULE_PROVIDES} ${CPP_FILES} ${CPP_FILES_GENERATED} ${Q${KITNAME}_GENERATED_CPP} ${CORRESPONDING__H_FILES} ${GLOBBED__H_FILES} ${TXX_FILES} ${DOX_FILES} ${UI_FILES} ${QRC_FILES} ${TOOL_GUI_CPPS})
        TARGET_LINK_LIBRARIES(${MODULE_PROVIDES} ${QT_LIBRARIES} ${ALL_LIBRARIES} QVTK)
      ENDIF()
      
      ENDIF(NOT MODULE_QT_MODULE) 


      # install only if shared lib (for now)
      IF(NOT _STATIC OR MINGW)
        IF(NOT MODULE_HEADERS_ONLY)
         # # deprecated: MITK_INSTALL_TARGETS(${MODULE_PROVIDES})
        ENDIF()
      ENDIF(NOT _STATIC OR MINGW)
    ENDIF(NOT MODULE_QT_MODULE OR MITK_USE_QT)
  ENDIF(MODULE_IS_ENABLED)
ENDIF(_MISSING_DEP)
  IF(NOT MODULE_IS_ENABLED)   
    _MITK_CREATE_MODULE_CONF()
  ENDIF(NOT MODULE_IS_ENABLED)   
ENDMACRO(MITK_CREATE_MODULE)

MACRO(MITK_USE_MODULE)
  SET(DEPENDS "${ARGN}") 
  SET(DEPENDS_BEFORE "not initialized")
  WHILE(NOT "${DEPENDS}" STREQUAL "${DEPENDS_BEFORE}")
    SET(DEPENDS_BEFORE ${DEPENDS})
    FOREACH(dependency ${DEPENDS})
      INCLUDE(${MITK_MODULES_CONF_DIR}/${dependency}Config.cmake)
      LIST(APPEND DEPENDS ${${dependency}_DEPENDS})
      LIST(APPEND PACKAGE_DEPENDS ${${dependency}_PACKAGE_DEPENDS})
    ENDFOREACH(dependency) 
    IF(DEPENDS)
      LIST(REMOVE_DUPLICATES DEPENDS)
      LIST(SORT DEPENDS)
    ENDIF(DEPENDS)
    IF(PACKAGE_DEPENDS)
      LIST(REMOVE_DUPLICATES PACKAGE_DEPENDS)
      LIST(SORT PACKAGE_DEPENDS)
    ENDIF(PACKAGE_DEPENDS)
  ENDWHILE()
  # CMake Debug
  FOREACH(dependency ${DEPENDS} ${MODULE_DEPENDS_INTERNAL})
    INCLUDE(${MITK_MODULES_CONF_DIR}/${dependency}Config.cmake)
    SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${dependency}_INCLUDE_DIRS})
    SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${${dependency}_PROVIDES})
    SET(ALL_LIBRARY_DIRS ${ALL_LIBRARY_DIRS} ${${dependency}_LIBRARY_DIRS})
  ENDFOREACH(dependency)

  FOREACH(package ${PACKAGE_DEPENDS})  
    INCLUDE(MITK_${package}_Config)
    #SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${${package}_INCLUDE_DIRS})
    # SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${${package}_LIBRARIES})
  ENDFOREACH(package)
  SET(ALL_LIBRARIES ${ALL_LIBRARIES} ${MODULE_ADDITIONAL_LIBS})
  SET(ALL_INCLUDE_DIRECTORIES ${ALL_INCLUDE_DIRECTORIES} ${MODULE_INCLUDE_DIRS} ${MODULE_INTERNAL_INCLUDE_DIRS} ${MITK_MODULES_CONF_DIR})
  IF(ALL_LIBRARY_DIRS)
    LIST(REMOVE_DUPLICATES ALL_LIBRARY_DIRS)
  ENDIF(ALL_LIBRARY_DIRS)

ENDMACRO(MITK_USE_MODULE)

# Usage: MITK_CHECK_MODULE(RESULT_VAR [dependencies ...] )
# check if all required modules exist and stores missing module names in RESULT_VAR.
MACRO(MITK_CHECK_MODULE RESULT_VAR)
  SET(${RESULT_VAR} "")
  SET(DEPENDS "${ARGN}") 
  SET(DEPENDS_BEFORE "not initialized")
  SET(PACKAGE_DEPENDS "")
  
  # create a list of all lowercase module names
  FILE(GLOB _ALL_MODULES RELATIVE ${MITK_MODULES_CONF_DIR} ${MITK_MODULES_CONF_DIR}/*Config.cmake)
  SET(_ALL_MODULES_LOWERCASE "")
  FOREACH(_module ${_ALL_MODULES})
    STRING(TOLOWER ${_module} _lowermodule)
    LIST(APPEND _ALL_MODULES_LOWERCASE ${_lowermodule})
  ENDFOREACH(_module ${_ALL_MODULES})
  
  WHILE(NOT "${DEPENDS}" STREQUAL "${DEPENDS_BEFORE}")
    SET(DEPENDS_BEFORE ${DEPENDS})
    FOREACH(dependency ${DEPENDS})
      
      SET(_dependency_file_name ${dependency}Config.cmake)
      LIST(FIND _ALL_MODULES ${_dependency_file_name} _index)
      IF(NOT _index EQUAL -1)
        INCLUDE(${MITK_MODULES_CONF_DIR}/${dependency}Config.cmake)
        IF(${dependency}_IS_ENABLED)
          LIST(APPEND DEPENDS ${${dependency}_DEPENDS})
          LIST(APPEND PACKAGE_DEPENDS ${${dependency}_PACKAGE_DEPENDS})
        ELSE(${dependency}_IS_ENABLED)
          LIST(APPEND ${RESULT_VAR} ${dependency})
          LIST(REMOVE_DUPLICATES ${RESULT_VAR})
        ENDIF(${dependency}_IS_ENABLED)
      ELSE(NOT _index EQUAL -1)
        STRING(TOLOWER ${_dependency_file_name} _lowercase_dependency_file_name)
        LIST(FIND _ALL_MODULES_LOWERCASE ${_lowercase_dependency_file_name} _index_lower)
        IF(NOT _index_lower EQUAL -1)
          LIST(GET _ALL_MODULES ${_index_lower} _real_module_name)
          STRING(REPLACE "Config.cmake" "" _real_module_name ${_real_module_name})
          MESSAGE("Warning: case mismatch for module name ${dependency}, did you mean ${_real_module_name} ?")
        ENDIF(NOT _index_lower EQUAL -1)
        LIST(APPEND ${RESULT_VAR} ${dependency})
        LIST(REMOVE_DUPLICATES ${RESULT_VAR})
      ENDIF(NOT _index EQUAL -1)

    ENDFOREACH(dependency) 
    LIST(REMOVE_DUPLICATES DEPENDS)
    LIST(REMOVE_DUPLICATES PACKAGE_DEPENDS)
    LIST(SORT DEPENDS)
    LIST(SORT PACKAGE_DEPENDS)
  ENDWHILE()
  FOREACH(_package PACKAGE_DEPENDS)
  IF((DEFINED MITK_USE_${_package}) AND NOT (${MITK_USE_${_package}}))
    LIST(APPEND ${RESULT_VAR} ${_package})
  ENDIF()  
  ENDFOREACH()

ENDMACRO(MITK_CHECK_MODULE)

#
# MITK specific cross plattform install macro
#
# Usage: MITK_INSTALL_TARGETS(target1 [target2] ....)
#
MACRO(MITK_INSTALL_TARGETS)
  MACRO_PARSE_ARGUMENTS(_install "TARGETS;EXECUTABLES;PLUGINS;LIBRARY_DIRS" "GLOB_PLUGINS" ${ARGN})
  LIST(APPEND _install_TARGETS ${_install_DEFAULT_ARGS})

  IF(WIN32)
  SET(MITK_INSTALLED_VERSION_LIB bin)
  SET(MITK_INSTALLED_VERSION_ARCHIVES bin)
  SET(MITK_INSTALLED_VERSION_BIN bin)
  ELSE()
  SET(MITK_INSTALLED_VERSION_LIB lib/mitk)
  SET(MITK_INSTALLED_VERSION_ARCHIVES lib/mitk/static)
  SET(MITK_INSTALLED_VERSION_BIN bin)
  ENDIF() 

  IF(WIN32)
   INSTALL(TARGETS ${_install_TARGETS} ${_install_EXECUTABLES}
     RUNTIME DESTINATION bin
   )
  ELSE()
   INSTALL(TARGETS ${_install_TARGETS} ${_install_EXECUTABLES} CONFIGURATIONS Release
     BUNDLE DESTINATION .
     RUNTIME DESTINATION bin
     # LIBRARY DESTINATION lib/mitk/
     # ARCHIVE DESTINATION lib/mitk/static
   )
  ENDIF()

SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
# SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/QtTest")
IF(APPLE)
  SET(plugin_dest_dir QtTest.app/Contents/MacOS)
  SET(qtconf_dest_dir QtTest.app/Contents/Resources)
  # SET(APPS "\${CMAKE_INSTALL_PREFIX}/QtTest.app")
ENDIF(APPLE)
# IF(WIN32)
#   SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/QtTest.exe")
# ENDIF(WIN32)


# SET(DIRS ${VTK_RUNTIME_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} ${QT_LIBRARY_DIR} ${MITK_BINARY_DIR}/bin ${CMAKE_INSTALL_PREFIX}/MacOS )
# TODO: how to supply to correct intermediate directory??
# CMAKE_CFG_INTDIR is not expanded to actual values inside the INSTALL(CODE "...") macro ...
SET(intermediate_dir )
IF(WIN32 AND NOT MINGW)
  SET(intermediate_dir Release)
ENDIF()
SET(DIRS 
  ${VTK_RUNTIME_LIBRARY_DIRS}/${intermediate_dir}
  ${ITK_LIBRARY_DIRS}/${intermediate_dir}
  ${QT_LIBRARY_DIR} 
  ${MITK_BINARY_DIR}/bin/${intermediate_dir} 
  ${_install_LIBRARY_DIRS}
  )

FOREACH(_target ${_install_EXECUTABLES})

GET_TARGET_PROPERTY(_is_bundle ${_target} MACOSX_BUNDLE)
IF(APPLE AND _is_bundle)
  SET(_target_location \${CMAKE_INSTALL_PREFIX}/${_target}.app)
  SET(plugin_dest_dir ${_target}.app/Contents/MacOS)
  SET(qtconf_dest_dir ${_target}.app/Contents/Resources)
ELSE()
  SET(_target_location \${CMAKE_INSTALL_PREFIX}/bin/${_target}${CMAKE_EXECUTABLE_SUFFIX})
  SET(plugin_dest_dir bin)
  SET(qtconf_dest_dir bin)
ENDIF()

IF(QT_PLUGINS_DIR)
  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}" 
          DESTINATION ${plugin_dest_dir} 
		  CONFIGURATIONS Release
		  
		  FILES_MATCHING REGEX "[^d]4\\.dll$"
		  )
		  
  INSTALL(DIRECTORY "${QT_PLUGINS_DIR}" 
          DESTINATION ${plugin_dest_dir} 
		  CONFIGURATIONS Debug
		  
		  FILES_MATCHING PATTERN "*d4.dll"
		  )

  #--------------------------------------------------------------------------------
  # install a qt.conf file
  # this inserts some cmake code into the install script to write the file
  INSTALL(CODE "
      file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
      ")




ENDIF(QT_PLUGINS_DIR)




IF(_install_GLOB_PLUGINS)

INSTALL(CODE "
    MACRO(gp_item_default_embedded_path_override item default_embedded_path_var)
      GET_FILENAME_COMPONENT(_item_name \"\${item}\" NAME)
      GET_FILENAME_COMPONENT(_item_path \"\${item}\" PATH)
      IF(_item_name MATCHES \"liborg\")
         IF(APPLE)
           SET(full_path \"full_path-NOTFOUND\")
           MESSAGE(\"override: \${item}\")        
           FILE (GLOB_RECURSE full_path \${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/\${_item_name} )
           MESSAGE(\"find file: \${full_path}\")        
           
           GET_FILENAME_COMPONENT(_item_path \"\${full_path}\" PATH)
           
           STRING(REPLACE 
              \${CMAKE_INSTALL_PREFIX} 
              @executable_path/../../../ \${default_embedded_path_var} \"\${_item_path}\" ) 
           MESSAGE(\"override result: \${\${default_embedded_path_var}}\")        
         ELSE(APPLE)
              SET(\${default_embedded_path_var} \"\${_item_path}\")
         ENDIF(APPLE)
      ENDIF()

      #IF(_item_name MATCHES \"^liborg\")
      #   IF(APPLE)
      #     MESSAGE(\"override: \${item}\")        
      #     STRING(REPLACE 
      #        ${CMAKE_INSTALL_PREFIX} 
      #        @loader_path/../ \${default_embedded_path_var} \"\${_item_path}\" ) 
      #   ELSE(APPLE)
      #        SET(\${default_embedded_path_var} \"\${_item_path}\")
      #   ENDIF(APPLE)
      #ENDIF()
    ENDMACRO(gp_item_default_embedded_path_override)

    MACRO(gp_resolved_file_type_override file type)
      IF(NOT APPLE)
        GET_FILENAME_COMPONENT(_file_path \"\${file}\" PATH)
        GET_FILENAME_COMPONENT(_file_name \"\${file}\" NAME)
        IF(_file_path MATCHES \"^\${CMAKE_INSTALL_PREFIX}\")
          SET(\${type} \"local\")
          # On linux, rpaths are removed from the plugins
          # if installing more than on application. This override
          # should prevent this.
          IF(_file_name MATCHES \"^liborg\")
            SET(\${type} \"system\")
          ENDIF(_file_name MATCHES \"^liborg\")       

         ENDIF()
      ENDIF()
    ENDMACRO(gp_resolved_file_type_override)
 
    file(GLOB_RECURSE PLUGINS
      # glob for all blueberry bundles of this application
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/${CMAKE_SHARED_LIBRARY_PREFIX}org*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      # glob for Qt plugins
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")    
    #file(GLOB_RECURSE PLUGINS
     # \"${MITK_BINARY_DIR}/liborg*${CMAKE_SHARED_LIBRARY_SUFFIX}\")    
    # use custom version of BundleUtilities
    message(\"globbed plugins: \${PLUGINS}\")
    SET(PLUGIN_DIRS ${DIRS})
    LIST(APPEND PLUGINS ${_install_PLUGINS})
    foreach(_plugin \${PLUGINS})
      get_filename_component(_pluginpath \${_plugin} PATH)
      list(APPEND PLUGIN_DIRS \${_pluginpath})
    endforeach(_plugin)
    list(APPEND DIRS \${PLUGIN_DIRS})
    # use custom version of BundleUtilities
    SET(CMAKE_MODULE_PATH ${MITK_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH} )
    include(BundleUtilities)
    fixup_bundle(\"${_target_location}\" \"\${PLUGINS}\" \"\${PLUGIN_DIRS}\")
    ")

ELSE(_install_GLOB_PLUGINS)

INSTALL(CODE "
    # use custom version of BundleUtilities
    SET(CMAKE_MODULE_PATH ${MITK_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH} )
    include(BundleUtilities)
    fixup_bundle(\"${_target_location}\" \"${_install_PLUGINS}\" \"${DIRS}\")
    ")

ENDIF(_install_GLOB_PLUGINS)

ENDFOREACH(_target)


ENDMACRO(MITK_INSTALL_TARGETS)


#
# Create tests and testdriver for this module
# 
# Usage: MITK_CREATE_MODULE_TESTS( [EXTRA_DRIVER_INIT init_code]  )
# 
# EXTRA_DRIVER_INIT is inserted as c++ code in the testdriver and will be executed before each test
#
MACRO(MITK_CREATE_MODULE_TESTS)
MACRO_PARSE_ARGUMENTS(MODULE_TEST "EXTRA_DRIVER_INIT;EXTRA_DRIVER_INCLUDE" "" ${ARGN})
IF(BUILD_TESTING AND MODULE_IS_ENABLED)
  SET(OLD_MOC_H_FILES ${MOC_H_FILES})
  SET(MOC_H_FILES)
  INCLUDE(files.cmake)
  INCLUDE_DIRECTORIES(.)
  
  IF(DEFINED MOC_H_FILES)
    QT4_WRAP_CPP(MODULE_TEST_GENERATED_MOC_CPP ${MOC_H_FILES})
  ENDIF(DEFINED MOC_H_FILES)
   
  SET(CMAKE_TESTDRIVER_BEFORE_TESTMAIN "mitk::LoggingBackend::Register(); ${MODULE_TEST_EXTRA_DRIVER_INIT};")
  SET(CMAKE_TESTDRIVER_AFTER_TESTMAIN "mitk::LoggingBackend::Unregister();")
  IF(NOT MODULE_TEST_EXTRA_DRIVER_INCLUDE)
    # this is necessary to make the LoggingBackend calls available if nothing else is included
    SET(MODULE_TEST_EXTRA_DRIVER_INCLUDE "mitkLog.h")
  ENDIF(NOT MODULE_TEST_EXTRA_DRIVER_INCLUDE)
  CREATE_TEST_SOURCELIST(MODULETEST_SOURCE ${MODULE_NAME}TestDriver.cpp 
    ${MODULE_TESTS} ${MODULE_IMAGE_TESTS} ${MODULE_CUSTOM_TESTS}
    EXTRA_INCLUDE ${MODULE_TEST_EXTRA_DRIVER_INCLUDE}
  )
  SET(TESTDRIVER ${MODULE_NAME}TestDriver)
  ADD_EXECUTABLE(${TESTDRIVER} ${MODULETEST_SOURCE} ${MODULE_TEST_GENERATED_MOC_CPP})
  TARGET_LINK_LIBRARIES(${TESTDRIVER} ${MODULE_PROVIDES} ${ALL_LIBRARIES})
  #
  # Now tell CMake which tests should be run. This is done automatically 
  # for all tests in ${KITNAME}_TESTS and ${KITNAME}_IMAGE_TESTS. The IMAGE_TESTS
  # are run for each image in the TESTIMAGES list.
  #
  FOREACH( test ${MODULE_TESTS} )
    GET_FILENAME_COMPONENT(TName ${test} NAME_WE)
    ADD_TEST(${TName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TESTDRIVER} ${TName})
  ENDFOREACH( test )

  FOREACH(image ${MODULE_TESTIMAGES} ${ADDITIONAL_TEST_IMAGES} )
    IF(EXISTS ${image})
      SET(IMAGE_FULL_PATH ${image})
    ELSE(EXISTS ${image})
      # todo: maybe search other paths as well
      # yes, please in mitk/Testing/Data, too
      SET(IMAGE_FULL_PATH ${MITK_SOURCE_DIR}/Core/Code/Testing/Data/${image})
    ENDIF(EXISTS ${image})

    IF(EXISTS ${IMAGE_FULL_PATH})
    FOREACH( test ${MODULE_IMAGE_TESTS} )
      GET_FILENAME_COMPONENT(TName ${test} NAME_WE)
      GET_FILENAME_COMPONENT(ImageName ${IMAGE_FULL_PATH} NAME)
      ADD_TEST(${TName}_${ImageName} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TESTDRIVER} ${TName} ${IMAGE_FULL_PATH})
    ENDFOREACH( test )
    ELSE(EXISTS ${IMAGE_FULL_PATH})
      MESSAGE("!!!!! No such file: ${IMAGE_FULL_PATH} !!!!!")
    ENDIF(EXISTS ${IMAGE_FULL_PATH})
  ENDFOREACH( image )

  SET(MOC_H_FILES ${OLD_MOC_H_FILES})
ENDIF(BUILD_TESTING AND MODULE_IS_ENABLED)

ENDMACRO(MITK_CREATE_MODULE_TESTS)

