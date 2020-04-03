#pragma once

#include <QWidget>
#include <QtWidgets>

class FramedPage: public QWidget {
protected:
	void paintEvent(QPaintEvent *e) override;

};


