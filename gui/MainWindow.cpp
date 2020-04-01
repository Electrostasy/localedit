#include "MainWindow.h"

MainWindow::MainWindow() {
	auto *centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);

	// Initialize mission list
	search = new QLineEdit();
	search->setPlaceholderText("Search missions...");
	missions = new MissionListWidget();
	auto *missionsLayout = new QVBoxLayout();
	missionsLayout->addWidget(search);
	missionsLayout->addWidget(missions);

	connect(search, &QLineEdit::textChanged, this, &MainWindow::searchMissionList);
	// connect(missions, &QListWidget::currentItemChanged, this, &MainWindow::updateMissionInTitle);

	// Initialize stages editor
	stages = new StagesEditorWidget();

	connect(missions, &QListWidget::currentItemChanged, this, [=](QListWidgetItem *currentItem) {
		// Upcasting from MissionListItem* to QListWidgetItem* on item addition to QListWidget*
		// Downcasting from QListWidgetItem* to MissionListItem* here on currentItemChanged
		stages->showStages(dynamic_cast<MissionListItem *>(currentItem));
	});

	auto *centralLayout = new QGridLayout(centralWidget);
	centralLayout->addLayout(missionsLayout, 0, 0, 0, 1);
	centralLayout->addWidget(stages, 0, 1);
	centralLayout->setColumnStretch(0, 2);
	centralLayout->setColumnStretch(1, 5);

	status = new QStatusBar();
	this->setStatusBar(status);
	this->setWindowTitle(applicationName);

	initMenusActions();
}

// Slots
void MainWindow::searchMissionList(const QString &filter) {
	for(int i = 0; i < missions->count(); ++i) {
		auto *mission = dynamic_cast<MissionListItem *>(missions->item(i));

		mission->setHidden(!mission->text().contains(filter, Qt::CaseInsensitive));
	}
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
	dialog.setNameFilter("Localization Files (*.INT *.GER *.ITA *.RUS *.SPA *.FRA *.BRA)");
	dialog.setViewMode(QFileDialog::ViewMode::List);

	if(dialog.exec()) {
		missions->clear();

		QFile missionTemplates;
		QFile taskObjectives;
		QStringList fileNames = dialog.selectedFiles();
		foreach(const QString &fileName, fileNames) {
			const QString trimmed = verifyAndTrim(fileName);

			if(trimmed == "MissionTemplates") {
				missionTemplates.setFileName(fileName);
				if(!missionTemplates.open(QIODevice::ReadOnly | QIODevice::Text)) {
					return;
				}
			}

			if(trimmed == "TaskObjectives") {
				taskObjectives.setFileName(fileName);
				if(!taskObjectives.open(QIODevice::ReadOnly | QIODevice::Text)) {
					return;
				}
			}
		}

		QRegularExpression IDENTIFIERS("MissionTemplates_(?<code>[A-Za-z0-9_]+)_MissionTitle=(?<title>.*)");
		QRegularExpression STAGE_REGEX("TaskObjectives_(?<code>[A-Za-z0-9_]+)_(?:Stage\\d{2}_)(?<special>Opp)?_?(?<side>\\w+)=(?<text>.*)");

		// TODO: Rework so codes are mapped to Missions instead of Widgets
		QMap<QString, MissionListItem> map;

		QTextStream streamTitles(&missionTemplates);
		QTextStream streamStages(&taskObjectives);
		QString line;
		// Read mission code and title
		while(streamTitles.readLineInto(&line)) {
			if(line.startsWith(';')) {
				continue;
			}

			QString code;
			QRegularExpressionMatch match = IDENTIFIERS.match(line);
			if(match.hasMatch()) {
				code = match.captured("code");

				map[code].setIdentifiers(match.captured("title"), code);
			}

			// Read stages for given code
			while(streamStages.readLineInto(&line)) {
				if(line.startsWith(';')) {
					continue;
				}

				match = STAGE_REGEX.match(line);
				if(match.hasMatch()) {
					QString side = match.captured("side");
					/**
					 * Some missions have an "Opp" tag on some stages, which by default means there's an alternate
					 * version that we can go to for objective text if our stage tagged "Opp" has none.
					 */
					MissionListItem::Stage stage(match.captured("text"), !match.captured("special").isEmpty());

					if(match.captured("code") == code) {
						// Stages belong to mission of code
						if(side == "OwnerBrief") {
							map[code].pushOwnerStage(stage);
						}

						if(side == "DispatchBrief") {
							map[code].pushDispatchStage(stage);
						}
					} else {
						// It's a new mission
						if(side == "OwnerBrief") {
							map[match.captured("code")].pushOwnerStage(stage);
						}

						if(side == "DispatchBrief") {
							map[match.captured("code")].pushDispatchStage(stage);
						}

						// Break out to get new mission title for new code
						break;
					}
				}
			}
		}

		missionTemplates.close();
		taskObjectives.close();

		// Build list item widgets
		for(auto &value: map.values()) {
			auto *item = new MissionListItem(value.name(), value.code(), value.ownerStages(), value.dispatchStages());
			item->setText(value.name());

			this->missions->addMission(item);
		}
	}
}

QString MainWindow::verifyAndTrim(const QString &fileName) {
	QRegularExpression rx("(?:BRA|FRA|GER|INT|ITA|RUS|SPA)\\/(?<fileName>\\w+)");
	QRegularExpressionMatch match = rx.match(fileName);

	return match.captured("fileName");
}
