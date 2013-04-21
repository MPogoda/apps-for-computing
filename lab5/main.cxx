#include "lab5_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab5_Widget w;
    w.show();

    return a.exec();
}
