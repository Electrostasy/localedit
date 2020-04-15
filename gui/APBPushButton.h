#pragma once

#include <QtWidgets>

class APBPushButton: public QPushButton {
	Q_OBJECT

	public:
	explicit APBPushButton(QWidget *parent = nullptr);
	void setButtonColour(const QColor &colour);

	protected:
	bool event(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

	private:
	int outerPaintMargin = 2;
	QFont buttonFont;
	QColor orange;
	QColor black;
	QColor lightGrey;
	QColor baseColour;
	void setLighterColour(QPalette *palette);
	void setDarkerColour(QPalette *palette);
	void setHighlight(QPalette *palette);
	void restoreDefaults(QPalette *palette);
};
