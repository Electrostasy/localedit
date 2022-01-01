#include "APBPushButton.h"
#include <tuple>

APBPushButton::APBPushButton(QWidget *parent): QPushButton(parent) {
	buttonFont.setStyleHint(QFont::StyleHint::Helvetica, QFont::StyleStrategy::PreferAntialias);
	buttonFont.setWeight(QFont::Weight::DemiBold);
	buttonFont.setCapitalization(QFont::Capitalization::Capitalize);
	buttonFont.setPixelSize(16);

	this->orange = QColor(227, 182, 109);
	this->black = QColor(0, 0, 0);
	this->lightGrey = QColor(216, 216, 216);
	this->baseColour = QColor(137, 137, 137);

	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::Highlight, this->black);
	palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::Button, this->baseColour);
	palette.setColor(QPalette::ColorGroup::Normal, QPalette::ColorRole::ButtonText, this->lightGrey);
	palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Highlight, QColor(50, 50, 50));
	palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, QColor(100, 100, 100));
	palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, QColor(120, 120, 120));
	this->setPalette(palette);
}

APBPushButton::APBPushButton(const QString &text, QWidget *parent): APBPushButton(parent) {
	this->setText(text);
}

bool APBPushButton::event(QEvent *event) {
	QPalette palette = this->palette();

	switch(event->type()) {
		case QEvent::Type::HoverEnter:
			if(this->isEnabled()) {
				// If a QTimeLine object exists, start the blink timer, otherwise just make it brighter
				if(this->timer != nullptr) {
					this->startTimer(&palette);
				} else {
					this->setLighterColour(&palette);
				}
			}

			break;
		case QEvent::Type::HoverLeave:
			if(this->isEnabled()) {
				// If a QTimeLine object exists, stop the blink timer, otherwise just make it darker
				if(this->timer != nullptr) {
					this->stopTimer(&palette);
				} else {
					this->setDarkerColour(&palette);
				}
			}

			break;
		case QEvent::MouseButtonPress:
			if(this->isEnabled()) {
				// If a QTimeLine object exists, stop the blink timer to retain highlight colours
				if(this->timer != nullptr) {
					this->stopTimer(&palette);
				}
				this->setHighlight(&palette);
			}

			break;
		case QEvent::MouseButtonDblClick:
			if(this->isEnabled()) {
				// If a QTimeLine object exists, stop the blink timer to retain highlight colours
				if(this->timer != nullptr) {
					this->stopTimer(&palette);
				}
				this->setHighlight(&palette);
			}

			break;
		case QEvent::MouseButtonRelease:
			if(this->isEnabled()) {
				this->restoreDefaults(&palette);
				// If a QTimeLine object exists, start the blink timer instead of reverting to static colour
				if(this->timer != nullptr) {
					this->startTimer(&palette);
				}
			}

			break;

    default: {} // Other events are not handled by this switch-statement
	}

	return QPushButton::event(event);
}

void APBPushButton::setButtonColour(const QColor &colour) {
	this->baseColour = colour;

	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Button, colour);
	this->setPalette(palette);
}

void APBPushButton::blinkOnHover(bool blink) {
	if(blink) {
		this->timer = new QTimeLine(1000, this);
		this->timer->setFrameRange(0, 100);
		this->timer->setUpdateInterval(25);
		this->timer->setLoopCount(0);
		connect(this->timer, &QTimeLine::valueChanged, this, &APBPushButton::cycleButtonColour);
	} else {
		this->timer->deleteLater();
	}
}

// TODO FIX: Moving mouse off and then back on retains the value, occasionally stutters
void APBPushButton::cycleButtonColour(double newValue) {
	if(this->loopCounter == this->timer->duration() / this->timer->updateInterval() / 2) {
		this->timer->toggleDirection();
		this->loopCounter = 0;
	}

	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Button, this->baseColour.lighter(100 + qFloor(19.0 * (1 - newValue))));
	this->setPalette(palette);

	this->loopCounter++;
}

void APBPushButton::startTimer(QPalette *palette) {
	this->resetTimer();
	this->restoreDefaults(palette);

	this->timer->start();
}

void APBPushButton::stopTimer(QPalette *palette) {
	this->timer->stop();

	this->resetTimer();
	this->restoreDefaults(palette);
}

void APBPushButton::resetTimer() {
	this->loopCounter = 0;
	this->timer->setStartFrame(0);
	this->timer->setCurrentTime(0);
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

void APBPushButton::paintEvent(QPaintEvent *event) {
	auto *painter = new QPainter(this);
	painter->setFont(this->buttonFont);
	painter->setRenderHints(QPainter::Antialiasing, QPainter::TextAntialiasing);
	painter->setBrush(this->palette().highlight());

	this->setPen(painter);

	auto [outer, inner] = this->drawBox(event);

	painter->drawPolygon(outer);
	painter->setBrush(this->createGradient(inner));
	painter->drawPolygon(inner);

	this->drawTitleBarIcon(painter, inner);

	painter->end();
}

void APBPushButton::drawTitleBarMinimize(QPainter *painter, const QRect &bounds) {
	QPen pen = painter->pen();
	pen.setWidthF(0.5 + outerPaintMargin);
	painter->setPen(pen);

	int offsetCenter = (bounds.bottom() - bounds.top()) / 2;
	painter->drawLine(
		// Center left
		QPoint(
			bounds.center().x() - offsetCenter + innerPaintMargin,
			bounds.center().y()
		),
		// Center right
		QPoint(
			bounds.center().x() + offsetCenter - innerPaintMargin,
			bounds.center().y()
		)
	);
}

void APBPushButton::drawTitleBarMaximize(QPainter *painter, const QRect &bounds) {
	QPen pen = painter->pen();
	pen.setWidthF(0.5 + outerPaintMargin);
	painter->setPen(pen);

	int offsetCenter = (bounds.bottom() - bounds.top()) / 2;
	// From top left corner to bottom right corner
	painter->drawRect(
		QRect(
			// Top left
			QPoint(
				bounds.center().x() - offsetCenter + innerPaintMargin,
				bounds.center().y() - offsetCenter + innerPaintMargin
			),
			// Bottom right
			QPoint(
				bounds.center().x() + offsetCenter - innerPaintMargin,
				bounds.center().y() + offsetCenter - innerPaintMargin
			)
		)
	);
}

void APBPushButton::drawTitleBarClose(QPainter *painter, const QRect &bounds) {
	QPen pen = painter->pen();
	pen.setWidthF(1.75 + outerPaintMargin);
	painter->setPen(pen);

	int offsetCenter = (bounds.bottom() - bounds.top()) / 2;
	// From top left corner to bottom right corner
	painter->drawLine(
		// Top left
		QPoint(
			bounds.center().x() - offsetCenter + innerPaintMargin,
			bounds.center().y() - offsetCenter + innerPaintMargin
		),
		// Bottom right
		QPoint(
			bounds.center().x() + offsetCenter - innerPaintMargin,
			bounds.center().y() + offsetCenter - innerPaintMargin
		)
	);
	// From top right corner to bottom left corner
	painter->drawLine(
		// Top right
		QPoint(
			bounds.center().x() + offsetCenter - innerPaintMargin,
			bounds.center().y() - offsetCenter + innerPaintMargin
		),
		// Bottom left
		QPoint(
			bounds.center().x() - offsetCenter + innerPaintMargin,
			bounds.center().y() + offsetCenter - innerPaintMargin
		)
	);
}

void APBPushButton::drawText(QPainter *painter, const QRect &bounds) {
	QPen pen = painter->pen();
	pen.setStyle(Qt::PenStyle::SolidLine);
	painter->setPen(pen);

	if(!this->text().isEmpty()) {
		QRect boundingRect;
		painter->drawText(
			bounds.x(), bounds.y(),
			bounds.width(), bounds.height(),
			Qt::AlignmentFlag::AlignCenter, this->text(),
			&boundingRect
		);
	}
}

void APBPushButton::setTitleIcon(TitleIcon icon) {
	this->titleBarIconType = icon;
}

std::tuple<QRect, QRect> APBPushButton::drawBox(QPaintEvent* event) {
	// Draw outer box
	QRect outer = event->rect();
	// Draw inner box
	QRect inner = QRect(QPoint(outer.left() + this->outerPaintMargin, outer.top() + this->outerPaintMargin),
		QPoint(outer.right() - this->outerPaintMargin, outer.bottom() - this->outerPaintMargin));

	return {
		outer, inner
	};
}

void APBPushButton::setPen(QPainter *painter) {
	QPen pen = painter->pen();
	pen.setStyle(Qt::PenStyle::NoPen);	  // Remove borders
	painter->setPen(pen);
	pen.setBrush(this->palette().buttonText());
	pen.setStyle(Qt::PenStyle::SolidLine);
	pen.setJoinStyle(Qt::PenJoinStyle::MiterJoin);
	painter->setPen(pen);
}

QLinearGradient APBPushButton::createGradient(const QRect &inner) {
	QColor brightestColour = this->palette().button().color();
	QLinearGradient gradient(QPoint(inner.center().x(), inner.top()), QPoint(inner.center().x(), inner.bottom()));

	// Top (brighter) half
	gradient.setColorAt(0.000, brightestColour);
	gradient.setColorAt(0.200, brightestColour.darker(110));
	gradient.setColorAt(0.350, brightestColour.darker(130));
	gradient.setColorAt(0.400, brightestColour.darker(125));
	gradient.setColorAt(0.500, brightestColour.darker(150));

	// Bottom (darker) half
	gradient.setColorAt(0.600, brightestColour.darker(200));
	gradient.setColorAt(0.650, brightestColour.darker(170));
	gradient.setColorAt(0.750, brightestColour.darker(175));
	gradient.setColorAt(0.900, brightestColour.darker(215));
	gradient.setColorAt(1.000, brightestColour.darker(178));

	return gradient;
}

void APBPushButton::drawTitleBarIcon(QPainter *painter, const QRect &bounds) {
	switch(this->titleBarIconType) {
		case TitleIcon::None:
			this->drawText(painter, bounds);
			break;
		case TitleIcon::Minimize:
			this->drawTitleBarMinimize(painter, bounds);
			break;
		case TitleIcon::Maximize:
			this->drawTitleBarMaximize(painter, bounds);
			break;
		case TitleIcon::Restore:
			this->drawTitleBarMaximize(painter, bounds);
			break;
		case TitleIcon::Close:
			this->drawTitleBarClose(painter, bounds);
			break;
	}
}
