#pragma once

#include <QWidget>
#include <QtWidgets>

#include "../MissionList/MissionListWidget.h"
#include "TabWidget/ExpandingTabWidget.h"
#include "../Utility/LayoutUtils.h"
#include "SyntaxHighlighter.h"
#include "ExpandedTextEdit.h"
#include "FramedPage.h"

class StagesEditorWidget: public QWidget {
	Q_OBJECT

	public:
	explicit StagesEditorWidget(QWidget *parent = nullptr);

	QTabWidget *tabWidget;
	QWidget *pageOwner;
	QWidget *pageDispatch;

	public slots:
	void showStages(MissionListItem *currentItem);

	private:
	void initSidedStages(QWidget *page, const QVector<MissionListItem::Stage> &stages);
};
