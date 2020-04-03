#include "FramedPage.h"

// Reimplemented paint event to draw a border on the left, right and bottom sides
void FramedPage::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	QPen lines(QColor(217, 217, 217), 1);
	painter.setPen(lines);
	painter.drawPolyline(
		QPolygon(
			QVector<QPoint>{
				QPoint(x(), y()),
				QPoint(x(), height() - 1),
				QPoint(width() - 1, height() - 1),
				QPoint(width() - 1, y())
			}
		)
	);

	QWidget::paintEvent(e);
}
