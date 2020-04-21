#pragma once

#include <QtWidgets>

#include "APBFramelessWindow/NativeTranslucentFramelessWindow.h"
#include "APBFramelessWindow/APBFramelessWindowTitleBar.h"
#include "APBFramelessWindow/APBPushButton.h"
#include "MissionList/MissionListItem.h"
#include "MissionList/MissionListWidget.h"
#include "StagesEditor/StagesEditorWidget.h"

class MainWindow: public NativeTranslucentFramelessWindow {
	public:
	explicit MainWindow();
	void updateTitle();

	public slots:
	void searchMissionList(const QString &filter);

	protected:
	void paintEvent(QPaintEvent *paintEvent) override;
	bool isTitleBarHit(const QRect &iRect, const long iBorderWidth, long iX, long iY) override;

	private:
	APBFramelessWindowTitleBar *titleBar;
	const QString applicationName = "Localedit";
	APBPushButton *importButton;
	APBPushButton *exportButton;
	QLineEdit *search;
	QCheckBox *nameIdSwitch;
	MissionListWidget *missions;
	StagesEditorWidget *stages;

	static QString verifyAndTrim(const QString &fileName);
	void openImportFilesDialog();
	static void writeInfoHeader(QTextStream &stream);
	static QString handleStageText(const int &index, const QVector<MissionListItem::Stage> &stages);
	static QString handleEmptyObjectives(const int &index, const QVector<MissionListItem::Stage> &stages);

	private slots:
	void importFiles();
	void exportFiles();
};
