#include "MainWindow.hpp"
#include <QApplication>
#include <QIcon>
#include <QSize>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Calculator");
    app.setApplicationDisplayName("Calculator");
    app.setDesktopFileName("calculator-gui");

    /* Every size goes into the one QIcon rather than just the largest:
       a window manager picks the closest match and scales it, and a
       16px title-bar icon downscaled from 256px loses the keypad grid
       that makes the shape read as a calculator at all. The PNGs are
       embedded via Gui/resources.qrc, so this works from any working
       directory. */
    QIcon icon;
    for (int size : {16, 24, 32, 48, 64, 128, 256})
        icon.addFile(QString(":/icons/calculator-%1.png").arg(size), QSize(size, size));

    app.setWindowIcon(icon);

    MainWindow window;
    window.setMinimumSize(560, 560);
    window.resize(620, 610);
    window.show();

    return app.exec();
}
