#include "FramelessMainWindow.h"

FramelessMainWindow::FramelessMainWindow(QWidget *parent): QWidget(parent, Qt::WindowType::CustomizeWindowHint) {
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	this->setMouseTracking(true);

	auto *verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(1);

	auto *horizontalLayout = new QHBoxLayout();
	horizontalLayout->setSpacing(1);
	horizontalLayout->setMargin(0);

	this->titleBar = new QWidget();
	this->titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	this->titleBar->setLayout(horizontalLayout);

	this->windowLabel = new QLabel(this->titleBar);
	this->windowLabel->setText(this->windowTitle());

	this->minimizeButton = new APBPushButton(this->titleBar);
	connect(this->minimizeButton, &APBPushButton::clicked, this, &FramelessMainWindow::minimize);

	this->restoreButton = new APBPushButton(this->titleBar);
	this->restoreButton->setVisible(false);
	connect(this->restoreButton, &APBPushButton::clicked, this, &FramelessMainWindow::restore);

	this->maximizeButton = new APBPushButton(this->titleBar);
	connect(this->maximizeButton, &APBPushButton::clicked, this, &FramelessMainWindow::maximize);

	this->closeButton = new APBPushButton(this->titleBar);
	this->closeButton->setButtonColour(QColor(216, 47, 53));
	connect(this->closeButton, &APBPushButton::clicked, this, &QWidget::close);

	horizontalLayout->addWidget(this->windowLabel);
	horizontalLayout->addStretch(1);
	horizontalLayout->addWidget(this->minimizeButton);
	horizontalLayout->addWidget(this->restoreButton);
	horizontalLayout->addWidget(this->maximizeButton);
	horizontalLayout->addWidget(this->closeButton);

	this->contents = new QMainWindow();
	contents->setWindowFlags(Qt::FramelessWindowHint);

	verticalLayout->addWidget(this->titleBar);
	verticalLayout->addWidget(this->contents);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->setLayout(verticalLayout);
}

QMainWindow *FramelessMainWindow::content() {
	return this->contents;
}

void FramelessMainWindow::mousePressEvent(QMouseEvent *event) {
	if(!this->titleBar->underMouse() && !this->windowLabel->underMouse()) {
		return;
	}

	if(event->button() == Qt::MouseButton::LeftButton) {
		this->moving = true;
		this->lastMousePos = event->pos();
	}
}

void FramelessMainWindow::mouseMoveEvent(QMouseEvent *event) {
	if(!this->titleBar->underMouse() && !this->windowLabel->underMouse()) {
		return;
	}

	if(event->buttons().testFlag(Qt::MouseButton::LeftButton) && this->moving) {
		this->move(this->pos() + (event->pos() - this->lastMousePos));
	}
}

void FramelessMainWindow::mouseReleaseEvent(QMouseEvent *event) {
	if(!this->titleBar->underMouse() && !this->windowLabel->underMouse()) {
		return;
	}

	if(event->button() == Qt::MouseButton::LeftButton) {
		this->moving = false;
	}
}

void FramelessMainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	Q_UNUSED(event)
}

void FramelessMainWindow::titleBarDoubleClick() {
	if(this->windowState().testFlag(Qt::WindowNoState)) {
		this->maximize();
	} else if(this->windowState().testFlag(Qt::WindowFullScreen)) {
		this->restore();
	}
}

// Slots
void FramelessMainWindow::setWindowTitle(const QString &text) {
	this->contents->setWindowTitle(text);
}

void FramelessMainWindow::setWindowIcon(const QIcon &icon) {
	this->contents->setWindowIcon(icon);
}

void FramelessMainWindow::minimize() {
	this->setWindowState(Qt::WindowState::WindowMinimized);
}

void FramelessMainWindow::restore() {
	this->restoreButton->setVisible(false);
	this->maximizeButton->setVisible(true);
	this->setWindowState(Qt::WindowState::WindowNoState);
	// setStyleSheet("#borderlessMainWindow{border:1px solid palette(highlight);}");
}

void FramelessMainWindow::maximize() {
	this->restoreButton->setVisible(true);
	this->maximizeButton->setVisible(false);
	this->setWindowState(Qt::WindowState::WindowMaximized);
	this->showMaximized();
	// setStyleSheet("#borderlessMainWindow{border:1px solid palette(base);}");
}
