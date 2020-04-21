cmake_minimum_required(VERSION 3.15)

project(Localedit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/APBFramelessWindow/APBFramelessWindowTitleBar.h
        src/APBFramelessWindow/APBFramelessWindowTitleBar.cpp
    src/APBFramelessWindow/APBPushButton.cpp
    src/APBFramelessWindow/APBPushButton.h
    src/MissionList/MissionListItem.cpp
    src/MissionList/MissionListWidget.cpp
    src/MissionList/MissionListWidget.h
    # src/PreviewBoxWidget.cpp
    # src/PreviewBoxWidget.h
    src/StagesEditor/StagesEditorWidget.cpp
    src/StagesEditor/StagesEditorWidget.h
    src/StagesEditor/ExpandedTextEdit.cpp
    src/StagesEditor/ExpandedTextEdit.h
    src/StagesEditor/TabWidget/FramedPage.cpp
    src/StagesEditor/TabWidget/FramedPage.h
    src/StagesEditor/SyntaxHighlighter.cpp
    src/StagesEditor/SyntaxHighlighter.h
    src/StagesEditor/TabWidget/ExpandingTabBar.cpp
    src/StagesEditor/TabWidget/ExpandingTabBar.h
    src/StagesEditor/TabWidget/ExpandingTabWidget.cpp
    src/StagesEditor/TabWidget/ExpandingTabWidget.h
    src/Utility/LayoutUtils.cpp
    src/Utility/LayoutUtils.h
    src/resources.qrc
        src/APBFramelessWindow/NativeTranslucentFramelessWindow.cpp src/APBFramelessWindow/NativeTranslucentFramelessWindow.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::WinExtras Dwmapi)
