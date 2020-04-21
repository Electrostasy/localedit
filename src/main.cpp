#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
	// QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
	// QApplication::setApplicationName("Localedit");

	QApplication app(argc, argv);
	auto window = new MainWindow();
	window->show();

	return QApplication::exec();
}
