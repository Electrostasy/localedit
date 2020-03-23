#pragma once

#include <QLayout>
#include <QMimeData>
#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtEvents>
#include <QtMath>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>

class MissionListWidget: public QWidget {
	Q_OBJECT

	public:
	// Map of mission codes to mission names
	QMap<QString, QString> missionMap;
	// Map of mission codes to sides of stages that have mission text
	QMap<QString, QPair<QVector<QString>, QVector<QString>>> missionStagesMap;

	QLineEdit *searchBar;
	QListWidget *missionList;

	explicit MissionListWidget(QWidget *parent = nullptr);
	[[nodiscard]] int addMissionToList(const QString &missionCode, const QString &missionName) const;

	// signals:
	// void missionSelected(QListWidgetItem *selectedItem);

	private:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;
};
