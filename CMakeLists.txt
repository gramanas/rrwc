cmake_minimum_required (VERSION 3.9.4)
project(rrwc)
set (CMAKE_CXX_STANDARD 11)

# Find external libraries
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/exiv2)
find_package(Qt5Widgets)
find_package(OpenCV REQUIRED) # COMPONENTS core imgproc highgui
find_package(Exiv2 REQUIRED)

# Set project directories
set(PROJECT_SOURCE_DIR ./src)
set(PROJECT_GUI_DIR ./gui)

# Set AutoUic search path
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_GUI_DIR})

# Set source code locations
set(rrwcBin_src ${PROJECT_SOURCE_DIR}/main.cpp)
set(rrwcLib_src
  ${PROJECT_SOURCE_DIR}/gui/mainwindow.cpp
  ${PROJECT_SOURCE_DIR}/gui/outputtab.cpp
  ${PROJECT_SOURCE_DIR}/output/outputmanager.cpp
  ${PROJECT_SOURCE_DIR}/rrwc.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/outputthread.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/outputengine.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/enginemanager.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/resizer.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/marker.cpp
  ${PROJECT_SOURCE_DIR}/output/engine/renamer.cpp
  ${PROJECT_SOURCE_DIR}/profile/profileparser.cpp
  ${PROJECT_SOURCE_DIR}/exif/exifmanager.cpp
  ${PROJECT_SOURCE_DIR}/error/errormanager.cpp
  ${PROJECT_SOURCE_DIR}/output/entrylist.cpp
)
set(rrwcLib_hdr
  ${PROJECT_SOURCE_DIR}/gui/mainwindow.hpp
  ${PROJECT_SOURCE_DIR}/gui/outputtab.hpp
  ${PROJECT_SOURCE_DIR}/output/output.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/outputthread.hpp
  ${PROJECT_SOURCE_DIR}/rrwc.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/outputengine.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/enginemanager.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/resizer.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/marker.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/renamer.hpp
  ${PROJECT_SOURCE_DIR}/profile/profileparser.hpp
  ${PROJECT_SOURCE_DIR}/exif/exifmanager.hpp
  ${PROJECT_SOURCE_DIR}/error/errormanager.hpp
  ${PROJECT_SOURCE_DIR}/globals.hpp
  ${PROJECT_SOURCE_DIR}/output/engine/worker.hpp
  ${PROJECT_SOURCE_DIR}/output/entrylist.hpp
)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_GUI_DIR})

# Run automated QT tools 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Create the shared library
add_library (rrwcLib SHARED 
    ${rrwcLib_src}
    ${rrwcLib_hdr}
)

# Link
target_link_libraries (rrwcLib Qt5::Widgets)
add_executable(rrwc ${rrwcBin_src})
target_link_libraries (rrwcLib exiv2)
target_link_libraries(rrwc ${OpenCV_LIBS})
target_link_libraries (rrwc rrwcLib)
