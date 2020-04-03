#pragma once

#include <QWidget>
#include <QtWidgets>

class ExpandedTextEdit: public QTextEdit {
public:
	explicit ExpandedTextEdit(QWidget *parent = nullptr);

public slots:
	void showContextMenu(const QPoint& pos);
};
