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

	private:
	static void clearLayout(QLayout *layout);
	static void populatePage(QWidget *page, const QVector<QString>& stages);
};
