#pragma once

#include <QWidget>
#include <QtWidgets>

#include "SyntaxHighlighter.h"

class ExpandedTextEdit: public QPlainTextEdit {
	public:
	explicit ExpandedTextEdit(QTextDocument *document);

	public slots:
	void showContextMenu(const QPoint &pos);
};
