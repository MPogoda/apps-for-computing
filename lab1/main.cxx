#include "lab1_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab1_Widget w;
    w.show();

    return a.exec();
}
