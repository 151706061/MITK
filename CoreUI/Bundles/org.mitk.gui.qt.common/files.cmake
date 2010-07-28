SET(SRC_CPP_FILES
  QmitkCloseProjectAction.cpp
  QmitkDnDFrameWidget.cpp
  QmitkDataNodeSelectionProvider.cpp
  QmitkFileOpenAction.cpp
  QmitkFileExitAction.cpp
  QmitkFunctionality.cpp
  QmitkStatusBar.cpp
  QmitkStdMultiWidgetEditor.cpp
  QmitkGeneralPreferencePage.cpp
  QmitkPreferencesDialog.cpp
  QmitkFunctionalityCoordinator.cpp
)

SET(INTERNAL_CPP_FILES
  QmitkDataNodeSelection.cpp
  QmitkCommonActivator.cpp
)

SET(MOC_H_FILES
  src/QmitkCloseProjectAction.h
  src/QmitkFileOpenAction.h
  src/QmitkFileExitAction.h
  src/QmitkPreferencesDialog.h
  src/QmitkDnDFrameWidget.h
)

# todo: add some qt style sheet resources
SET(RES_FILES
  resources/resources.qrc
)

SET(CPP_FILES manifest.cpp)

foreach(file ${SRC_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  SET(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})
