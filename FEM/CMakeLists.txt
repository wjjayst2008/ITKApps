PROJECT(FEM)

IF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )

  FIND_PACKAGE(ITK)
  IF(ITK_FOUND)
    INCLUDE(${ITK_USE_FILE})
  ELSE(ITK_FOUND)
    MESSAGE(FATAL_ERROR
            "Cannot build ITKApps without ITK.  Please set ITK_DIR.")
  ENDIF(ITK_FOUND)

ENDIF( BUILD_OUTSIDE_INSIGHT_APPLICATIONS )


# Create project files for all examples
ADD_EXECUTABLE(FEMRead FEMRead.cxx)
INSTALL_TARGETS(/bin FEMRead)
TARGET_LINK_LIBRARIES (FEMRead ${ITK_LIBRARIES})


ADD_EXECUTABLE(FEMTruss FEMTruss.cxx)
INSTALL_TARGETS(/bin FEMTruss)
TARGET_LINK_LIBRARIES (FEMTruss ${ITK_LIBRARIES})


ADD_EXECUTABLE(FEMImageRegLMEx FEMImageRegLMEx.cxx)
INSTALL_TARGETS(/bin FEMImageRegLMEx)
TARGET_LINK_LIBRARIES (FEMImageRegLMEx ${ITK_LIBRARIES})


ADD_EXECUTABLE(FEMSolverHyperbolicExample FEMSolverHyperbolicExample.cxx)
INSTALL_TARGETS(/bin FEMSolverHyperbolicExample)
TARGET_LINK_LIBRARIES (FEMSolverHyperbolicExample ${ITK_LIBRARIES})

ADD_EXECUTABLE(FEMInputFileGenerator FEMInputFileGenerator.cxx)
INSTALL_TARGETS(/bin FEMInputFileGenerator)
TARGET_LINK_LIBRARIES (FEMInputFileGenerator ${ITK_LIBRARIES})

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES (FEMSolverHyperbolicExample.cxx PROPERTIES COMPILE_FLAGS -Wno-unused)
  SET_SOURCE_FILES_PROPERTIES (FEMTruss.cxx PROPERTIES COMPILE_FLAGS -Wno-unused)
  SET_SOURCE_FILES_PROPERTIES (FEMRead.cxx PROPERTIES COMPILE_FLAGS -Wno-unused)
  SET_SOURCE_FILES_PROPERTIES (FEMImageRegLMEx.cxx PROPERTIES COMPILE_FLAGS -Wno-unused)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
