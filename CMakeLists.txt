cmake_minimum_required(VERSION 3.25)

project(cop290_assignment_1 LANGUAGES CXX) # Instantiat the project while ensurig C++ is there and upto standards (17 or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)  # Qt helper libraries -> Automoc for the events and slots (I/O), 
set(CMAKE_AUTOUIC ON) #AutoUIC for turning UI files into C++ code.

find_package(Qt6 COMPONENTS Widgets) # Its efficient and privat only so no BT.
set(SOURCES
  main.cpp
  ui/main/mainwidget.cpp
  ui/main/mainwidget_file.cpp
  ui/main/mainwidget.h
  ui/panels/colourpanel.cpp
  ui/panels/colourpanel.h
  ui/panels/controlpanel.cpp
  ui/panels/controlpanel.h
  ui/panels/shapepanel.cpp
  ui/panels/shapepanel.h
  ui/menus/floatingmenu.cpp
  ui/menus/floatingmenu.h

  canvas/core/mycanvas.h
  canvas/core/mycanvas_shapes.cpp
  canvas/core/mycanvas_render.cpp
  canvas/input/mycanvas_mousepress.cpp
  canvas/input/mycanvas_mousemove.cpp
  canvas/input/mycanvas_mouserelease.cpp
  canvas/input/mycanvas_clipboard.cpp
  canvas/undo/undoredo.h
  canvas/undo/undoredo.cpp

  shapes/base/shape.cpp
  shapes/base/shape_resize.cpp
  shapes/base/shape.h
  shapes/uniform/square.cpp
  shapes/uniform/square.h
  shapes/uniform/circle.cpp
  shapes/uniform/circle.h
  shapes/uniform/hexagon.cpp
  shapes/uniform/hexagon.h
  shapes/rectangular/rectangle.cpp
  shapes/rectangular/rectangle.h
  shapes/rectangular/rounded_rectangle.cpp
  shapes/rectangular/rounded_rectangle.h
  shapes/rectangular/text_box.cpp
  shapes/rectangular/text_box.h
  shapes/freeform/line.cpp
  shapes/freeform/line.h
  shapes/freeform/polyline.h
  shapes/freeform/polyline.cpp
  shapes/shapes.h

  svg/parser/Svg2Data.h
  svg/parser/Svg2Data_File.cpp
  svg/parser/Svg2Data_Xml.cpp
  svg/import/Data2Vec.h
  svg/import/Data2Vec.cpp
  svg/import/Data2Shape.h
  svg/import/Data2Shape.cpp
  svg/export/Shape2Data.h
  svg/export/Shape2Data.cpp
  svg/export/Vec2Svg.h
  svg/export/Vec2Svg.cpp
  svg/registry/shape_registry.h
  svg/registry/shape_registry.cpp
  svg/registry/shape_loaders_basic.cpp
  svg/registry/shape_loaders_complex.cpp

) # All the files in my project
set(QT_LIBS Qt6::Widgets) # All the Qt files i use

add_executable(cop290_assignment_1 ${SOURCES}) # Creates an executable target named "cop290_assignment_1" that builds the specified source files.
target_link_libraries(cop290_assignment_1 PRIVATE ${QT_LIBS}) # Qt is an "Internal dependency" -> user still needs qt on system to run the functions, but it isn't searched and checked by the user until unless it is required in their project -> only my .cpp files will interact with users Qt, not user files -> also naming clashes etc ki bt rehti
target_include_directories(cop290_assignment_1 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/config
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/main
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/panels
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/menus
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas/core
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas/input
  ${CMAKE_CURRENT_SOURCE_DIR}/canvas/undo
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes/base
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes/uniform
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes/rectangular
  ${CMAKE_CURRENT_SOURCE_DIR}/shapes/freeform
  ${CMAKE_CURRENT_SOURCE_DIR}/svg
  ${CMAKE_CURRENT_SOURCE_DIR}/svg/parser
  ${CMAKE_CURRENT_SOURCE_DIR}/svg/import
  ${CMAKE_CURRENT_SOURCE_DIR}/svg/export
  ${CMAKE_CURRENT_SOURCE_DIR}/svg/registry
)
