#include "FramedPage.h"

FramedPage::FramedPage(QWidget *parent): QWidget(parent) {
	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Window, QColor(60, 60, 60));
	this->setPalette(palette);
}

// Reimplemented paint event to draw a border on the left, right and bottom sides
void FramedPage::paintEvent(QPaintEvent *event) {
	auto *painter = new QPainter(this);
	painter->setPen(Qt::PenStyle::NoPen);
	// TODO BUG: Palette doesn't work for some reason
	painter->setBrush(QColor(60, 60, 60));

	painter->drawRect(event->rect());

	painter->end();
}
