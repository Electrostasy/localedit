cmake_minimum_required(VERSION 3.15)
project(apb_config)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES main.cpp gui/MainWindow.h gui/MainWindow.cpp)

# find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# find_package(Qt5Gui REQUIRED)

add_executable(apb_config ${SOURCE_FILES})

# Link Qt libraries to the executable
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)