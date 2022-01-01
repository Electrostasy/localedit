#pragma once

#include <QString>
#include <QWidget>
#include <string>

class MessageBox {
	public:
	static QMessageBox UnsavedChangesBox(const QString windowTitle);
};
