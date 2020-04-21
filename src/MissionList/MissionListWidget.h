#pragma once

#include <QListWidget>

#include "MissionListItem.h"

class MissionListWidget: public QListWidget {
	public:
	explicit MissionListWidget(QWidget *parent = nullptr);
	void addMission(MissionListItem *item);

	public slots:
	void switchItemsText(int state);
};
