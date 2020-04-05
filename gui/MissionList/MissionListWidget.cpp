#include "MissionListWidget.h"

MissionListWidget::MissionListWidget(QWidget *parent): QListWidget(parent) {
	this->setSelectionBehavior(QAbstractItemView::SelectItems);
	this->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MissionListWidget::addMission(MissionListItem *item) {
	this->addItem(item);

	int width = QFontMetrics(item->font()).horizontalAdvance(item->mission.name);
	if(width > item->listWidget()->minimumWidth()) {
		item->listWidget()->setMinimumWidth(width);
	}
}
