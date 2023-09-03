#include "QtMultiThreadWithUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMultiThreadWithUI w;
    w.show();
    return a.exec();
}
