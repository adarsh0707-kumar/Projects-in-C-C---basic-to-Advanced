#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setMinimumSize(520, 420);
    window.resize(560, 460);
    window.show();

    return app.exec();
}
