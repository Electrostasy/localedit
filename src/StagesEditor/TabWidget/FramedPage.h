#pragma once

#include <QWidget>
#include <QtWidgets>

class FramedPage: public QWidget {
	Q_OBJECT
	public:
	explicit FramedPage(QWidget *parent = nullptr);

	protected:
	void paintEvent(QPaintEvent *event) override;
};
