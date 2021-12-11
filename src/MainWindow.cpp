#include "MainWindow.h"

MainWindow::MainWindow() {
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

	// Create import/export buttons
	auto horizontalButtonLayout = new QHBoxLayout();
	importButton = new PushButtonBase("Import");
	exportButton = new PushButtonBase("Export");
	exportButton->setDisabled(true);
	connect(importButton, &PushButtonBase::released, this, &MainWindow::openImportFilesDialog);
	connect(exportButton, &PushButtonBase::released, this, &MainWindow::exportFiles);
	horizontalButtonLayout->addWidget(importButton);
	horizontalButtonLayout->addWidget(exportButton);
	horizontalButtonLayout->setMargin(0);
	horizontalButtonLayout->setSpacing(5);

	// Create the missions search bar
	auto *horizontalSearchLayout = new QHBoxLayout();
	search = new QLineEdit();
	search->setDisabled(true);
	search->setPlaceholderText("Search missions...");
	nameIdSwitch = new QCheckBox();
	nameIdSwitch->setDisabled(true);
	nameIdSwitch->setStatusTip("Switch between displaying mission codes and titles");
	connect(search, &QLineEdit::textChanged, this, &MainWindow::searchMissionList);
	horizontalSearchLayout->addWidget(search);
	horizontalSearchLayout->addWidget(nameIdSwitch);
	horizontalSearchLayout->setMargin(0);

	// Create the mission list then add the buttons and search bar
	auto *verticalMissionsLayout = new QVBoxLayout();
	missions = new MissionListWidget();
	missions->setDisabled(true);
	connect(nameIdSwitch, &QCheckBox::stateChanged, missions, &MissionListWidget::switchItemsText);
	connect(missions, &QListWidget::currentItemChanged, this, &MainWindow::updateTitle);
	verticalMissionsLayout->addLayout(horizontalButtonLayout);
	verticalMissionsLayout->addLayout(horizontalSearchLayout);
	verticalMissionsLayout->addWidget(missions);
	verticalMissionsLayout->setMargin(0);

	auto *splitter = new QSplitter();

	auto *missionListContainer = new QWidget();
	missionListContainer->setLayout(verticalMissionsLayout);

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

	splitter->addWidget(missionListContainer);
	splitter->addWidget(stages);
	splitter->setChildrenCollapsible(false);
	splitter->setSizes(QList{ missionListContainer->width() / 3, splitter->width() - missionListContainer->width() / 3 });
	if(this->layout() == nullptr) {
    this->setLayout(new QVBoxLayout());
  };
	this->layout()->setMargin(0);
	this->layout()->setContentsMargins(0, 0, 0, 0);
	this->layout()->setSpacing(5);
	this->layout()->addWidget(splitter);

	// Add styling using QPalette to the main window
	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Window, QColor(94, 94, 94));
	this->setPalette(palette);
}

void MainWindow::searchMissionList(const QString &filter) {
	for(int i = 0; i < missions->count(); ++i) {
		auto *mission = dynamic_cast<MissionListItem *>(missions->item(i));

		mission->setHidden(!mission->text().contains(filter, Qt::CaseInsensitive));
	}
}

void MainWindow::updateTitle() {
	QString title = this->applicationName;

	auto *currentItem = dynamic_cast<MissionListItem *>(missions->currentItem());
	if(currentItem != nullptr) {
		title += " - " + currentItem->name();
	}

	this->setWindowTitle(title);
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
		QRegularExpression STAGE_REGEX("TaskObjectives_(?<code>[A-Za-z0-9_]+?)_(?:Stage\\d\\d_)?(?:(?<special>Opp)_)?(?<side>[A-Za-z]+)=(?<text>.*)");

		QMap<QString, MissionListItem *> map;
		QString line;

		// Read mission code and title
		QTextStream stream(&missionTemplates);
		while(stream.readLineInto(&line)) {
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
		}
		missionTemplates.close();

		// Read stages
		stream.setDevice(&taskObjectives);
		while(stream.readLineInto(&line)) {
			// Unlikely to encounter comments, TODO: maybe remove if statement
			if(line.startsWith(';')) {
				continue;
			}

			QRegularExpressionMatch match = STAGE_REGEX.match(line);
			if(match.hasMatch()) {
				QString side = match.captured("side");
				// Some missions have an "Opp" tag on some stages, we flag it as appropriate to avoid displaying it
				// and then during exporting we copy the data from the equivalent non-Opp stage to it
				auto *objectives = new QTextDocument(match.captured("text"));
				bool opp = !match.captured("special").isEmpty();
				if(opp) {
					// Clear any text in Opp stages because we duplicate it on export
					objectives->setPlainText("");
				}
				MissionListItem::Stage stage(objectives, opp);

				if(match.hasMatch()) {
					if(map[match.captured("code")] == nullptr) {
						map[match.captured("code")] = new MissionListItem();
					}

					if(side == "OwnerBrief") {
						map[match.captured("code")]->pushOwnerStage(stage);
					}

					if(side == "DispatchBrief") {
						map[match.captured("code")]->pushDispatchStage(stage);
					}
				}
			}
		}
		taskObjectives.close();

		// Build list item widgets
		for(auto *mission: map.values()) {
			this->missions->addMission(mission);
		}

		exportButton->setDisabled(false);
		search->setDisabled(false);
		nameIdSwitch->setDisabled(false);
		nameIdSwitch->setCheckState(Qt::CheckState::Unchecked);
		missions->setDisabled(false);
		this->missions->currentItemChanged(missions->item(0), nullptr);
		this->missions->item(0)->setSelected(true);
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
	stream << "; Steam:   https://steamcommunity.com/id/cactupia/\n";
	stream << "; Discord: cactus#7333\n";
	stream << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n\n";
}

QString MainWindow::verifyAndTrim(const QString &fileName) {
	QRegularExpression rx("(?:BRA|FRA|GER|INT|ITA|RUS|SPA)\\/(?<fileName>\\w+)");
	QRegularExpressionMatch match = rx.match(fileName);

	return match.captured("fileName");
}

// Tint the background colour of the window
void MainWindow::paintEvent(QPaintEvent *paintEvent) {
	auto *painter = new QPainter(this);

	QColor backgroundColor = palette().window().color();
	backgroundColor.setAlpha(175);
	painter->fillRect(this->rect(), backgroundColor);

	painter->end();
	QWidget::paintEvent(paintEvent);
}

