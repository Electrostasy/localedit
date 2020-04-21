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

void MissionListWidget::switchItemsText(int state) {
	switch(state) {
		case Qt::CheckState::Unchecked:
			for(int index = 0; index < this->count(); ++index) {
				auto *item = dynamic_cast<MissionListItem *>(this->item(index));
				item->setText(item->name());
			}
			break;
		case Qt::CheckState::Checked:
			for(int index = 0; index < this->count(); ++index) {
				auto *item = dynamic_cast<MissionListItem *>(this->item(index));
				item->setText(item->code());
			}
			break;
		default:
			return;
	}
}
