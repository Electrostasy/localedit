#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    window.setAcceptDrops(true);
    window.setMinimumSize(700, 700);
    window.resize(900, 900);
    window.setWindowTitle("localedit");
    window.show();

    return app.exec();
}
