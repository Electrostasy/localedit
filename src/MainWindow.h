#pragma once

#include <QtWidgets>

#include "Components/MessageBox.h"
#include "MissionList/MissionListItem.h"
#include "MissionList/MissionListWidget.h"
#include "StagesEditor/StagesEditorWidget.h"

// Use policy-based design to choose the correct platform-dependent
// base classes for the main window and buttons at compile time
#if defined(_WIN64)
#include "APBFramelessWindow/APBPushButton.h"
#include "APBFramelessWindow/NativeTranslucentFramelessWindow.h"
namespace {
	typedef NativeTranslucentFramelessWindow MainWindowBase;
	typedef APBPushButton PushButtonBase;
}	 // namespace
#elif defined(__linux__) || defined(__MACH__)
// The Linux/MacOS builds do not support all the UI customizations of the
// Windows build due to platform specific code, and there being no
// standardized ways to override the client-side decorations
namespace {
	typedef QWidget MainWindowBase;
	typedef QPushButton PushButtonBase;
}	 // namespace
#endif

class MainWindow: public MainWindowBase {
	Q_OBJECT

	public:
	explicit MainWindow();
	void updateTitle();

	public slots:
	void searchMissionList(const QString &filter);

	protected:
	void paintEvent(QPaintEvent *paintEvent) override;

	private:
	const QString applicationName = "Localedit";
	PushButtonBase *importButton;
	PushButtonBase *exportButton;
	QLineEdit *search;
	QCheckBox *nameIdSwitch;
	MissionListWidget *missions;
	StagesEditorWidget *stages;

	void openImportFilesDialog();
	static void writeInfoHeader(QTextStream &stream);
	static QString handleStageText(const int &index, const QVector<MissionListItem::Stage> &stages);
	static QString handleEmptyObjectives(const int &index, const QVector<MissionListItem::Stage> &stages);

	private slots:
	void importFiles();
	void exportFiles();
	void handleUnsavedChangesBox();
	static QString verifyAndTrim(const QString &fileName);
	void readMission(QTextStream *stream, QString line, QMap<QString, MissionListItem *> *map) const;
	void readTasks(QTextStream *stream, QString line, QMap<QString, MissionListItem *> *map) const;
	void verifyFileNames(QFileDialog const *dialog, QFile *missionTemplates, QFile *taskObjectives) const;
	void exportMissions(QTextStream *output) const;
};
