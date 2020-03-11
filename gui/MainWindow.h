#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "MissionListWidget.h"
#include "StagesEditorWidget.h"

class MainWindow: public QMainWindow {
	Q_OBJECT

	public:
	explicit MainWindow();

	private:
	MissionListWidget *missionListWidget;
	StagesEditorWidget *stagesEditorWidget;
	QStatusBar *statusBarWidget;
};

#endif
