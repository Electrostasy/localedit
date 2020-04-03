#include "ExpandedTextEdit.h"

ExpandedTextEdit::ExpandedTextEdit(QWidget *parent) {
	this->setParent(parent);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QWidget::customContextMenuRequested, this, &ExpandedTextEdit::showContextMenu);
}

void ExpandedTextEdit::showContextMenu(const QPoint &pos) {
	QPoint clickPos = this->mapToGlobal(pos);

	auto *menu = new QMenu();
	QAction *copy = menu->addAction("Copy");
	QAction *paste = menu->addAction("Paste");
	QAction *cut = menu->addAction("Cut");
	menu->addSeparator();
	QAction *redo = menu->addAction("Redo");
	QAction *undo = menu->addAction("Undo");
	menu->addSeparator();
	QAction *insertNewline = menu->addAction("Insert ↵");

	connect(copy, &QAction::triggered, this, &QTextEdit::copy);
	connect(paste, &QAction::triggered, this, &QTextEdit::paste);
	connect(cut, &QAction::triggered, this, &QTextEdit::cut);
	connect(redo, &QAction::triggered, this, &QTextEdit::redo);
	connect(undo, &QAction::triggered, this, &QTextEdit::undo);
	connect(insertNewline, &QAction::triggered, this, [=]() {
		this->textCursor().insertText("↵");
	});

	menu->exec(clickPos);
	delete menu;
}
