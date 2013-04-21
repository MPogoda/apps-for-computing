#include "lab6_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab6_Widget w;
    w.show();

    return a.exec();
}
