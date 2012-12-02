#ifndef LAB3_WIDGET_H
#define LAB3_WIDGET_H

class QWidget;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;
class QwtPlot;
class QwtPlotCurve;
class QwtLegend;

/**
 * for std::function
 */
#include <functional>
/**
 * for Describable interface, obv
 */
#include "../describable/describable.hxx"

/**
 * @brief ternary_f     typename for function of 3 qreal arguements returning qreal
 */
typedef std::function< qreal( qreal, qreal, qreal) >
        ternary_f;

/**
 * @brief The Lab3_Widget class
 * Substitute second-order equation with following system
 * 1) y' = g(x, y, z) = z
 * 2) z' = f(x, y, z) = 2*y/(x^2)
 *
 * And solving them simultaneously >_>
 * Sadly but initial state was not fully specified, so I've been forced to allow
 * input of z0.
 *
 * So, initial states are
 *  1) y(1) = 0.83
 *  2) y'(1) = z0
 */
class Lab3_Widget : public Describable
{
    Q_OBJECT

public:
    /**
     * @brief Default ctor for QObject childs >_>
     * @param parent    Parent of this widget
     */
    Lab3_Widget(QWidget *parent = nullptr);
    /**
     * @brief Destructor :>
     */
    virtual ~Lab3_Widget();
private:
    /**
     * @brief z0_spinbox    Input for y'(1)
     */
    QDoubleSpinBox *z0_spinbox;
    /**
     * @brief n_spinbox     number of intervals
     */
    QSpinBox       *n_spinbox;
    /**
     * @brief error1    Label containing error of 3rd-order Runge-Kutta method
     */
    QLabel         *error1;
    /**
     * @brief error2    Label containing error of 4th-order Runge-Kutta method
     */
    QLabel         *error2;

    /**
     * @brief plot      An instance of plotting widget
     */
    QwtPlot        *plot;
    /**
     * @brief curve1    curve containing points we've got using 3rd-order Runge-Kutta method
     */
    QwtPlotCurve   *curve1;
    /**
     * @brief curve2    curve containing points we've got using 4th-order Runge-Kutta method
     */
    QwtPlotCurve   *curve2;
    /**
     * @brief curve3    curve containing points we've got using real answer of this equation
     */
    QwtPlotCurve   *curve3;
    /**
     * @brief legend    Legend all of those curves
     */
    QwtLegend      *legend;

    /**
     * @brief reinit    reimplemented method from Describable interface. It's used to restore init state of widget
     */
    virtual void reinit();

    /**
     * @brief third_order   Implemented third-order Runge-Kutta method
     * @param x     starting X
     * @param h     step
     * @param b     ending X
     * @param y     starting Y
     * @param z     starting Z
     * @param f     function f, see comment to Lab3_Widget
     * @param g     function g, see comment to Lab3_Widget
     * @return Vectors of Yvalues in all points in [x, x+h, x+2h, ..., b]
     */
    const QVector< qreal > third_order(qreal x, const qreal h, const qreal b,
                                 qreal y, qreal z,
                                 const ternary_f& f, const ternary_f& g) const;
    /**
     * @brief fourth_order   Implemented fourth-order Runge-Kutta method
     * @param x     starting X
     * @param h     step
     * @param b     ending X
     * @param y     starting Y
     * @param z     starting Z
     * @param f     function f, see comment to Lab3_Widget
     * @param g     function g, see comment to Lab3_Widget
     * @return Vectors of Yvalues in all points in [x, x+h, x+2h, ..., b]
     */
    const QVector< qreal > fourth_order(qreal x, const qreal h, const qreal b,
                                 qreal y, qreal z,
                                 const ternary_f& f, const ternary_f& g) const;

    /**
     * @brief epsilon     Accepted error <_<
     */
    static const qreal epsilon;

public slots:
    void run();
};

#endif // LAB3_WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
