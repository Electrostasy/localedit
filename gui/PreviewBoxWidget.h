#pragma once

#include <QWidget>
#include <QtGui>
#include <QtWidgets>
#include <QtMath>

class PreviewBoxWidget: public QWidget {
	Q_OBJECT

	public:
	explicit PreviewBoxWidget(QWidget *parent = nullptr);

	public slots:
	void setMissionName(QListWidgetItem *selectedItem);
	void setMissionStageText(QTextDocument *document);

	private:
	QLabel *missionName;
	QPlainTextEdit *missionText;

	static void setPaletteBG(QWidget *widget, const QColor& colour);
	static void setPaletteFG(QWidget *widget, const QColor& colour);
};
