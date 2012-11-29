#ifndef LAB3_WIDGET_H
#define LAB3_WIDGET_H

#include <QWidget>
#include <QVector>
#include <functional>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>

class QwtPlot;
class QwtPlotCurve;
class QwtLegend;
#include "../describable/describable.hxx"

class Lab3_Widget : public Describable
{
    Q_OBJECT

public:
    Lab3_Widget(QWidget *parent = nullptr);
    ~Lab3_Widget();
private:


    QwtPlot        *plot1;
    QwtPlotCurve   *curve1;
    QwtPlotCurve   *curve2;
    QwtLegend      *legend;

    virtual void reinit();
};

#endif // LAB3_WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
