cmake_minimum_required (VERSION 3.9.4)
project(rrwc)

# Find external libraries
find_package(Qt5Widgets)
find_package(OpenCV REQUIRED) # COMPONENTS core imgproc highgui

# Set project directories
set(PROJECT_SOURCE_DIR ./src)
set(PROJECT_GUI_DIR ./gui)

# Set AutoUic search path
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_GUI_DIR})

# Set source code locations
set(rrwcBin_src ${PROJECT_SOURCE_DIR}/main.cpp)
set(rrwcLib_src
  ${PROJECT_SOURCE_DIR}/person.cpp
  ${PROJECT_SOURCE_DIR}/mainwindow.cpp
  ${PROJECT_SOURCE_DIR}/outputtab.cpp
  ${PROJECT_SOURCE_DIR}/outputmanager.cpp
)
set(rrwcLib_hdr
  ${PROJECT_SOURCE_DIR}/person.hpp
  ${PROJECT_SOURCE_DIR}/mainwindow.hpp
  ${PROJECT_SOURCE_DIR}/outputtab.hpp
  ${PROJECT_SOURCE_DIR}/outputmanager.hpp
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
target_link_libraries(rrwc ${OpenCV_LIBS})
target_link_libraries (rrwc rrwcLib)
