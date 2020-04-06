#pragma once

#include <QtWidgets>

#include "MissionList/MissionListItem.h"
#include "MissionList/MissionListWidget.h"
#include "StagesEditor/StagesEditorWidget.h"

class MainWindow: public QMainWindow {
	public:
	explicit MainWindow();
	void updateTitle();

	public slots:
	void searchMissionList(const QString &filter);

	private:
	const QString applicationName = "Localedit";
	QLineEdit *search;
	MissionListWidget *missions;
	StagesEditorWidget *stages;
	QStatusBar *status;
	QAction *importAction;
	QAction *exportAction;

	static QString verifyAndTrim(const QString &fileName);
	void initMenusActions();
	void openImportFilesDialog();
	static void writeInfoHeader(QTextStream &stream);
	static QString handleStageText(const int &index, const QVector<MissionListItem::Stage> &stages);
	static QString handleEmptyObjectives(const int &index, const QVector<MissionListItem::Stage> &stages);

	private slots:
	void importFiles();
	void exportFiles();
};
