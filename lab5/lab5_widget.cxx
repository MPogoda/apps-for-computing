#include "lab5_widget.hxx"
#include <QSize>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>

Lab4_Widget::Lab4_Widget(QWidget *parent)
    : Describable(QString::null,
        parent)
{
    setMinimumSize(QSize(431, 407));
}

void Lab4_Widget::run()
{

}

Lab4_Widget::~Lab4_Widget()
{
}

void Lab4_Widget::reinit()
{
}
