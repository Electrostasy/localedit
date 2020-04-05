#pragma once

#include <QWidget>
#include <QtWidgets>

class FramedPage: public QWidget {
	Q_OBJECT

	protected:
	void paintEvent(QPaintEvent *e) override;
};
