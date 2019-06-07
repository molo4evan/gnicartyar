#include "gui/mainwindow.h"
#include <QApplication>
#include <model/primitives/plane.h>
#include <model/primitives/quadrangle.h>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    } catch (char const *ex) {
        qDebug() << ex;
        throw ex;
    }
}
