#include "lab5_widget.hxx"
#include <QSize>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

Lab5_Widget::Lab5_Widget(QWidget *parent)
    : Describable(QString::null,
        parent)
{
    setMinimumSize(QSize(431, 407));
}

void Lab5_Widget::run()
{

}

Lab5_Widget::~Lab5_Widget()
{
}

void Lab5_Widget::reinit()
{
}
