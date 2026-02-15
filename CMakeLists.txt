cmake_minimum_required(VERSION 3.16)

project(cop290_assignment_1 LANGUAGES CXX) # Instantiat the project while ensurig C++ is there and upto standards (17 or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)  # Qt helper libraries -> Automoc for the events and slots (I/O), 
set(CMAKE_AUTOUIC ON) #AutoUIC for turning UI files into C++ code.

find_package(Qt6 COMPONENTS Widgets) # Its efficient and privat only so no BT.
set(SOURCES
  main.cpp
  mainwidget.cpp
  mainwidget.h
  mycanvas.cpp
  mycanvas.h
  colourpanel.cpp
  colourpanel.h
  svg_parser.cpp
  svg_parser.h
  svg_convert.cpp
  svg_convert.h
  shapepanel.h
  ShapePanel.cpp
  shape_loaders.cpp
  shapes.h
) # All the files in my project
set(QT_LIBS Qt6::Widgets) # All the Qt files i use

add_executable(cop290_assignment_1 ${SOURCES}) # Creates an executable target named "cop290_assignment_1" that builds the specified source files.
target_link_libraries(cop290_assignment_1 PRIVATE ${QT_LIBS}) # Qt is an "Internal dependency" -> user still needs qt on system to run the functions, but it isn't searched and checked by the user until unless it is required in their project -> only my .cpp files will interact with users Qt, not user files -> also naming clashes etc ki bt rehti
