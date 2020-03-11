#include "MissionListWidget.h"
#include <QtWidgets/QLabel>

MissionListWidget::MissionListWidget(QWidget *parent): QWidget(parent) {
	this->setAcceptDrops(true);

	searchBar = new QLineEdit();
	searchBar->setPlaceholderText("Search missions...");

	// Create a list to search through
	missionList = new QListWidget();
	missionList->setSelectionBehavior(QAbstractItemView::SelectItems);
	missionList->setSelectionMode(QAbstractItemView::SingleSelection);

	// Connect search to list
	connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &filter) {
		for(int i = 0; i < missionList->count(); ++i) {
			auto *item = missionList->item(i);

			// Show item if match, otherwise hide
			item->setHidden(!item->text().contains(filter, Qt::CaseInsensitive));
		}
	});

	// Add widgets to a vertical layout
	auto *missionsLayout = new QVBoxLayout(this);
	missionsLayout->addWidget(searchBar);
	missionsLayout->addWidget(missionList);
}

double MissionListWidget::addMissionToList(const QString &missionCode, const QString &missionName) const {
	auto *item = new QListWidgetItem(missionList);
	item->setData(Qt::UserRole, missionCode);
	item->setText(missionName);

	return QFontMetrics(item->font()).horizontalAdvance(missionName) / 1.8;
}

// Accept for drag and drop only physical (local) files that have a path on disk
void MissionListWidget::dragEnterEvent(QDragEnterEvent *event) {
	if(event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

// Drop multiple files onto the window and load them from path as read only
void MissionListWidget::dropEvent(QDropEvent *event) {
	QRegularExpression rxMissionCodeTemplates("(?<=MissionTemplates_).+?(?=_MissionTitle)");
	QRegularExpression rxMissionCodeObjectives("(?<=TaskObjectives_).+?(?=_Stage)");
	QRegularExpression rxRemainder("(?<=\\=).*");
	foreach(const QUrl &url, event->mimeData()->urls()) {
		QFile file(url.toLocalFile());
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return;
		}

		QTextStream namesStream(&file);
		while(!namesStream.atEnd()) {
			QString line = namesStream.readLine();

			// If file is MissionTemplates (mission code, mission name)
			if(line.startsWith("MissionTemplates_")) {
				QRegularExpressionMatch matchMissionCode = rxMissionCodeTemplates.match(line);
				QRegularExpressionMatch matchMissionName = rxRemainder.match(line);

				if(matchMissionCode.hasMatch() && matchMissionName.hasMatch()) {
					missionMap[matchMissionCode.captured(0)] = matchMissionName.captured(0);
					double width = addMissionToList(matchMissionCode.captured(0), matchMissionName.captured(0));
					if(width > this->minimumWidth()) {
						this->setMinimumWidth(qFloor(width));
					}
				}
			}

			// If file is TaskObjectives (mission code, mission stage and description)
			if(line.startsWith("TaskObjectives_")) {
				QRegularExpressionMatch matchMissionCode = rxMissionCodeObjectives.match(line);
				QRegularExpressionMatch matchMissionText = rxRemainder.match(line);

				if(matchMissionCode.hasMatch() && matchMissionText.hasMatch()) {
					if(line.contains("_OwnerBrief=")) {
						missionStagesMap[matchMissionCode.captured(0)].first.push_back(matchMissionText.captured(0));
					}

					if(line.contains("_DispatchBrief=")) {
						missionStagesMap[matchMissionCode.captured(0)].second.push_back(matchMissionText.captured(0));
					}
				}
			}
		}
		file.close();
	}
}
