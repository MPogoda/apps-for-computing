#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVector>
#include <QButtonGroup>

#include "describable.hxx"

class  Main_Widget : public QWidget
{
  Q_OBJECT
public:
    Main_Widget(QWidget *parent = nullptr);

private:
    QButtonGroup *buttons;
    QLabel       *description;
    QVector<Describable *> labs;

    int current;
private slots:
    void launch_lab();
};

#endif
