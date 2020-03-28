#include "MainWindow.h"

MainWindow::MainWindow() {
	auto *centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);

	missionListWidget = new MissionListWidget();
	previewBoxWidget = new PreviewBoxWidget();
	stagesEditorWidget = new StagesEditorWidget(missionListWidget);

	// Preview box widget mustn't stretch!
	auto *centralLayout = new QGridLayout(centralWidget);
	centralLayout->addWidget(missionListWidget, 0, 0, 0, 1);
	centralLayout->addWidget(previewBoxWidget, 0, 1);
	centralLayout->addWidget(stagesEditorWidget, 1, 1);
	centralLayout->setColumnStretch(0, 2);
	centralLayout->setRowStretch(0, 0);
	centralLayout->setColumnStretch(1, 5);

	statusBarWidget = new QStatusBar();
	this->setStatusBar(statusBarWidget);
	this->setWindowTitle(applicationName);

	// Connect list to preview
	connect(missionListWidget->missionList, &QTreeWidget::currentItemChanged, this, &MainWindow::updateMissionInTitle);

	initMenusActions();
}

void MainWindow::updateMissionInTitle(QTreeWidgetItem *selectedItem) {
	this->selectedMission = selectedItem->text(0);
	updateTitle();
}

void MainWindow::updateTitle() {
	// TODO: Touching how title is created apparently breaks loading of missions
	QString title = applicationName + " - " + selectedMission;

	// If a mission has pending changes, append star
	if(hasPendingChanges && !title.endsWith("(*)")) {
		title += " (*)";
	}

	this->setWindowTitle(title);
}

void MainWindow::initMenusActions() {
	// File menu actions
	fileMenu = menuBar()->addMenu("File");
	importAction = new QAction("Import");
	importAction->setStatusTip("Import localization files from APB Reloaded");
	connect(importAction, &QAction::triggered, this, &MainWindow::importFiles);
	fileMenu->addAction(importAction);
}

void MainWindow::importFiles() {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::FileMode::ExistingFiles);
	dialog.setNameFilter("Localisation Files (*.INT *.GER *.ITA *.RUS *.SPA *.FRA *.BRA)");
	dialog.setViewMode(QFileDialog::ViewMode::List);

	QStringList files;
	if(dialog.exec()) {
		files = dialog.selectedFiles();

		missionListWidget->missionList->clear();
		missionListWidget->missionMap.clear();

		foreach(const QString &filename, files) {
			QFile file(filename);
			if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				return;
			}

			QTextStream namesStream(&file);
			while(!namesStream.atEnd()) {
				QString line = namesStream.readLine();
				MissionListWidget::Mission mission;

				if(line.startsWith("MissionTemplates_")) {
					missionListWidget->parseMissionTemplates(mission, line);
				}

				if(line.startsWith("TaskObjectives_")) {
					missionListWidget->parseTaskObjectives(mission, line);
				}
			}
			file.close();
		}
	}
}



