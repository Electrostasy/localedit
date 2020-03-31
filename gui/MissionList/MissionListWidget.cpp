#include "MissionListWidget.h"

MissionListWidget::MissionListWidget(QWidget *parent): QListWidget(parent) {
	this->setSelectionBehavior(QAbstractItemView::SelectItems);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MissionListWidget::addMission(MissionListItem *item) {
	this->addItem(item);

	// Automatic resizing based on mission name length
	int width = QFontMetrics(item->font()).horizontalAdvance(item->mission.name);
	if(this->lineWidth() < width) {
		this->resizeContents(width, this->contentsSize().height());
	}
}
