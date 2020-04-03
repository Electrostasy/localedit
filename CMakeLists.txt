cmake_minimum_required(VERSION 3.15)
project(apb_config)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES main.cpp
        gui/MainWindow.h
        gui/MainWindow.cpp
        gui/MissionList/MissionListItem.cpp
        gui/MissionList/MissionListItem.h
        gui/MissionList/MissionListWidget.cpp
        gui/MissionList/MissionListWidget.h
        gui/PreviewBoxWidget.cpp
        gui/PreviewBoxWidget.h
        gui/StagesEditor/SyntaxHighlighter.cpp
        gui/StagesEditor/SyntaxHighlighter.h
        gui/StagesEditor/StagesEditorWidget.cpp
        gui/StagesEditor/StagesEditorWidget.h
        gui/StagesEditor/TabWidget/ExpandingTabBar.cpp
        gui/StagesEditor/TabWidget/ExpandingTabBar.h
        gui/StagesEditor/TabWidget/ExpandingTabWidget.cpp
        gui/StagesEditor/TabWidget/ExpandingTabWidget.h
        gui/Utility/LayoutUtils.cpp
        gui/Utility/LayoutUtils.h
        resources.qrc gui/StagesEditor/ExpandedTextEdit.cpp gui/StagesEditor/ExpandedTextEdit.h gui/fileDialog.cpp gui/fileDialog.h gui/StagesEditor/FramedPage.cpp gui/StagesEditor/FramedPage.h)

# find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# find_package(Qt5Gui REQUIRED)

add_executable(apb_config ${SOURCE_FILES})

# Link Qt libraries to the executable
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
