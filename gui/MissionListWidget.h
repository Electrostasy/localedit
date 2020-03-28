#pragma once

#include <QLayout>
#include <QMimeData>
#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtEvents>
#include <QtMath>
#include <QLineEdit>
#include <QTreeWidget>

class MissionListWidget: public QWidget {
	Q_OBJECT

	public:
	enum Metadata {
		MissionCode = 0x00FF,
		MissionModified = 0x00FE
	};

	struct Mission {
		QString name;
		QString code;
		QVector<QString> stagesOwner;
		QVector<QString> stagesDispatch;
		bool hasPendingChanges = false;
	};
	/* We use a map of mission codes to missions, because we build missions from two different files that contain
	 * different mission information.
	 * - In MissionTemplates.* files, we assign the mission its code and name.
	 * - In TaskObjectives.* files, we assign the mission its stages for Owner and Dispatch sides.
	 * QMap was chosen for lookup speed when doing a 2nd pass to add mission information.
	 * TODO: Mission map needs a clearer data structure
	 */
	QMap<QString, Mission> missionMap;

	QLineEdit *searchBar;
	QTreeWidget *missionList;

	explicit MissionListWidget(QWidget *parent = nullptr);
	void parseFile(QFile *file);
	void parseMissionTemplates(Mission &mission, const QString &line);
	void parseTaskObjectives(Mission &mission, const QString &line);
	void addMissionToList(Mission &mission) const;

	signals:
	// void missionSelected(QTreeWidgetItem *selectedMission);


	private:
	QRegularExpression rxMissionCodeTemplates;
	QRegularExpression rxMissionCodeObjectives;
	QRegularExpression rxRemainder;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;
};
