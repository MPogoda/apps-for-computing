#include "lab4_widget.hxx"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lab4_Widget w;
    w.show();

    return a.exec();
}
