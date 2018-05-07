cmake_minimum_required(VERSION 2.8)
project( cproc )
add_library(imgutils STATIC ImgUtils.cpp findRegion.cpp findPerimeter.cpp findSmoothPerimeter.cpp polyfit.cpp)
find_package( OpenCV REQUIRED )

add_library(imgutilslib STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(imgutilslib PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/build/libimgutils.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

add_executable( ctest CTest.cpp )
target_link_libraries( ctest ${OpenCV_LIBS} imgutilslib )
