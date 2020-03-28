#pragma once

#include <QtWidgets>

#include "MissionListWidget.h"
#include "PreviewBoxWidget.h"
#include "StagesEditorWidget.h"

class MainWindow: public QMainWindow {
	Q_OBJECT

	public:
	explicit MainWindow();
	void updateTitle();

	public slots:
	void updateMissionInTitle(QTreeWidgetItem *selectedItem);

	private:
	const QString applicationName = "Localedit";
	QString selectedMission;
	bool hasPendingChanges = false;

	MissionListWidget *missionListWidget;
	PreviewBoxWidget *previewBoxWidget;
	StagesEditorWidget *stagesEditorWidget;
	QStatusBar *statusBarWidget;

	void initMenusActions();

	QMenu *fileMenu;
	QAction *importAction;

	private slots:
	void importFiles();
};
