#ifndef LAB4_WIDGET_H
#define LAB4_WIDGET_H

class QWidget;
#include "../describable/describable.hxx"

class Lab4_Widget : public Describable
{
    Q_OBJECT

public:
    Lab4_Widget(QWidget *parent = nullptr);
    virtual ~Lab4_Widget();
    virtual void reinit();
private:
private slots:
    void run();

};

#endif // LAB4_WIDGET_H
