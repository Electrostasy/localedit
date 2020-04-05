#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
	QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);

	QApplication app(argc, argv);
	auto window = new MainWindow();
	window->show();

	return QApplication::exec();
}
