#pragma once

#include <QtWidgets>

class APBPushButton: public QPushButton {
	Q_OBJECT

	public:
	explicit APBPushButton(QWidget *parent = nullptr);
	void setButtonColour(const QColor &colour);
	void blinkOnHover(bool blink);

	// States a button can have to function as title bar replacement or generic buttons (None)
	enum TitleIcon {
		Maximize,
		Restore,
		Minimize,
		Close,
		None
	};
	void setTitleIcon(TitleIcon icon);
	TitleIcon titleIcon();

	protected:
	bool event(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

	private:
	// Timer used to animate blinking of button palette
	QTimeLine *timer = nullptr;
	int loopCounter = 0;
	void startTimer(QPalette *palette);
	void stopTimer(QPalette *palette);
	void resetTimer();

	// Painter/palette properties
	int outerPaintMargin = 2;
	int innerPaintMargin = 5;
	TitleIcon titleBarIconType = TitleIcon::None;
	QFont buttonFont;
	QColor orange;
	QColor black;
	QColor lightGrey;
	QColor baseColour;

	// Statically change button colours
	void setLighterColour(QPalette *palette);
	void setDarkerColour(QPalette *palette);
	void setHighlight(QPalette *palette);
	void restoreDefaults(QPalette *palette);

	// Draw custom symbols for title bar or text
	void drawText(QPainter *painter, const QRect &bounds);
	void drawTitleBarMinimize(QPainter *painter, const QRect &bounds);
	void drawTitleBarMaximize(QPainter *painter, const QRect &bounds);
	void drawTitleBarClose(QPainter *painter, const QRect &bounds);

	private slots:
	// Called whenever QTimeLine is running to lighten and darken button palette
	void cycleButtonColour(double newValue);
};
