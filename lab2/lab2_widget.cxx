#include "lab2_widget.hxx"
#include <QVector>
#include <QSize>
#include <algorithm>
#include <iostream>
#include <numeric>

Lab2_Widget::Lab2_Widget(QWidget *parent)
    : QWidget(parent),
      main_layout(new QVBoxLayout(this)),
      inputs_layout(new QHBoxLayout(nullptr)),
      labels_layout(new QHBoxLayout(nullptr)),
      l1(new QLabel(trUtf8("a: "), this)),
      l2(new QLabel(trUtf8("b: "), this)),
      l3(new QLabel(trUtf8("n: "), this)),
      integral_num(new QLabel(trUtf8("Calculated integral: _____"), this)),
      integral_real(new QLabel(trUtf8("Real value of integral: _____"), this)),
      integral_diff(new QLabel(trUtf8("Error: _____"), this)),
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
    delete labels_layout;
    delete curve1;
    delete curve2;
    delete inputs_layout;
}

void Lab2_Widget::setup_ui()
{
    l1->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    l2->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
    l3->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));

    a_spinbox->setDecimals(2);
    a_spinbox->setRange(0.5, 5.0);
    a_spinbox->setValue(1.0);
    a_spinbox->setSingleStep(0.1);

    b_spinbox->setDecimals(2);
    b_spinbox->setRange(0.5, 5.0);
    b_spinbox->setValue(2.0);
    b_spinbox->setSingleStep(0.1);

    n_spinbox->setRange(5, 5000);
    n_spinbox->setValue(50);
    n_spinbox->setSingleStep(5);

    connect(a_spinbox, SIGNAL(valueChanged(double)), this, SLOT(b_sn(double)));
    connect(b_spinbox, SIGNAL(valueChanged(double)), this, SLOT(a_sn(double)));

    inputs_layout->addWidget(l1);
    inputs_layout->addWidget(a_spinbox);
    inputs_layout->addWidget(l2);
    inputs_layout->addWidget(b_spinbox);
    inputs_layout->addWidget(l3);
    inputs_layout->addWidget(n_spinbox);

    labels_layout->addWidget(integral_num);
    labels_layout->addWidget(integral_real);
    labels_layout->addWidget(integral_diff);

    curve1->attach(plot1);
    curve1->setPen(QPen(Qt::red));
    curve2->attach(plot1);
    curve2->setPen(QPen(Qt::blue));
    plot1->insertLegend(legend);

    connect(launch_button, SIGNAL(clicked()), this, SLOT(calculate()));

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
        return (func(x + 0.5 * step) * step);
    };

    transform(xs.begin(), xs.end(), xs.begin(), my_f);

    const auto result = accumulate(xs.begin(), xs.end(), 0.0);

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
    integral_num->setText(trUtf8("Calculated integral: %1").arg(I, 0, 'f', 3));
    integral_real->setText(trUtf8("Real value of integral: %1").arg(rI, 0, 'f', 3));
    integral_diff->setText(trUtf8("Error: %1").arg(dI));
    //std::cout << "real value: " << F(b) - F(a) << std::endl << "got: " << integrate(f, xs) << std::endl;

    auto ys(xs);
    diff(F, ys);

    auto zs(xs);
    std::transform(zs.begin(), zs.end(), zs.begin(), f);


    curve1->setSamples(xs, ys);
    curve2->setSamples(xs, zs);
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
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
