#include "APBPushButton.h"

APBPushButton::APBPushButton(QWidget *parent): QPushButton(parent) {
	buttonFont.setStyleHint(QFont::StyleHint::Times, QFont::StyleStrategy::PreferAntialias);
	buttonFont.setPointSize(10);

	this->orange = QColor(227, 182, 109);
	this->black = QColor(0, 0, 0);
	this->lightGrey = QColor(216, 216, 216);
	this->baseColour = QColor(132, 132, 132);

	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Highlight, this->black);
	palette.setColor(QPalette::ColorRole::Button, this->baseColour);
	palette.setColor(QPalette::ColorRole::ButtonText, this->lightGrey);
	this->setPalette(palette);
}

bool APBPushButton::event(QEvent *event) {
	QPalette palette = this->palette();

	switch(event->type()) {
		case QEvent::Type::HoverEnter:
			this->setLighterColour(&palette);
			break;
		case QEvent::Type::HoverLeave:
			this->setDarkerColour(&palette);
			break;
		case QEvent::MouseButtonPress:
			this->setHighlight(&palette);
			break;
		case QEvent::MouseButtonDblClick:
			this->setHighlight(&palette);
			break;
		case QEvent::MouseButtonRelease:
			this->restoreDefaults(&palette);
			break;
		default:
			return QPushButton::event(event);
	}
}

void APBPushButton::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setFont(this->buttonFont);

	// Remove borders
	QPen pen = painter.pen();
	pen.setStyle(Qt::PenStyle::NoPen);
	painter.setPen(pen);

	// Draw outer box
	QRect outer = event->rect();
	painter.setBrush(this->palette().highlight());
	painter.drawPolygon(outer);

	// Draw inner box
	QRect inner = QRect(
		QPoint(outer.left() + this->outerPaintMargin, outer.top() + this->outerPaintMargin),
	    QPoint(outer.right() - this->outerPaintMargin, outer.bottom() - this->outerPaintMargin)
	);
	QLinearGradient gradient(
		QPoint(inner.center().x(), inner.top()),
		QPoint(inner.center().x(), inner.bottom())
	);
	QColor brightestColour = this->palette().button().color();
	// Brighter bit
	gradient.setColorAt(0.000, brightestColour);
	gradient.setColorAt(0.200, brightestColour.darker(110));
	gradient.setColorAt(0.350, brightestColour.darker(130));
	gradient.setColorAt(0.400, brightestColour.darker(125));
	gradient.setColorAt(0.500, brightestColour.darker(150));
	// Darker bit
	gradient.setColorAt(0.600, brightestColour.darker(200));
	gradient.setColorAt(0.650, brightestColour.darker(170));
	gradient.setColorAt(0.750, brightestColour.darker(175));
	gradient.setColorAt(0.900, brightestColour.darker(215));
	gradient.setColorAt(1.000, brightestColour.darker(178));
	painter.setBrush(gradient);
	painter.drawPolygon(inner);

	// Draw text
	pen.setStyle(Qt::PenStyle::SolidLine);
	pen.setColor(this->palette().buttonText().color());
	painter.setPen(pen);
	QRect boundingRect;
	painter.drawText(
		inner.x(), inner.y(),
		inner.width(), inner.height(),
		Qt::AlignmentFlag::AlignCenter, this->text(),
		&boundingRect
	);
}

void APBPushButton::setButtonColour(const QColor &colour) {
	this->baseColour = colour;
}

void APBPushButton::setLighterColour(QPalette* palette) {
	palette->setColor(QPalette::ColorRole::Button, palette->button().color().lighter(118));
	this->setPalette(*palette);
}

void APBPushButton::setDarkerColour(QPalette* palette) {
	palette->setColor(QPalette::ColorRole::Button, palette->button().color().darker(118));
	this->setPalette(*palette);
}

void APBPushButton::setHighlight(QPalette* palette) {
	palette->setColor(QPalette::ColorRole::Highlight, this->orange);
	palette->setColor(QPalette::ColorRole::Button, this->black);
	palette->setColor(QPalette::ColorRole::ButtonText, this->orange);
	this->setPalette(*palette);
}

void APBPushButton::restoreDefaults(QPalette* palette) {
	palette->setColor(QPalette::ColorRole::Highlight, this->black);
	palette->setColor(QPalette::ColorRole::Button, this->baseColour);
	palette->setColor(QPalette::ColorRole::ButtonText, this->lightGrey);
	this->setPalette(*palette);
}



