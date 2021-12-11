#include "ExpandingTabBar.h"

ExpandingTabBar::ExpandingTabBar(QWidget *parent): QTabBar(parent) {
	this->setMouseTracking(true);

	QFont tabFont;
	tabFont.setStyleHint(QFont::StyleHint::Helvetica, QFont::StyleStrategy::PreferAntialias);
	tabFont.setWeight(QFont::Weight::DemiBold);
	tabFont.setCapitalization(QFont::Capitalization::Capitalize);
	tabFont.setPixelSize(16);
	this->setFont(tabFont);

	QPalette palette = this->palette();
	palette.setColor(QPalette::ColorRole::Base, QColor(0, 0, 0));
	palette.setColor(QPalette::ColorRole::AlternateBase, QColor(30, 30, 30));
	palette.setColor(QPalette::ColorRole::Highlight, QColor(60, 60, 60));
	palette.setColor(QPalette::ColorRole::Text, QColor(188, 188, 188));
	palette.setColor(QPalette::ColorRole::HighlightedText, QColor(227, 182, 109));
	this->setPalette(palette);
}

bool ExpandingTabBar::event(QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
		// Keep track of which tab has mouseover for PaintEvent
		mouseoveredTab = this->tabAt(dynamic_cast<QMouseEvent *>(event)->pos());
  } else if (event->type() == QEvent::HoverLeave) {
		// MouseMove can't detect if we left the tab bar, so we use HoverLeave
		mouseoveredTab = -1;
		this->update();
  }

	return QTabBar::event(event);
}

void ExpandingTabBar::paintEvent(QPaintEvent *paintEvent) {
	auto *painter = new QPainter(this);
	QPen pen = painter->pen();

	for(int index = 0; index < this->count(); ++index) {
		this->setBrush(painter, index);

		QRect tabRect = this->createTabRect(index);
		painter->setPen(Qt::PenStyle::NoPen);	 // Remove borders by setting to no pen
		painter->drawRect(tabRect);

		// Draw text on tab
		if(!this->tabText(index).isEmpty()) {
			this->restorePen(pen, index);

			QRect boundingRect;
			painter->drawText(tabRect.x() + outerPaintMargin, tabRect.y() + outerPaintMargin,
				tabRect.width() - outerPaintMargin, tabRect.height() - outerPaintMargin, Qt::AlignmentFlag::AlignLeft,
				this->tabText(index), &boundingRect);
		}
	}

	painter->end();
}

void ExpandingTabBar::setBrush(QPainter* painter, int index) {
	if(index == this->currentIndex()) {
		painter->setBrush(this->palette().highlight());
	} else {
		// If current tab is under mouse, make it brighter
		if(index == mouseoveredTab) {
			painter->setBrush(this->palette().alternateBase());
		} else {
			painter->setBrush(this->palette().base());
		}
	}
}

QRect ExpandingTabBar::createTabRect(int index) {
	// Create gaps between tabs that aren't the first or last tab
	int leftMargin = index != 0 ? outerPaintMargin / 2 : 0;
	int rightMargin = index != this->count() - 1 ? outerPaintMargin / 2 : 0;
	QRect tabRect = QRect(this->tabRect(index).left() + leftMargin, this->tabRect(index).top(),
		this->tabRect(index).width() - rightMargin, this->tabRect(index).height());

	return tabRect;
}

void ExpandingTabBar::restorePen(QPen pen, int index) {
	// Restore pen to draw text
	pen.setStyle(Qt::PenStyle::SolidLine);
	auto penColor =
		index == this->currentIndex() ? this->palette().highlightedText().color() : this->palette().text().color();
	pen.setColor(penColor);
}
