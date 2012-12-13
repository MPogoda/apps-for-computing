#ifndef LAB5_WIDGET_H
#define LAB5_WIDGET_H

class QWidget;
#include "../describable/describable.hxx"

class Lab5_Widget : public Describable
{
    Q_OBJECT

public:
    Lab5_Widget(QWidget *parent = nullptr);
    virtual ~Lab5_Widget();
    virtual void reinit();
private:
private slots:
    void run();

};

#endif // LAB5_WIDGET_H
