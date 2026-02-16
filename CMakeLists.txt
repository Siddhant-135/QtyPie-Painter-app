cmake_minimum_required(VERSION 3.16)

project(cop290_assignment_1 LANGUAGES CXX) # Instantiat the project while ensurig C++ is there and upto standards (17 or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)  # Qt helper libraries -> Automoc for the events and slots (I/O), 
set(CMAKE_AUTOUIC ON) #AutoUIC for turning UI files into C++ code.

find_package(Qt6 COMPONENTS Widgets) # Its efficient and privat only so no BT.
set(SOURCES
  main.cpp
  ui/mainwidget.cpp
  ui/mainwidget.h
  ui/colourpanel.cpp
  ui/colourpanel.h
  ui/controlpanel.cpp
  ui/controlpanel.h
  ui/shapepanel.cpp
  ui/shapepanel.h
  ui/floatingmenu.cpp
  ui/floatingmenu.h

  canvas/mycanvas_draw.cpp
  canvas/mycanvas_mouse.cpp
  canvas/mycanvas_clipboard.cpp
  canvas/mycanvas.h
  canvas/undoredo.h
  canvas/undoredo.cpp

  shapes/base.cpp
  shapes/base_equal_hack.cpp
  shapes/base.h
  shapes/uniform.cpp
  shapes/uniform.h
  shapes/lines.cpp
  shapes/lines.h
  shapes/rectangle.cpp
  shapes/rectangle.h
  shapes/polyline.h
  shapes/polyline.cpp
  shapes/shapes.h

  svg/Svg2Data.h
  svg/Svg2Data_File.cpp
  svg/Svg2Data_Xml.cpp
  svg/Data2Vec.h
  svg/Data2Vec.cpp
  svg/Data2Shape.h
  svg/Data2Shape.cpp
  svg/Shape2Data.h
  svg/Shape2Data.cpp
  svg/Vec2Svg.h
  svg/Vec2Svg.cpp
  svg/shape_registry.h
  svg/shape_registry.cpp
  svg/shape_loaders_1.cpp

) # All the files in my project
set(QT_LIBS Qt6::Widgets) # All the Qt files i use

add_executable(cop290_assignment_1 ${SOURCES}) # Creates an executable target named "cop290_assignment_1" that builds the specified source files.
target_link_libraries(cop290_assignment_1 PRIVATE ${QT_LIBS}) # Qt is an "Internal dependency" -> user still needs qt on system to run the functions, but it isn't searched and checked by the user until unless it is required in their project -> only my .cpp files will interact with users Qt, not user files -> also naming clashes etc ki bt rehti
target_include_directories(cop290_assignment_1 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/ui
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes
  ${CMAKE_CURRENT_SOURCE_DIR}/svg
)
