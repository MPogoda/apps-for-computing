#include "lab3_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab3_Widget w;
    w.show();

    return a.exec();
}
