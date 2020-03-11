#ifndef STAGESEDITORWIDGET_H
#define STAGESEDITORWIDGET_H

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

	private:
	static void clearLayout(QLayout *layout);
};

#endif
