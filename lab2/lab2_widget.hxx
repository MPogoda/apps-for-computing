#ifndef LAB2_WIDGET_H
#define LAB2_WIDGET_H

class QWidget;
class QLabel;
class QDoubleSpinBox;
class QSpinBox;
class QwtPlot;
class QwtPlotCurve;
class QwtLegend;

#include <QVector>
#include <functional>
#include "../describable/describable.hxx"

class Lab2_Widget : public Describable
{
    Q_OBJECT

public:
    Lab2_Widget(QWidget *parent = nullptr);
    virtual ~Lab2_Widget();
private:
    QLabel         *integral_num;
    QLabel         *integral_real;
    QLabel         *integral_diff;
    QDoubleSpinBox *a_spinbox;
    QDoubleSpinBox *b_spinbox;
    QSpinBox       *n_spinbox;

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
