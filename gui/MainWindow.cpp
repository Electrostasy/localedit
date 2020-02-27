#include <QtGui>
#include <QWidget>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) {
	// Stack all widgets (layouts) horizontally
	auto masterHorizontalLayout = new QHBoxLayout(this);

	{ // Construct vertical column for mission selection
		auto label = new QLabel("Missions");

		auto missionSearch = new QLineEdit();
		missionSearch->setPlaceholderText("Search missions...");

		// Create a list to search through
		missionList = new QListWidget();
		missionList->setSelectionBehavior(QAbstractItemView::SelectItems);
		missionList->setSelectionMode(QAbstractItemView::SingleSelection);

		// Connect search to list
		connect(missionSearch, &QLineEdit::textChanged, this, [=](const QString& filter) {
			for (int i = 0; i < missionList->count(); ++i) {
				auto item = missionList->item(i);

				// Show item if match, otherwise hide
				item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
			}
		});

		// Add widgets to a vertical layout
		auto missionsLayout = new QVBoxLayout();
		missionsLayout->addWidget(label);
		missionsLayout->addWidget(missionSearch);
		missionsLayout->addWidget(missionList);

		// Add the vertical layout for missions to the main window
		masterHorizontalLayout->addLayout(missionsLayout);
	}

	{ // Construct preview and stages editor
		auto tabOwner = new QWidget();
		auto tabDispatch = new QWidget();

		tabOwner->setLayout(new QVBoxLayout());
		tabDispatch->setLayout(new QVBoxLayout());

		tabWidget = new QTabWidget();
		tabWidget->addTab(tabOwner, "Owner");
		tabWidget->addTab(tabDispatch, "Dispatch");

		// Items selected in the mission list will update the stages
		connect(missionList, &QListWidget::itemClicked, this, [=](const QListWidgetItem* item) {
			clearLayout(tabOwner->layout());
			clearLayout(tabDispatch->layout());

			// TODO stages expand window size, implement scrollbar instead

			auto missionCode = item->data(Qt::UserRole).toString();

			foreach (const QString& stage, missionStagesMap[missionCode].first) {
				auto textField = new QPlainTextEdit(tabOwner);
				textField->setPlainText(stage);

				tabOwner->layout()->addWidget(textField);
			}

			foreach (const QString& stage, missionStagesMap[missionCode].second) {
				auto textField = new QPlainTextEdit(tabDispatch);
				textField->setPlainText(stage);

				tabDispatch->layout()->addWidget(textField);
			}
		});

		auto* stagesLayout = new QVBoxLayout();
		stagesLayout->addWidget(tabWidget);
		masterHorizontalLayout->addLayout(stagesLayout);
	}
}

void MainWindow::clearLayout(QLayout *layout) {
	if (layout) {
		while(layout->count() > 0) {
			QLayoutItem *item = layout->takeAt(0);
			QWidget* widget = item->widget();
			if(widget) {
				delete widget;
			}
			delete item;
		}
	}
}

void MainWindow::addMissionToList(const QString& missionCode, const QString& missionName) {
	auto* item = new QListWidgetItem(missionList);
	item->setData(Qt::UserRole, missionCode);
	item->setText(missionName);
}

// Accept for drag and drop only physical (local) files that have a path on disk
void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

// Drop multiple files onto the window and load them from path as read only
void MainWindow::dropEvent(QDropEvent* event) {
	QRegularExpression rxMissionCodeTemplates("(?<=MissionTemplates_).+?(?=_MissionTitle)");
	QRegularExpression rxMissionCodeObjectives("(?<=TaskObjectives_).+?(?=_Stage)");
	QRegularExpression rxRemainder("(?<=\\=).*");
	foreach (const QUrl& url, event->mimeData()->urls()) {
		QFile file(url.toLocalFile());
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				return;
			}

			QTextStream namesStream(&file);
			while (!namesStream.atEnd()) {
				QString line = namesStream.readLine();

				// If file is MissionTemplates (mission code, mission name)
				if (line.startsWith("MissionTemplates_")) {
					QRegularExpressionMatch matchMissionCode = rxMissionCodeTemplates.match(line);
					QRegularExpressionMatch matchMissionName = rxRemainder.match(line);

					if (matchMissionCode.hasMatch() && matchMissionName.hasMatch()) {
						missionMap[matchMissionCode.captured(0)] = matchMissionName.captured(0);
						addMissionToList(matchMissionCode.captured(0), matchMissionName.captured(0));
					}
				}

				// If file is TaskObjectives (mission code, mission stage and description)
				if (line.startsWith("TaskObjectives_")) {
					QRegularExpressionMatch matchMissionCode = rxMissionCodeObjectives.match(line);
					QRegularExpressionMatch matchMissionText = rxRemainder.match(line);

					if (matchMissionCode.hasMatch() && matchMissionText.hasMatch()) {
						if (line.contains("_OwnerBrief=")) {
							missionStagesMap[matchMissionCode.captured(0)].first.push_back(matchMissionText.captured(0));
						}

						if (line.contains("_DispatchBrief=")) {
							missionStagesMap[matchMissionCode.captured(0)].second.push_back(matchMissionText.captured(0));
						}
					}
				}
			}
			file.close();
	}
}
