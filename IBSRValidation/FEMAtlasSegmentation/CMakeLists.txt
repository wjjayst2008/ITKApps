
SET(FEMBrainStripApp_SRCS
  Code/FEMBrainStripValidationApp.cxx
#  Code/
)

# Header and template source files for FEM project
# we can only include them if compiling on windows
IF(NOT UNIX)
IF(WIN32)
SET(FEMBrainStripApp_SRCS ${FEMBrainStripApp_SRCS}
  Code/FEMValidationInputParser.h
  Code/FEMValidationInputParser.txx
  Code/FEMPreprocessor.h
  Code/FEMPreprocessor.txx
  Code/FEMRegistrator.h
  Code/FEMRegistrator.txx
  Code/FEMAtlasLabeler.h
  Code/FEMAtlasLabeler.txx
  Code/FEMBrainStripValidationApp.h
  Code/FEMBrainStripValidationApp.txx
)
ENDIF(WIN32)
ENDIF(NOT UNIX)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Examples/IBSRValidation/Common
${ITK_SOURCE_DIR}/Examples/IBSRValidation/FEMAtlasSegmentation
)

ADD_EXECUTABLE(FEMBrainStripValidationApp ${FEMBrainStripApp_SRCS} )
TARGET_LINK_LIBRARIES(FEMBrainStripValidationApp ITKBasicFilters ITKNumerics FEM ITKIO)


