cmake_minimum_required(VERSION 3.21)

project(DBN)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Core Widgets REQUIRED)
find_path(EXPRTK_INCLUDE_DIRS "exprtk.hpp")

include_directories("MainWindow")
include_directories("Variables")
include_directories("WindowEdit")
include_directories("WindowExport")
include_directories("WindowOpen")

add_executable(DBN
				main.cpp
				MainWindow/mainwindow.cpp
				Variables/var.cpp
				WindowEdit/window_edit.cpp
				WindowExport/window_export.cpp
				WindowOpen/window_open.cpp
)

target_include_directories(DBN PRIVATE ${EXPRTK_INCLUDE_DIRS})
target_link_libraries(DBN PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

set_target_properties(DBN PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
