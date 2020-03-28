#pragma once

#include <QWidget>
#include <QtWidgets>

#include "MissionListWidget.h"
#include "SyntaxHighlighter.h"

class StagesEditorWidget: public QWidget {
	Q_OBJECT

	public:
	explicit StagesEditorWidget(MissionListWidget *missionListWidget);

	QTabWidget *tabWidget;
	QWidget *ownerPage;
	QWidget *dispatchPage;

	QVector<QTextEdit*> shownStages;

	private:
	MissionListWidget *associatedListWidget;

	static void clearLayout(QLayout *layout);
	void showMissionStages(QTreeWidgetItem *currentItem);
	void buildMutableStages(MissionListWidget::Mission& mutableMission, const bool& forOwner);

	signals:
	void stageFocused(const int& index);
};
