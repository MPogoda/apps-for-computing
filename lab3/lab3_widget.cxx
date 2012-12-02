#include "lab3_widget.hxx"
#include <QVector>
#include <QSize>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <algorithm>
#include <numeric>
#include <functional>

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

const qreal Lab3_Widget::epsilon = 0.00001;

Lab3_Widget::Lab3_Widget(QWidget *parent)
    : Describable(trUtf8("Метод Рунге-Кутты третьего порядка применяется для\n"
                         "численного решения задачи Коши.\n"
                         "Для этого необходимо итеративно вычислять коэффициенты\n"
                         "k1, k2, k3, q1, q2, q3\n"
                         "А через них вычислять, соотвественно, y'_{n+1} и y_{n+1}\n"),
        parent),
      z0_spinbox(new QDoubleSpinBox(this)),
      n_spinbox(new QSpinBox(this)),
      error1(new QLabel(this)),
      error2(new QLabel(this)),
      plot(new QwtPlot(this)),
      curve1(new QwtPlotCurve("3rd order")),
      curve2(new QwtPlotCurve("4th order")),
      curve3(new QwtPlotCurve("Real")),
      legend(new QwtLegend(plot))
{
    // top row
    QHBoxLayout *top_layout(new QHBoxLayout(nullptr));
    QLabel *z0_label(new QLabel(trUtf8("y'(1) = "), this));
    z0_label->setBuddy(z0_spinbox);
    z0_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    z0_spinbox->setMinimum(0.1);
    z0_spinbox->setMaximum(2.0);
    z0_spinbox->setValue(1.0);
    z0_spinbox->setSingleStep(0.1);

    QLabel *n_label(new QLabel(trUtf8("n: "), this));
    n_label->setBuddy(n_spinbox);
    n_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    n_spinbox->setMinimum(5);
    n_spinbox->setMaximum(500);
    n_spinbox->setValue(9);

    QPushButton *button(new QPushButton(trUtf8("Run"), this));
    connect(button, SIGNAL(clicked()), this, SLOT(run()));

    top_layout->addWidget(z0_label);
    top_layout->addWidget(z0_spinbox);
    top_layout->addWidget(n_label);
    top_layout->addWidget(n_spinbox);
    top_layout->addWidget(button);

    //Middle row
    QHBoxLayout *middle_layout(new QHBoxLayout(nullptr));
    QLabel *error1_description(new QLabel(trUtf8("Avg error of 3rd-order method: "), this));
    error1_description->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    error1_description->setBuddy(error1);

    error1->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    QLabel *error2_description(new QLabel(trUtf8("Avg error of 4th-order method: "), this));
    error2_description->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    error2_description->setBuddy(error2);

    error2->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    middle_layout->addWidget(error1_description);
    middle_layout->addWidget(error1);
    middle_layout->addWidget(error2_description);
    middle_layout->addWidget(error2);


    QVBoxLayout *main_layout(new QVBoxLayout(this));

    curve1->setPen(QPen(Qt::blue));
    curve2->setPen(QPen(Qt::green));
    curve3->setPen(QPen(Qt::red));
    curve1->attach(plot);
    curve2->attach(plot);
    curve3->attach(plot);

    plot->insertLegend(legend);

    main_layout->addLayout(top_layout);
    main_layout->addLayout(middle_layout);
    main_layout->addWidget(plot);

    setMinimumSize(QSize(431, 407));
}

Lab3_Widget::~Lab3_Widget()
{
    delete curve1;
    delete curve2;
    delete curve3;
    delete legend;
}

void Lab3_Widget::reinit()
{
    z0_spinbox->setValue(1.0);
    error1->clear();
    error2->clear();

    curve1->hide();
    curve2->hide();
    curve3->hide();
    plot->replot();
}

const QVector< qreal > Lab3_Widget::third_order(qreal x, const qreal h, const qreal b,
                                                qreal y, qreal z,
                                                const ternary_f& f, const ternary_f& g) const
{
    QVector< qreal > ys;
    QVector< qreal > zs;

    for (; b + epsilon > x; x += h)
    {
        ys << y;
        zs << z;

        const qreal k1 = h * f(x, y, z);
        const qreal q1 = h * g(x, y, z);

        const qreal k2 = h * f(x + h/3.0, y + q1/3.0, z + k1/3.0);
        const qreal q2 = h * g(x + h/3.0, y + q1/3.0, z + k1/3.0);

        const qreal k3 = h * f(x + 2.0*h/3.0, y + 2.0*q2/3.0, z + 2.0*k2/3.0);
        const qreal q3 = h * g(x + 2.0*h/3.0, y + 2.0*q2/3.0, z + 2.0*k2/3.0);

        z += 0.25 * (k1 + 3 * k3);
        y += 0.25 * (q1 + 3 * q3);

    }

    return ys;
}

const QVector< qreal > Lab3_Widget::fourth_order(qreal x, const qreal h, const qreal b,
                                                 qreal y, qreal z,
                                                 const ternary_f& f, const ternary_f& g) const
{
    QVector< qreal > ys;
    QVector< qreal > zs;

    for (; b + epsilon > x; x += h)
    {
        ys << y;
        zs << z;

        const qreal k1 = h * f(x, y, z);
        const qreal q1 = h * g(x, y, z);

        const qreal k2 = h * f(x + 0.5*h, y + 0.5*q1, z + 0.5*k1);
        const qreal q2 = h * g(x + 0.5*h, y + 0.5*q1, z + 0.5*k1);

        const qreal k3 = h * f(x + 0.5*h, y + 0.5*q2, z + 0.5*k2);
        const qreal q3 = h * g(x + 0.5*h, y + 0.5*q2, z + 0.5*k2);

        const qreal k4 = h * f(x + h, y + q3, z + k3);
        const qreal q4 = h * g(x + h, y + q3, z + k3);

        z += (k1 + 2.0*k2 + 2.0*k3 + k4)/6.0;
        y += (q1 + 2.0*q2 + 2.0*q3 + q4)/6.0;
    }

    return ys;
}


void Lab3_Widget::run()
{
    /**
     * I've to solve equation withing interval [a; b] with step h
     */
    const static auto y0( 0.83 );
    const static auto a( 1.0 ) ;
    const static auto b( 2.0 );
    const auto h( (b - a) / (n_spinbox->value() + 1) );
    const static auto f = [](const double x, const double y, const double z)
    {
        return (2.0 * y) / (qPow(x, 2));
    };
    const static auto g = [](const double x, const double y, const double z)
    {
        return z;
    };


    const auto z0 = z0_spinbox->value();

    /**
     * Real answer to this equation, was calculated using wolframalpha
     */
    const auto F = [z0](const double x)
    {
        return (qPow(x, 3) * ((1.0/3.0) * z0 + 0.2766666) - (1.0/3.0) * z0 + 0.55333) / x;
    };

    // [a; a+h; a+2h; ...; b]
    QVector< qreal > xs;
    /**
     * First of all we've to fill Xs
     */
    for (auto x(a); b + epsilon > x; x += h)
        xs << x;

    const auto ys1( third_order(a, h, b, y0, z0, f, g) );
    const auto ys2( fourth_order(a, h, b, y0, z0, f, g) );
    auto ys(xs);
    // Get values of real values by transforming Xs to Ys using F
    std::transform(ys.begin(), ys.end(), ys.begin(), F);

    curve1->setSamples(xs, ys1);
    curve2->setSamples(xs, ys2);
    curve3->setSamples(xs, ys);

    curve1->show();
    curve2->show();
    curve3->show();

    plot->replot();

    auto error1( 0.0 );
    auto error2( 0.0);

    for (auto i( 0 ); ys.size() != i; ++i)
    {
        error1 += qPow(ys[i] - ys1[i], 2);
        error2 += qPow(ys[i] - ys2[i], 2);
    }

    error1 = qSqrt(error1);
    error2 = qSqrt(error2);

    this->error1->setText( QString::number(error1) );
    this->error2->setText( QString::number(error2) );
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
