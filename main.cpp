#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    window.setAcceptDrops(true);
    window.resize(320, 240);
    window.setWindowTitle("APB Mission Text Edit");
    window.show();

    return app.exec();
}