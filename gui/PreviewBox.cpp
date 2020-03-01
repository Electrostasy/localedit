#include "PreviewBox.h"

PreviewBox::PreviewBox(QWidget* parent) {
	auto layout = new QVBoxLayout(this);
	auto header = new QFrame();
	header->setStyleSheet("QFrame { background-color: black; }");
	header->setGeometry(0, 0, 100, 50);
	layout->addWidget(header);
}
