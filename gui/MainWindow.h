#pragma once

#include <QtWidgets>

#include "MissionList/MissionListWidget.h"
#include "MissionList/MissionListItem.h"
#include "StagesEditorWidget.h"

class MainWindow: public QMainWindow {
	Q_OBJECT

	public:
	explicit MainWindow();
	void updateTitle();

	public slots:
	void searchMissionList(const QString &filter);
	void updateMissionInTitle(QTreeWidgetItem *selectedItem);

	private:
	const QString applicationName = "Localedit";
	QString selectedMission;
	bool hasPendingChanges = false;
	QLineEdit *search;
	MissionListWidget *missions;
	StagesEditorWidget *stages;

	QMenu *fileMenu;
	QAction *importAction;
	QStatusBar *status;

	static QString verifyAndTrim(const QString &fileName);
	void initMenusActions();

	private slots:
	void importFiles();
};
