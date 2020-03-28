#include "MissionListWidget.h"
#include <QtWidgets/QLabel>

MissionListWidget::MissionListWidget(QWidget *parent): QWidget(parent) {
	// Initialize regex patterns for mission text parsing
	rxMissionCodeTemplates.setPattern("(?<=MissionTemplates_).+?(?=_MissionTitle)");
	rxMissionCodeObjectives.setPattern("(?<=TaskObjectives_).+?(?=_Stage)");
	rxRemainder.setPattern("(?<=\\=).*");

	this->setAcceptDrops(true);

	searchBar = new QLineEdit();
	searchBar->setPlaceholderText("Search missions...");

	missionList = new QTreeWidget();
	missionList->setColumnCount(2);
	missionList->setColumnWidth(1, QFontMetrics(missionList->font()).horizontalAdvance("*"));
	missionList->setHeaderHidden(true);
	missionList->setSelectionBehavior(QAbstractItemView::SelectItems);
	missionList->setSelectionMode(QAbstractItemView::SingleSelection);

	// connect(missionList, &QTreeWidget::currentItemChanged, this, &MissionListWidget::missionSelected);
	connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &filter) {
		QTreeWidgetItemIterator itemIterator(missionList);

		while(*itemIterator) {
			(*itemIterator)->setHidden(!(*itemIterator)->text(0).contains(filter, Qt::CaseInsensitive));

			itemIterator++;
		}
	});

	auto *missionsLayout = new QVBoxLayout(this);
	missionsLayout->addWidget(searchBar);
	missionsLayout->addWidget(missionList);
}

// Accept for drag and drop only physical (local) files that have a path on disk
void MissionListWidget::dragEnterEvent(QDragEnterEvent *event) {
	if(event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

// Drop multiple files onto the window and load them from path as read only
void MissionListWidget::dropEvent(QDropEvent *event) {
	foreach(const QUrl &url, event->mimeData()->urls()) {
		QFile file(url.toLocalFile());
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return;
		}

		parseFile(&file);
		file.close();
	}
}

void MissionListWidget::parseFile(QFile *file) {
	QTextStream fileContent(file);
	while(!fileContent.atEnd()) {
		QString line = fileContent.readLine();
		if(line.startsWith(';')) {
			// Skip lines with comments
			continue;
		}

		Mission mission;

		if(line.startsWith("MissionTemplates_")) {
			parseMissionTemplates(mission, line);
		}

		if(line.startsWith("TaskObjectives_")) {
			parseTaskObjectives(mission, line);
		}

		break;
	}
}

void MissionListWidget::parseMissionTemplates(Mission &mission, const QString &line) {
	QRegularExpressionMatch matchMissionCode = rxMissionCodeTemplates.match(line);
	QRegularExpressionMatch matchMissionName = rxRemainder.match(line);

	if(matchMissionCode.hasMatch() && matchMissionName.hasMatch()) {
		mission.code = matchMissionCode.captured(0);
		mission.name = matchMissionName.captured(0);
		missionMap[mission.code] = mission;

		addMissionToList(mission);
	}
}
// int MissionListWidget::addMissionToList(Mission &mission) const {
void MissionListWidget::addMissionToList(Mission &mission) const {
	auto *item = new QTreeWidgetItem(missionList);
	item->setData(0, Metadata::MissionCode, mission.code);
	item->setText(0, mission.name);
	item->setText(1, "*");

	if(missionList->columnWidth(0) < QFontMetrics(item->font(0)).horizontalAdvance(mission.name)) {
		missionList->resizeColumnToContents(0);
	}
}

void MissionListWidget::parseTaskObjectives(Mission &mission, const QString &line) {
	QRegularExpressionMatch matchMissionCode = rxMissionCodeObjectives.match(line);
	QRegularExpressionMatch matchMissionText = rxRemainder.match(line);

	if(matchMissionCode.hasMatch() && matchMissionText.hasMatch()) {
		if(line.contains("_OwnerBrief=")) {
			missionMap[matchMissionCode.captured(0)].stagesOwner.push_back(matchMissionText.captured(0));
		}

		if(line.contains("_DispatchBrief=")) {
			missionMap[matchMissionCode.captured(0)].stagesDispatch.push_back(matchMissionText.captured(0));
		}
	}
}
