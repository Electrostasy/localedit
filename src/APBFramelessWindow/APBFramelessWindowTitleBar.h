#pragma once

#include <QtWidgets>

#include "APBPushButton.h"

class APBFramelessWindowTitleBar: public QWidget {
	Q_OBJECT

	public:
	explicit APBFramelessWindowTitleBar(QWidget *parent = nullptr);
	[[nodiscard]] int buttonWidth() const;
	[[nodiscard]] int labelWidth() const;
	void setTitleBarText(const QString &text);
	QLabel *titleBarLabel();

	signals:
	void minimizeButtonClicked();
	void restoreButtonClicked();
	void maximizeButtonClicked();
	void closeButtonClicked();

	protected:
	void paintEvent(QPaintEvent *paintEvent) override;

	private:
	QLabel *windowTitle;
	APBPushButton *minimizeButton;
	APBPushButton *restoreButton;
	APBPushButton *maximizeButton;
	APBPushButton *closeButton;
};
