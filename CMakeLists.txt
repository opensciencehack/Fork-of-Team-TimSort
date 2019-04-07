cmake_minimum_required(VERSION 3.13)
project(OSH)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include_directories (include)

add_executable (OSH main.cpp include/BBox.hpp include/Polygon.hpp include/Vertice.hpp)
add_executable (Pumper pumper.cpp src/poly_extract.cpp src/differ.cpp src/cats_reduce.cpp include/cats_reduce.hpp include/Polygon.hpp include/Vertice.hpp)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules" ${CMAKE_MODULE_PATH})
find_package (SFML 2 REQUIRED graphics window system network)
find_package (OpenCV 2 REQUIRED)


if (SFML_FOUND AND OPENCV_FOUND)
    include_directories (${SFML_INCLUDE_DIR})
    target_link_libraries (OSH ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif ()

set (PY_VERSION 3.7)
message ("PyVer: ${PY_VERSION}")
find_package (PythonLibs ${PY_VERSION} REQUIRED)
find_package (Boost 1.64.0 REQUIRED COMPONENTS system python)

# include directories
include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PYTHON_INCLUDE_DIRS})
include_directories (${Boost_INCLUDE_DIRS})

# create the lib
add_library (pyExDiff SHARED include/py_lnk.hpp src/pywrap.cpp src/poly_extract.cpp src/differ.cpp)

# link
target_link_libraries (pyExDiff ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENCV_LIBRARIES} python3.7m boost_python37)

# Copy the __init__.py file
#configure_file (__init__.py ${CMAKE_CURRENT_BINARY_DIR}/src/__init__.py COPYONLY)

# Suppress prefix "lib" because Python does not allow this prefix
set_target_properties (pyExDiff PROPERTIES PREFIX "")

#install (TARGETS pyExDiff
#        RUNTIME DESTINATION "${PYTHON_INSTALL_PATH}"
#        LIBRARY DESTINATION "${PYTHON_INSTALL_PATH}"
#        ARCHIVE DESTINATION "${PYTHON_INSTALL_PATH}")
install (FILES __init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src/)

message ("PIP: ${PYTHON_INSTALL_PATH}")