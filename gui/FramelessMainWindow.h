#pragma once

#include <QtWidgets>

#include "APBPushButton.h"

// https://stackoverflow.com/a/44748349
class FramelessMainWindow: public QWidget {
	Q_OBJECT
	public:
	explicit FramelessMainWindow(QWidget *parent = nullptr);
	QMainWindow *content();

	public slots:
	void setWindowTitle(const QString &text);
	void setWindowIcon(const QIcon &icon);

	protected:
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void titleBarDoubleClick();

	private slots:
	void minimize();
	void restore();
	void maximize();

	private:
	QMainWindow *contents;
	QWidget *titleBar;
	QLabel *windowLabel;
	APBPushButton *minimizeButton;
	APBPushButton *restoreButton;
	APBPushButton *maximizeButton;
	APBPushButton *closeButton;
	const int DRAG_BORDER_SIZE = 15;
	QRect startGeometry;
	QPoint lastMousePos;
	bool moving;
	bool maximized;

	bool leftBorderSelected(const QPoint &pos);
	bool rightBorderSelected(const QPoint &pos);
	bool topBorderSelected(const QPoint &pos);
	bool bottomBorderSelected(const QPoint &pos);
};
