cmake_minimum_required(VERSION 3.15)

project(Localedit LANGUAGES CXX)

# TODO: WinExtras is only required on Windows, remove for other platforms
find_package(Qt5 COMPONENTS Core Gui Widgets WinExtras REQUIRED)

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

if(WIN32)
	# Retrieve the absolute path to qmake and then use that path to find
	# the binaries
	get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
	get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
	find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
	find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

	# Bundle the DLLs with the compiled executable
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E
			env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
				"$<TARGET_FILE:${PROJECT_NAME}>"
		COMMENT "Running windeployqt..."
	)
ELSE()
# TODO: Platform independent deployment?
# Deploy on mac
# add_custom_command(TARGET myapp POST_BUILD
#     COMMAND "${MACDEPLOYQT_EXECUTABLE}"
#         "$<TARGET_FILE_DIR:myapp>/../.."
#         -always-overwrite
#     COMMENT "Running macdeployqt..."
# )
ENDIF()