#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QtWidgets>
#include <QPlainTextEdit>

class MainWindow: public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
	// Map of mission codes to mission names
    QMap<QString, QString> missionMap;
	// Map of mission codes to sides of stages that have mission text
    QMap<QString, QPair<QString, QVector<QString>>> missionStagesMap;

	QListWidget *missionList;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void addMissionToList(const QString &missionCode, const QString &missionName);
};

#endif