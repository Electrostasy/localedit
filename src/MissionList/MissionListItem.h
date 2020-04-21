#pragma once

#include <QListWidget>
#include <QtCore>
#include <QtGui/QTextDocument>

class MissionListItem: public QListWidgetItem {
	friend class MissionListWidget;

	public:
	struct Stage {
		QTextDocument *objectives = nullptr;
		bool hasOpp = false;

		Stage(QTextDocument *objectives, const bool &hasOpp);
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
