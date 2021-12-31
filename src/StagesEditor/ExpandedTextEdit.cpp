#include "ExpandedTextEdit.h"

ExpandedTextEdit::ExpandedTextEdit(QTextDocument *document) {
	this->setDocument(document);
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	// Attach a syntax highlighter
	if(this->document()->findChild<QSyntaxHighlighter *>() == nullptr) {
		new SyntaxHighlighter(this->document());
	}

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

	connect(copy, &QAction::triggered, this, &QPlainTextEdit::copy);
	connect(paste, &QAction::triggered, this, &QPlainTextEdit::paste);
	connect(cut, &QAction::triggered, this, &QPlainTextEdit::cut);
	connect(redo, &QAction::triggered, this, &QPlainTextEdit::redo);
	connect(undo, &QAction::triggered, this, &QPlainTextEdit::undo);
	connect(insertNewline, &QAction::triggered, this, [=,*this]() {
		this->textCursor().insertText("↵");
	});

	menu->exec(clickPos);
	delete menu;
}
