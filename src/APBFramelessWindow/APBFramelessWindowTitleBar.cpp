#include "APBFramelessWindowTitleBar.h"

APBFramelessWindowTitleBar::APBFramelessWindowTitleBar(QWidget *parent): QWidget(parent) {
	this->setMouseTracking(true);

	auto *horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	this->setLayout(horizontalLayout);

	this->windowTitle = new QLabel(this);

	this->minimizeButton = new APBPushButton(this);
	this->minimizeButton->setTitleIcon(APBPushButton::TitleIcon::Minimize);

	this->restoreButton = new APBPushButton(this);
	this->restoreButton->setTitleIcon(APBPushButton::TitleIcon::Restore);
	this->restoreButton->setVisible(false);

	this->maximizeButton = new APBPushButton(this);
	this->maximizeButton->setTitleIcon(APBPushButton::TitleIcon::Maximize);

	this->closeButton = new APBPushButton(this);
	this->closeButton->setButtonColour(QColor(216, 47, 53));
	this->closeButton->setTitleIcon(APBPushButton::TitleIcon::Close);
	this->closeButton->blinkOnHover(true);

	connect(this->minimizeButton, &APBPushButton::clicked, this, &APBFramelessWindowTitleBar::minimizeButtonClicked);
	connect(this->restoreButton, &APBPushButton::clicked, this, &APBFramelessWindowTitleBar::restoreButtonClicked);
	connect(this->maximizeButton, &APBPushButton::clicked, this, &APBFramelessWindowTitleBar::maximizeButtonClicked);
	connect(this->closeButton, &APBPushButton::clicked, this, &APBFramelessWindowTitleBar::closeButtonClicked);

	horizontalLayout->addWidget(this->windowTitle);
	horizontalLayout->addStretch(1);
	horizontalLayout->addWidget(this->minimizeButton);
	horizontalLayout->addWidget(this->restoreButton);
	horizontalLayout->addWidget(this->maximizeButton);
	horizontalLayout->addWidget(this->closeButton);
	horizontalLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
}

void APBFramelessWindowTitleBar::setTitleBarText(const QString &text) {
	this->windowTitle->setText(text);
}

QLabel *APBFramelessWindowTitleBar::titleBarLabel() {
	return this->windowTitle;
}

void APBFramelessWindowTitleBar::paintEvent(QPaintEvent *paintEvent) {
	auto *painter = new QPainter(this);

	painter->setBrush(QColor(0, 0, 0));
	painter->drawRect(paintEvent->rect());

	painter->end();
	QWidget::paintEvent(paintEvent);
}

int APBFramelessWindowTitleBar::buttonWidth() const {
	const int minimizeButtonWidth = this->minimizeButton->isVisible() ? this->minimizeButton->width() : 0;
	const int restoreButtonWidth = this->restoreButton->isVisible() ? this->minimizeButton->width() : 0;
	const int maximizeButtonWidth = this->maximizeButton->isVisible() ? this->minimizeButton->width() : 0;
	const int closeButtonWidth = this->closeButton->isVisible() ? this->minimizeButton->width() : 0;
	return minimizeButtonWidth + restoreButtonWidth + maximizeButtonWidth + closeButtonWidth;
}

int APBFramelessWindowTitleBar::labelWidth() const {
	return this->windowTitle->width();
}
