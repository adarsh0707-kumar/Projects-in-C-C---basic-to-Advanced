#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setMinimumSize(560, 560);
    window.resize(620, 610);
    window.show();

    return app.exec();
}
