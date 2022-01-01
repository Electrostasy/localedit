#pragma once

#include <QWidget>
#include <string>

class MessageBox {
	public:
	static QMessageBox UnsavedChangesBox(std::string windowTitle);
};
