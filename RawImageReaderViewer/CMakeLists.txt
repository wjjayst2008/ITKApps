INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)

INCLUDE_DIRECTORIES(
${ITK_SOURCE_DIR}/Code/Common
${ITK_SOURCE_DIR}/Code/BasicFilters
${ITK_SOURCE_DIR}/Code/Algorithms
${ITK_SOURCE_DIR}/Code/IO
${ITK_SOURCE_DIR}/Auxiliary/MetaImage/IView
${ITK_SOURCE_DIR}/Auxiliary/MetaImage
${ITK_SOURCE_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_SOURCE_DIR}/Examples/RawImageReaderViewer
${FLTK_INCLUDE_PATH}
${OPENGL_INCLUDE_PATH}
)

LINK_DIRECTORIES(
${ITK_BINARY_DIR}/Code/Common
${ITK_BINARY_DIR}/Code/BasicFilters
${ITK_BINARY_DIR}/Code/Algorithms
${ITK_BINARY_DIR}/Code/IO
${ITK_BINARY_DIR}/Auxiliary/MetaImage
${ITK_BINARY_DIR}/Auxiliary/FltkImageViewer
${ITK_BINARY_DIR}/Auxiliary/MetaImage/IView
${ITK_BINARY_DIR}/Examples/RawImageReaderViewer
)

LINK_LIBRARIES (
ITKCommon
ITKBasicFilters 
ITKIO
IView
itkpng
itkzlib
FltkImageViewer
${FLTK_LIBRARY}
${OPENGL_LIBRARY}
${GLU_LIBRARY}
)

IF(WIN32)
  LINK_LIBRARIES(
  wsock32
  )
ENDIF(WIN32)

IF(UNIX)
  LINK_LIBRARIES (
  ${CMAKE_X_LIBS} -lm
  )
ENDIF(UNIX)


SOURCE_FILES(RawImageReaderViewer_SRCS
itkRawImageReaderViewer
RawImageReaderViewer
)

SOURCE_FILES(RawImageReaderViewer_GUI_SRCS
itkRawImageReaderViewerGUI.fl
)

ADD_EXECUTABLE(RawImageReaderViewer   RawImageReaderViewer_SRCS)

FLTK_WRAP_UI(RawImageReaderViewer   RawImageReaderViewer_GUI_SRCS)

