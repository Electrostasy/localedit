#pragma once

#include <QWidget>
#include <QtWidgets>

class ExpandingTabBar: public QTabBar {
	Q_OBJECT

	public:
	explicit ExpandingTabBar(QWidget *parent = nullptr);

	protected:
	bool event(QEvent *event) override;
	void paintEvent(QPaintEvent *paintEvent) override;
	void restorePen(QPen pen, int index);
	void setBrush(QPainter* painter, int index);
	QRect createTabRect(int index);

	private:
	int outerPaintMargin = 4;
	int mouseoveredTab;
};
