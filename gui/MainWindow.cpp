#include "MainWindow.h"

MainWindow::MainWindow() {
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

	search = new QLineEdit();
	search->setPlaceholderText("Search missions...");
	missions = new MissionListWidget();

	connect(search, &QLineEdit::textChanged, this, &MainWindow::searchMissionList);
	connect(missions, &QListWidget::currentItemChanged, this, &MainWindow::updateTitle);

	stages = new StagesEditorWidget();

	connect(missions, &QListWidget::currentItemChanged, this, [=](QListWidgetItem *currentItem) {
		// Upcasting from MissionListItem* to QListWidgetItem* on item addition to QListWidget*
		// Downcasting from QListWidgetItem* to MissionListItem* here on currentItemChanged
		auto *item = dynamic_cast<MissionListItem *>(currentItem);
		// If item is nullptr, importing more files with open stages will crash
		if(item != nullptr) {
			stages->buildStages(stages->pageOwner, item->ownerStages(), item);
			stages->buildStages(stages->pageDispatch, item->dispatchStages(), item);
		}
	});

	auto *missionsLayout = new QVBoxLayout();
	missionsLayout->addWidget(search);
	missionsLayout->addWidget(missions);
	missionsLayout->setMargin(0);

	auto splitter = new QSplitter();
	auto missionListContainer = new QWidget();
	missionListContainer->setLayout(missionsLayout);
	splitter->addWidget(missionListContainer);
	splitter->addWidget(stages);
	splitter->setChildrenCollapsible(false);
	splitter->setSizes(QList{ missionListContainer->width() / 3, splitter->width() - missionListContainer->width() / 3 });

	auto splitterContainer = new QWidget();
	splitterContainer->setLayout(new QHBoxLayout());
	splitterContainer->layout()->addWidget(splitter);

	this->setCentralWidget(splitterContainer);
	status = new QStatusBar();
	status->setSizeGripEnabled(false);
	this->setStatusBar(status);
	this->setWindowTitle(applicationName);

	initMenusActions();
}

void MainWindow::searchMissionList(const QString &filter) {
	for(int i = 0; i < missions->count(); ++i) {
		auto *mission = dynamic_cast<MissionListItem *>(missions->item(i));

		mission->setHidden(!mission->text().contains(filter, Qt::CaseInsensitive));
	}
}

void MainWindow::updateTitle() {
	QString title = applicationName;

	auto *currentItem = dynamic_cast<MissionListItem *>(missions->currentItem());
	if(currentItem != nullptr) {
		title += " - " + currentItem->name();
	}

	this->setWindowTitle(title);
}

void MainWindow::initMenusActions() {
	importAction = new QAction("Import");
	importAction->setStatusTip("Select and import localization files to edit");
	connect(importAction, &QAction::triggered, this, &MainWindow::importFiles);
	this->menuBar()->addAction(importAction);

	exportAction = new QAction("Export");
	exportAction->setEnabled(false);
	exportAction->setStatusTip("Export localization files to selected folder");
	connect(exportAction, &QAction::triggered, this, &MainWindow::exportFiles);
	this->menuBar()->addAction(exportAction);
}

void MainWindow::importFiles() {
	if(missions->count() != 0) {
		for(int i = 0; i < missions->count(); ++i) {
			auto *item = missions->item(0);
			if(!item->data(Qt::ItemDataRole::DecorationRole).value<QIcon>().isNull()) {
				QMessageBox unsavedChangesBox;
				unsavedChangesBox.setWindowTitle(this->applicationName);
				unsavedChangesBox.setText("You have unsaved changes.");
				unsavedChangesBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ignore | QMessageBox::Cancel);
				unsavedChangesBox.setDefaultButton(QMessageBox::Cancel);
				int ret = unsavedChangesBox.exec();
				switch(ret) {
					case QMessageBox::Save:
						// Save was clicked
						this->exportFiles();
						return;
					case QMessageBox::Ignore:
						// Ignore was clicked
						this->openImportFilesDialog();
						return;
					case QMessageBox::Cancel:
						// Cancel was clicked
						return;
					default:
						// should never be reached
						break;
				}
			}
		}
	}
	this->openImportFilesDialog();
}

void MainWindow::openImportFilesDialog() {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::FileMode::ExistingFiles);
	dialog.setNameFilter("Localization Files (*.INT *.GER *.ITA *.RUS *.SPA *.FRA *.BRA)");
	dialog.setViewMode(QFileDialog::ViewMode::List);

	if(dialog.exec()) {
		if(missions->count() > 0) {
			missions->clear();
		}

		QFile missionTemplates;
		QFile taskObjectives;
		QStringList fileNames = dialog.selectedFiles();
		for(auto const &fileName: fileNames) {
			// Check if required files are contained in fileNames and trim the path and extension
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

		QMap<QString, MissionListItem *> map;

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

				// If the mission isn't initialized yet, initialize it
				if(map[code] == nullptr) {
					map[code] = new MissionListItem();
				}
				map[code]->setIdentifiers(match.captured("title"), code);
			}

			// Read stages for given code
			while(streamStages.readLineInto(&line)) {
				// Unlikely to encounter comments, TODO: maybe remove if statement
				if(line.startsWith(';')) {
					continue;
				}

				match = STAGE_REGEX.match(line);
				if(match.hasMatch()) {
					QString side = match.captured("side");
					// Some missions have an "Opp" tag on some stages, we flag it as appropriate to avoid displaying it
					// and then during exporting we copy the data from the equivalent non-Opp stage to it
					MissionListItem::Stage stage(
						new QTextDocument(match.captured("text")), !match.captured("special").isEmpty());
					if(match.captured("code") == code) {
						// Ensure we are reading the stages for the mission whose name and code we got earlier
						if(side == "OwnerBrief") {
							map[code]->pushOwnerStage(stage);
						}

						if(side == "DispatchBrief") {
							map[code]->pushDispatchStage(stage);
						}
					} else {
						// In the case we overshoot and start reading data of the next mission, just initialize it
						// and save whatever stage we got from it. On next iteration we continue where we left off on
						if(map[match.captured("code")] == nullptr) {
							map[match.captured("code")] = new MissionListItem();
						}

						if(side == "OwnerBrief") {
							map[match.captured("code")]->pushOwnerStage(stage);
						}

						if(side == "DispatchBrief") {
							map[match.captured("code")]->pushDispatchStage(stage);
						}

						// End this loop since we overshot
						break;
					}
				}
			}
		}

		missionTemplates.close();
		taskObjectives.close();

		// Build list item widgets
		for(auto *mission: map.values()) {
			this->missions->addMission(mission);
		}

		this->missions->currentItemChanged(missions->item(0), nullptr);
		this->missions->item(0)->setSelected(true);
		this->exportAction->setEnabled(true);
		this->updateTitle();
	}
}

void MainWindow::exportFiles() {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::FileMode::Directory);
	dialog.setViewMode(QFileDialog::ViewMode::List);
	dialog.setOption(QFileDialog::Option::ShowDirsOnly);

	if(dialog.exec()) {
		QStringList directories = dialog.selectedFiles();
		if(directories.size() == 1) {
			const QString &url = directories.at(0);

			QFile missionTemplates(url + "/MissionTemplates.INT");
			if(!missionTemplates.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}
			QTextStream output(&missionTemplates);
			output.setCodec(QTextCodec::codecForName("UTF-8"));

			writeInfoHeader(output);

			output << "[MissionTemplates]\n";
			for(int i = 0; i < missions->count(); ++i) {
				auto *item = dynamic_cast<MissionListItem *>(missions->item(i));
				output << "MissionTemplates_" + item->code() + "_MissionTitle=" + item->name() + "\n";
			}
			missionTemplates.close();

			// Reuse the same QTextStream object
			output.flush();

			QFile taskObjectives(url + "/TaskObjectives.INT");
			if(!taskObjectives.open(QIODevice::WriteOnly | QIODevice::Text)) {
				return;
			}
			output.setDevice(&taskObjectives);
			writeInfoHeader(output);

			output << "[TaskObjectives]\n";
			for(int i = 0; i < missions->count(); ++i) {
				auto *item = dynamic_cast<MissionListItem *>(missions->item(i));

				// Owner stages gen
				for(int j = 0; j < item->ownerStages().size(); ++j) {
					QString stageText = handleStageText(j, item->ownerStages());
					QString objectives = item->ownerStages()[j].objectives->toPlainText();
					if(objectives.isEmpty()) {
						objectives = handleEmptyObjectives(j, item->ownerStages());
					}

					output << "TaskObjectives_" + item->code() + "_" + stageText + "_OwnerBrief=" + objectives + "\n";
				}

				// Dispatch stages gen
				for(int j = 0; j < item->dispatchStages().size(); ++j) {
					QString stageText = handleStageText(j, item->dispatchStages());
					QString objectives = item->dispatchStages()[j].objectives->toPlainText();
					if(objectives.isEmpty()) {
						objectives = handleEmptyObjectives(j, item->dispatchStages());
					}

					output << "TaskObjectives_" + item->code() + "_" + stageText + "_DispatchBrief=" + objectives + "\n";
				}
			}
			taskObjectives.close();
		}
	}
}

QString MainWindow::handleStageText(const int &index, const QVector<MissionListItem::Stage>& stages) {
	QString stageText = "Stage";
	int printedIndex = 1 + index;
	if(stages[index].hasOpp) {
		printedIndex--;
	}

	if(printedIndex < 10) {
		stageText += "0" + QString::number(printedIndex);
	} else {
		stageText += QString::number(printedIndex);
	}

	if(stages[index].hasOpp) {
		stageText += "_Opp";
	}

	return stageText;
}

QString MainWindow::handleEmptyObjectives(const int &index, const QVector<MissionListItem::Stage>& stagesVec) {
	if(stagesVec[index].hasOpp && index - 1 >= 0 && !stagesVec[index - 1].hasOpp) {
		// If this is an empty Opp stage, get objectives from the previous stage
		return stagesVec[index - 1].objectives->toPlainText();
	} else if(!stagesVec[index].hasOpp && index + 1 < stagesVec.size() && stagesVec[index + 1].hasOpp) {
		// Conversely, if this is an empty stage, get objectives from the following Opp stage if it exists
		return stagesVec[index + 1].objectives->toPlainText();
	} else {
		// Or it's just an empty stage, not a peculiarity of the game, so keep it empty
		return "";
	}
}

void MainWindow::writeInfoHeader(QTextStream &stream) {
	stream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n";
	stream << "; This file has been generated by cactupia's Localedit\n";
	stream << "; GitHub:  https://github.com/Electrostasy\n";
	stream << "; Steam:   https://steamcommunity.com/id/cactupia/\n";
	stream << "; Discord: cactus#7333\n";
	stream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\n";
}

QString MainWindow::verifyAndTrim(const QString &fileName) {
	QRegularExpression rx("(?:BRA|FRA|GER|INT|ITA|RUS|SPA)\\/(?<fileName>\\w+)");
	QRegularExpressionMatch match = rx.match(fileName);

	return match.captured("fileName");
}
