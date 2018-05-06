cmake_minimum_required(VERSION 2.8)
#project( libcproc_project )
project( cproc )
add_library(cproc STATIC CProc.cpp)
find_package( OpenCV REQUIRED )

add_library(cproclib STATIC IMPORTED) # or STATIC instead of SHARED
set_target_properties(cproclib PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libcproc.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}"
)

add_executable( ctest CTest.cpp )
target_link_libraries( ctest ${OpenCV_LIBS} cproclib )
