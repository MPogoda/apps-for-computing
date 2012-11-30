#include "lab2_widget.hxx"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <QSize>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <algorithm>
#include <iostream>
#include <numeric>

#include <QHBoxLayout>
#include <QVBoxLayout>

Lab2_Widget::Lab2_Widget(QWidget *parent)
    : Describable(trUtf8("Численное интегрирование методом центральных прямоугольников:\n"
                         "1. Промежуток интегрирования разбивается на n интервалов.\n"
                         "2. Значение функции на каждом промежутке принимается равным\n"
                         "      значению функции в центре промежутка Fi.\n"
                         "3. Интеграл считается как сумма Fi * h\n\n"
                         "Численное дифференцирование сводится к использованию определения\n"
                         "    производной, но не устремляя шаг к 0."), parent),
      integral_num(new QLabel(this)),
      integral_real(new QLabel( this)),
      integral_diff(new QLabel( this)),
      a_spinbox(new QDoubleSpinBox(this)),
      b_spinbox(new QDoubleSpinBox(this)),
      n_spinbox(new QSpinBox(this)),
      launch_button(new QPushButton(trUtf8("Calculate!"), this)),
      plot1(new QwtPlot(this)),
      curve1(new QwtPlotCurve("diff")),
      curve2(new QwtPlotCurve("real")),
      legend(new QwtLegend(plot1))
{
    setup_ui();
}

Lab2_Widget::~Lab2_Widget()
{
    delete curve1;
    delete curve2;
}

void Lab2_Widget::setup_ui()
{
    // top layout (for spinboxes)
    QHBoxLayout *inputs_layout(new QHBoxLayout(nullptr));
    QLabel *l(nullptr);

      // add a_spinbox
      l = new QLabel(trUtf8("a: "), this);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      l->setBuddy(a_spinbox);
      a_spinbox->setDecimals(2);
      a_spinbox->setRange(0.5, 5.0);
      a_spinbox->setValue(1.0);
      a_spinbox->setSingleStep(0.1);

    inputs_layout->addWidget(l);
    inputs_layout->addWidget(a_spinbox);


      // b_spinbox
      l = new QLabel(trUtf8("b: "), this);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      l->setBuddy(b_spinbox);
      b_spinbox->setDecimals(2);
      b_spinbox->setRange(0.5, 5.0);
      b_spinbox->setValue(2.0);
      b_spinbox->setSingleStep(0.1);

    inputs_layout->addWidget(l);
    inputs_layout->addWidget(b_spinbox);

      // n_spinbox
      l = new QLabel(trUtf8("n: "), this);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      l->setBuddy(n_spinbox);
      n_spinbox->setRange(5, 5000);
      n_spinbox->setValue(50);
      n_spinbox->setSingleStep(5);

    inputs_layout->addWidget(l);
    inputs_layout->addWidget(n_spinbox);

    connect(a_spinbox, SIGNAL(valueChanged(double)), this, SLOT(b_sn(double)));
    connect(b_spinbox, SIGNAL(valueChanged(double)), this, SLOT(a_sn(double)));


    // middle layout (for outputs
    QHBoxLayout *labels_layout(new QHBoxLayout(nullptr));
      l = new QLabel(trUtf8("Calculated integral: "), this);
      l->setBuddy(integral_num);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      integral_num->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      labels_layout->addWidget(l);
      labels_layout->addWidget(integral_num);

      l = new QLabel(trUtf8("Real value of integral: "), this);
      l->setBuddy(integral_real);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      integral_real->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      labels_layout->addWidget(l);
      labels_layout->addWidget(integral_real);

      l = new QLabel(trUtf8("Error: "), this);
      l->setBuddy(integral_diff);
      l->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,
                                   QSizePolicy::Preferred));
      integral_diff->setFrameStyle( QFrame::Panel | QFrame::Sunken );
      labels_layout->addWidget(l);
      labels_layout->addWidget(integral_diff);

    // Qwt stuff
    curve1->attach(plot1);
    curve1->setPen(QPen(Qt::red));
    curve2->attach(plot1);
    curve2->setPen(QPen(Qt::blue));
    plot1->insertLegend(legend);

    connect(launch_button, SIGNAL(clicked()), this, SLOT(calculate()));

    QVBoxLayout *main_layout(new QVBoxLayout(this));
    main_layout->addLayout(inputs_layout);
    main_layout->addLayout(labels_layout);
    main_layout->addWidget(launch_button);
    main_layout->addWidget(plot1);

    setMinimumSize(QSize(431, 407));
}

void Lab2_Widget::create_xs(QVector< double >& xs, const double& a,
                            const double& b, const std::size_t n) const
{
    xs.resize(n);

    const auto step = (b - a) / n;

    auto x(a);
    for (auto i(0); n != i; ++i, x += step)
        xs[i] = x;
}

void Lab2_Widget::diff(const std::function< double(double) >& func,
                        QVector< double >& ys) const
{
    using namespace std;

    const auto step = ys.at(1) - ys.at(0);

    transform(ys.begin(), ys.end(), ys.begin(),
             [&func, &step](double x) {
                           return (func(x + step) - func(x)) / step;
             });
}

double Lab2_Widget::integrate(const std::function< double(double) > &func,
                                          QVector< double > xs) const
{
    using namespace std;
    const auto step = xs.at(1) - xs.at(0);

    const auto my_f = [&step, &func](double x) {
        return (func(x + 0.5 * step));
    };

    transform(xs.begin(), xs.end(), xs.begin(), my_f);

    const auto result = accumulate(xs.begin(), xs.end(), 0.0) * step;

    return result;
}

void Lab2_Widget::calculate()
{
    // FUNCTIONS ARE DEFINED HERE
    // double -> double
    static const auto f = [](double x) { return 1.0 / (x * qSqrt(x * x + 0.25)); };
    // double -> double
    static const auto F = [](double x) { return -2.0 * log((0.5 + qSqrt(x * x + 0.25)) / x); };

    QVector< double > xs;
    const auto a = a_spinbox->value();
    const auto b = b_spinbox->value();
    const auto n = n_spinbox->value();

    create_xs(xs, a, b, n);

    const auto I = integrate(f, xs);
    const auto rI = F(b) - F(a);
    const auto dI = qAbs(rI - I);
    integral_num->setText(trUtf8("%1").arg(I, 0, 'f', 3));
    integral_real->setText(trUtf8("%1").arg(rI, 0, 'f', 3));
    integral_diff->setText(trUtf8("%1").arg(dI));

    auto ys(xs);
    diff(F, ys);

    auto zs(xs);
    std::transform(zs.begin(), zs.end(), zs.begin(), f);


    curve1->setSamples(xs, ys);
    curve2->setSamples(xs, zs);
    curve1->show();
    curve2->show();
    plot1->replot();

}

void Lab2_Widget::a_sn(double value)
{
    if (value < a_spinbox->value())
        a_spinbox->setValue(value);
}

void Lab2_Widget::b_sn(double value)
{
    if (value > b_spinbox->value())
        b_spinbox->setValue(value);
}

void Lab2_Widget::reinit()
{
    a_spinbox->setValue(1.0);
    b_spinbox->setValue(2.0);
    n_spinbox->setValue(50);
    integral_num->clear();
    integral_real->clear();
    integral_diff->clear();
    curve1->hide();
    curve2->hide();
    plot1->replot();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
