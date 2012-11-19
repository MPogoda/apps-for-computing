#ifndef LAB2_WIDGET_H
#define LAB2_WIDGET_H

#include <QWidget>
#include <QVector>
#include <functional>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>

#include "../describable.hxx"

class Lab2_Widget : public Describable
{
    Q_OBJECT

public:
    Lab2_Widget(QWidget *parent = nullptr);
    ~Lab2_Widget();
private:
    QLabel         *l1;
    QLabel         *l2;
    QLabel         *l3;
    QLabel         *integral_num;
    QLabel         *integral_real;
    QLabel         *integral_diff;
    QDoubleSpinBox *a_spinbox;
    QDoubleSpinBox *b_spinbox;
    QSpinBox       *n_spinbox;
    QPushButton    *launch_button;

    QwtPlot        *plot1;
    QwtPlotCurve   *curve1;
    QwtPlotCurve   *curve2;
    QwtLegend      *legend;

    void setup_ui();

    void  create_xs(QVector< double >& xs, const double& a, const double& b,
                    const std::size_t n) const;
    void  diff(const std::function< double(double) >& func,
               QVector< double >& ys) const;
    qreal integrate(const std::function< double(double) >& func,
                    QVector< double > xs) const;

    virtual void reinit();
public slots:
    void calculate();
    void a_sn(double);
    void b_sn(double);
};

#endif // LAB2_WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
