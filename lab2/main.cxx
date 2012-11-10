#include "lab2_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab2_Widget w;
    w.show();

    return a.exec();
}
