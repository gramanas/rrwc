cmake_minimum_required (VERSION 3.9.4)
project(rrwc)
set (CMAKE_CXX_STANDARD 11)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
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
  ${PROJECT_SOURCE_DIR}/engine/outputthread.cpp
  ${PROJECT_SOURCE_DIR}/engine/outputengine.cpp
  ${PROJECT_SOURCE_DIR}/engine/threadmanager.cpp
  ${PROJECT_SOURCE_DIR}/engine/resizer.cpp
  ${PROJECT_SOURCE_DIR}/engine/marker.cpp
  ${PROJECT_SOURCE_DIR}/engine/renamer.cpp
  ${PROJECT_SOURCE_DIR}/profile/profileparser.cpp
  ${PROJECT_SOURCE_DIR}/exif/exifmanager.cpp
  ${PROJECT_SOURCE_DIR}/error/errormanager.cpp
  ${PROJECT_SOURCE_DIR}/output/entrylist.cpp
  ${PROJECT_SOURCE_DIR}/logger.cpp
  ${PROJECT_SOURCE_DIR}/cliparser.cpp
  ${PROJECT_SOURCE_DIR}/tui.cpp
)
set(rrwcLib_hdr
  ${PROJECT_SOURCE_DIR}/gui/mainwindow.hpp
  ${PROJECT_SOURCE_DIR}/gui/outputtab.hpp
  ${PROJECT_SOURCE_DIR}/output/output.hpp
  ${PROJECT_SOURCE_DIR}/engine/outputthread.hpp
  ${PROJECT_SOURCE_DIR}/rrwc.hpp
  ${PROJECT_SOURCE_DIR}/engine/outputengine.hpp
  ${PROJECT_SOURCE_DIR}/engine/threadmanager.hpp
  ${PROJECT_SOURCE_DIR}/engine/resizer.hpp
  ${PROJECT_SOURCE_DIR}/engine/marker.hpp
  ${PROJECT_SOURCE_DIR}/engine/renamer.hpp
  ${PROJECT_SOURCE_DIR}/profile/profileparser.hpp
  ${PROJECT_SOURCE_DIR}/exif/exifmanager.hpp
  ${PROJECT_SOURCE_DIR}/error/errormanager.hpp
  ${PROJECT_SOURCE_DIR}/globals.hpp
  ${PROJECT_SOURCE_DIR}/engine/worker.hpp
  ${PROJECT_SOURCE_DIR}/output/entrylist.hpp
  ${PROJECT_SOURCE_DIR}/logger.hpp
  ${PROJECT_SOURCE_DIR}/cliparser.hpp
  ${PROJECT_SOURCE_DIR}/tui.hpp
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
