project(Workbench)

set(_app_options)
if(MITK_SHOW_CONSOLE_WINDOW)
  list(APPEND _app_options SHOW_CONSOLE)
endif()

# Create a cache entry for the provisioning file which is used to export
# the file name in the MITKConfig.cmake file. This will keep external projects
# which rely on this file happy.
set(MITK_EXTAPP_PROVISIONING_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MitkWorkbench.provisioning" CACHE INTERNAL "MitkWorkbench provisioning file" FORCE)

# Plug-ins listed below will not be
#  - added as a build-time dependency to the executable
#  - listed in the provisioning file for the executable
#  - installed if they are external plug-ins

set(_exclude_plugins
  org.blueberry.test
  org.blueberry.uitest
  org.mitk.gui.qt.coreapplication
  org.mitk.gui.qt.diffusionimagingapp
)

FunctionCreateBlueBerryApplication(
  NAME MitkWorkbench
  DESCRIPTION "MITK Workbench"
  EXCLUDE_PLUGINS ${_exclude_plugins}
  ${_app_options}
)
mitk_use_modules(TARGET MitkWorkbench MODULES qtsingleapplication MitkCore)

# Add meta dependencies (e.g. on auto-load modules from depending modules)
if(ALL_META_DEPENDENCIES)
  add_dependencies(MitkWorkbench ${ALL_META_DEPENDENCIES})
endif()

#Setting application icon for mac os x systems
set_target_properties(MitkWorkbench PROPERTIES MACOSX_BUNDLE_ICON_FILE "icon.icns")

if(APPLE)
  install(FILES "icons/icon.icns" DESTINATION "MitkWorkbench.app/Contents/Resources")
endif(APPLE)

# Add a build time dependency to legacy BlueBerry bundles.
if(MITK_MODULES_ENABLED_PLUGINS)
  add_dependencies(MitkWorkbench ${MITK_MODULES_ENABLED_PLUGINS})
endif()