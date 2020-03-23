#include "MainWindow.h"

MainWindow::MainWindow() {
	auto *centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);

	missionListWidget = new MissionListWidget();
	// previewBoxWidget = new PreviewBoxWidget();
	stagesEditorWidget = new StagesEditorWidget(missionListWidget);

	// Connect list to preview
	// connect(missionListWidget->missionList, &QListWidget::currentItemChanged, previewBoxWidget, &PreviewBoxWidget::setMissionName);

	// Preview box widget mustn't stretch!
	auto *centralLayout = new QGridLayout(centralWidget);
	centralLayout->addWidget(missionListWidget, 0, 0, 0, 1);
	// centralLayout->addWidget(previewBoxWidget, 0, 1);
	centralLayout->addWidget(stagesEditorWidget, 1, 1);
	centralLayout->setColumnStretch(0, 2);
	centralLayout->setRowStretch(0, 0);
	centralLayout->setColumnStretch(1, 5);

	statusBarWidget = new QStatusBar();
	this->setStatusBar(statusBarWidget);

	this->setWindowTitle("localedit");
}
