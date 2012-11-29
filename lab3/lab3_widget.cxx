#include "lab3_widget.hxx"
#include <QVector>
#include <QSize>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <algorithm>
#include <iostream>
#include <numeric>

#include <QHBoxLayout>
#include <QVBoxLayout>
// x^2 y'' -2y = 0
// [1, 2]
// y(1) = 0.83
// h = 0.1

Lab3_Widget::Lab3_Widget(QWidget *parent)
    : Describable(trUtf8("Численное интегрирование методом центральных прямоугольников:\n"
                         "1. Промежуток интегрирования разбивается на n интервалов.\n"
                         "2. Значение функции на каждом промежутке принимается равным\n"
                         "      значению функции в центре промежутка Fi.\n"
                         "3. Интеграл считается как сумма Fi * h\n\n"
                         "Численное дифференцирование сводится к использованию определения\n"
                         "    производной, но не устремляя шаг к 0."), parent),

      plot1(new QwtPlot(this)),
      curve1(new QwtPlotCurve("diff")),
      curve2(new QwtPlotCurve("real")),
      legend(new QwtLegend(plot1))
{
}

Lab3_Widget::~Lab3_Widget()
{
    delete curve1;
    delete curve2;
}

void Lab3_Widget::reinit()
{
    curve1->hide();
    curve2->hide();
    plot1->replot();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
