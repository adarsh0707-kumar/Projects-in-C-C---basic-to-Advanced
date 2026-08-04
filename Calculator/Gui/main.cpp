#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setMinimumSize(320, 420);
    window.show();

    return app.exec();
}
