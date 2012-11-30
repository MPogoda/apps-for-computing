#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QVector>

class QWidget;
class QButtonGroup;
class QLabel;

#include "describable/describable.hxx"

class  Main_Widget : public QWidget
{
  Q_OBJECT
public:
    Main_Widget(QWidget *parent = nullptr);
    ~Main_Widget();

private:
    QButtonGroup *buttons;
    QLabel       *description;
    QVector< Describable * > labs;

    int current;
private slots:
    void launch_lab();
};

#endif
