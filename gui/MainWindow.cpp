#include "MainWindow.h"

MainWindow::MainWindow() {
	auto *centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);

	missionListWidget = new MissionListWidget();
	stagesEditorWidget = new StagesEditorWidget(missionListWidget);

	auto *centralLayout = new QGridLayout(centralWidget);
	centralLayout->addWidget(missionListWidget, 0, 0);
	centralLayout->addWidget(stagesEditorWidget, 0, 1);
	centralLayout->setColumnStretch(0, 2);
	centralLayout->setColumnStretch(1, 5);

	statusBarWidget = new QStatusBar();
	this->setStatusBar(statusBarWidget);

	this->setWindowTitle("localedit");
}
