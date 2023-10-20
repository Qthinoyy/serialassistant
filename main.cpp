#include "serialmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    serialmain w;
    w.show();
    return a.exec();
}
