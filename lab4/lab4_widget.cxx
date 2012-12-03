#include "lab4_widget.hxx"
#include <QSize>
#include <QString>

Lab4_Widget::Lab4_Widget(QWidget *parent)
    : Describable(trUtf8(QString::null),
        parent)
{

    setMinimumSize(QSize(431, 407));
}

Lab4_Widget::~Lab4_Widget()
{
}

void Lab4_Widget::reinit()
{
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
