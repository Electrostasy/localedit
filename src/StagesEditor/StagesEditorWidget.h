#pragma once

#include <QWidget>
#include <QtWidgets>

#include "../MissionList/MissionListWidget.h"
#include "../Utility/LayoutUtils.h"
#include "ExpandedTextEdit.h"
#include "TabWidget/ExpandingTabWidget.h"
#include "TabWidget/FramedPage.h"

class StagesEditorWidget: public QWidget {
	Q_OBJECT

	public:
	explicit StagesEditorWidget(QWidget *parent = nullptr);

	QTabWidget *tabWidget;
	QWidget *pageOwner;
	QWidget *pageDispatch;

	void buildStages(QWidget *page, const QVector<MissionListItem::Stage> &stages, MissionListItem *currentItem);
};
