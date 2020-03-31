#pragma once

#include <QListWidget>

class MissionListItem: public QListWidgetItem {
	friend class MissionListWidget;

	public:
	struct Stage {
		QString objectives;
		bool hasOpp;

		Stage();
		Stage(const QString &objectives, const bool &hasOpp);
	};

	struct Mission {
		QString name;
		QString code;
		QVector<Stage> owner;
		QVector<Stage> dispatch;

		Mission();
		Mission(const QString &name, const QString &code);
	};

	MissionListItem();
	explicit MissionListItem(const Mission &mission);
	MissionListItem(const QString &name, const QString &code);
	MissionListItem(const QString &name, const QString &code, const QVector<Stage> &owner, const QVector<Stage> &dispatch);

	void setIdentifiers(const QString &name, const QString &code);
	void pushOwnerStage(const Stage &stage);
	void pushDispatchStage(const Stage &stage);
	[[nodiscard]] QString name() const;
	[[nodiscard]] QString code() const;
	QString name();
	QString code();
	const QVector<Stage> &ownerStages();
	const QVector<Stage> &dispatchStages();

	private:
	Mission mission;
};
