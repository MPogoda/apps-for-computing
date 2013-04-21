#include "lab5_widget.hxx"
#include <QSize>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>

#include <stdexcept>
#include <cmath>

#include <iostream>
Lab5_Widget::Lab5_Widget(QWidget *parent)
    : Describable( trUtf8("Предприятие выпускает 2 вида изделий: P1 и P2, каждое из которых\n"
        "проходит последовательную обработку на станках типов T1 , T2 и T3.\n"
        "Запас мощностей станков, т. е. рабочее время станка, составляет\n"
        "соответственно B1, B2 и B3 единиц времени.\n"
        "При реализации одно изделие Pi приносит предприятию Ci единиц прибыли.\n"
        "Требуется составить такой план загрузки станков, при котором предприятие\n"
        "получит максимальную прибыль."),
        parent),
      a_table(new QTableWidget(3, 2, this)),
      b_vector(new QTableWidget(3, 1, this)),
      c_vector(new QTableWidget(1, 2, this)),
      result_label(new QLabel(this)),
      a( bnu::matrix< double >(0, 0, 0.0))
{
    a_table->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    b_vector->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    c_vector->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    a_table->setItem(0, 0, new QTableWidgetItem(QString::number(2.0)));
    a_table->setItem(0, 1, new QTableWidgetItem(QString::number(3.0)));
    a_table->setItem(1, 0, new QTableWidgetItem(QString::number(3.0)));
    a_table->setItem(1, 1, new QTableWidgetItem(QString::number(1.0)));
    a_table->setItem(2, 0, new QTableWidgetItem(QString::number(0.0)));
    a_table->setItem(2, 1, new QTableWidgetItem(QString::number(1.0)));

    b_vector->setItem(0, 0, new QTableWidgetItem(QString::number(20.0)));
    b_vector->setItem(1, 0, new QTableWidgetItem(QString::number(37.0)));
    b_vector->setItem(2, 0, new QTableWidgetItem(QString::number(30.0)));

    c_vector->setItem(0, 0, new QTableWidgetItem(QString::number(11.0)));
    c_vector->setItem(0, 1, new QTableWidgetItem(QString::number(9.0)));

    QVBoxLayout *a_layout = new QVBoxLayout(nullptr);
    QLabel *a_label = new QLabel(trUtf8("Resource consumption:"), this);
    a_label->setBuddy(a_table);
    a_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    a_layout->addWidget(a_label);
    a_layout->addWidget(a_table);
    a_layout->addStretch();

    QVBoxLayout *b_layout = new QVBoxLayout(nullptr);
    QLabel *b_label = new QLabel(trUtf8("Resource limit:"), this);
    b_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    b_label->setBuddy(b_vector);
    b_layout->addWidget(b_label);
    b_layout->addWidget(b_vector);
    b_layout->addStretch();

    QVBoxLayout *c_layout = new QVBoxLayout(nullptr);
    QLabel *c_label = new QLabel(trUtf8("Resource income:"), this);
    c_label->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    c_label->setBuddy(c_vector);
    c_layout->addWidget(c_label);
    c_layout->addWidget(c_vector);
    c_layout->addStretch();

    QHBoxLayout *top_layout = new QHBoxLayout(nullptr);
    top_layout->addLayout(a_layout);
    top_layout->addLayout(b_layout);
    top_layout->addLayout(c_layout);

    QPushButton *run_btn = new QPushButton(trUtf8("Maximize!"), this);
    result_label->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    QHBoxLayout *bottom_layout = new QHBoxLayout(nullptr);
    bottom_layout->addWidget(result_label);
    bottom_layout->addWidget(run_btn);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(top_layout);
    main_layout->addLayout(bottom_layout);


    connect(run_btn, SIGNAL(clicked()), this, SLOT(run()));

    setMinimumSize(QSize(695, 184));
}

void Lab5_Widget::run()
{
    (std::vector< double >(b_vector->rowCount())).swap(b);
    for (std::size_t i = 0; b.size() != i; ++i)
        b[i] = b_vector->itemAt(0, i)->text().toDouble(nullptr);
    (std::vector< double >(c_vector->columnCount())).swap(c);
    for (std::size_t i = 0; c.size() != i; ++i)
        c[i] = c_vector->itemAt(i, 0)->text().toDouble(nullptr);
    M = b.size();
    N = c.size();

    a.resize(M + 1, N + M + 1, false);
    for (std::size_t i( 0 ); M != i; ++i)
        for (std::size_t j( 0 ); N != j; ++j)
            a(i, j) = a_table->itemAt(i, j)->text().toDouble(nullptr);
    for (std::size_t i( 0 ); M != i; ++i)
        a(i, N + i) = 1.0;
    for (std::size_t j( 0 ); N != j; ++j)
        a(M, j) = c[j];
    for (std::size_t i( 0 ); M != i; ++i)
        a(i, M + N) = b[i];

    (std::vector< std::size_t >(M)).swap(basis);
    for (std::size_t i( 0 ); M != i; ++i)
        basis[i] = N + i;

    solve();
    try {
        check(); } catch (std::logic_error *e)
    {
        std::cerr << e->what() << std::endl;
    }

    const auto& prim = primal();
    result_label->setText(QString("Max f(x) = %1 when x1 = %2 and x2 = %3")
                          .arg(value()).arg(prim[0]).arg(prim[1]));
}

Lab5_Widget::~Lab5_Widget()
{
}

void Lab5_Widget::reinit()
{
}

void Lab5_Widget::solve()
{
    forever
    {
        const std::size_t q = bland();
        if ((M + N) == q)
            break;

        const std::size_t p = min_ratio_rule(q);
        if (M == p)
            throw new std::logic_error("Linear program is unbounded.");

        pivot(p, q);

        basis[p] = q;
    }
}

const std::size_t Lab5_Widget::bland() const
{
    for (std::size_t j = 0 ; (M + N) != j; ++j)
        if (a(M, j) > 0)
            return j;
    return M + N;
}

const std::size_t Lab5_Widget::min_ratio_rule(const std::size_t q) const
{
    std::size_t p = M;
    for (std::size_t i = 0; M != i; ++i)
    {
        const auto aiq = a(i, q);
        if (aiq < EPSILON) // aiq <= 0
            continue;
        else if (M == p)
            p = i;
        else if ((a(i, M + N) / aiq) < (a(p, M + N) / a(p, q)))
            p = i;
    }

    return p;
}

void Lab5_Widget::pivot(const std::size_t p, const std::size_t q)
{
    const auto apq = a(p, q);

    for (std::size_t i = 0; M >= i; ++i)
        for (std::size_t j = 0; (M + N) >= j; ++j)
            if ((p != i) && (q != j))
                a(i, j) -= a(p, j) * a(i, q) / apq;

    for (std::size_t i = 0; M >= i; ++i)
        if (p != i)
            a(i, q) = 0.0;

    for (std::size_t j = 0; (M + N) >= j; ++j)
        a(p, j) /= apq;
}

void Lab5_Widget::check() const
{
    is_primal_feasible();
    is_dual_feasible();
    is_optimal();
}

void Lab5_Widget::is_primal_feasible() const
{
    const auto& x = primal();

//    for (std::size_t j = 0; N != j; ++j)
//        if (x[j] < 0.0)
    for (const auto& _x : x)
        if (_x < 0.0)
            throw new std::logic_error("x[j] is negative!");

    for (std::size_t i = 0; M != i; ++i)
    {
        double sum = 0.0;
        for (std::size_t j = 0; N != j; ++j)
            sum += a(i, j) * x[j];

        if ((sum - b[i]) > EPSILON)
            throw new std::logic_error("Not primal feasible!");
    }
}

const std::vector< double > Lab5_Widget::primal() const
{
    std::vector< double > result(N);
    for (std::size_t i = 0; M != i; ++i)
        if (N > basis[i])
            result[ basis[i] ] = a(i, M + N);

    return result;
}

void Lab5_Widget::is_dual_feasible() const
{
    const auto& y = dual();

//    for (std::size_t i = 0; M != i; ++i)
//        if (y[i] < 0.0)
    for (const auto& _y : y)
        if (_y < 0.0)
            throw new std::logic_error("y[i] is negative!");

    for (std::size_t j = 0; N != j; ++j)
    {
        double sum = 0.0;
        for (std::size_t i = 0; M != i; ++i)
            sum += a(i, j) * y[i];

        std::cout << sum << std::endl;
        std::cout << c[j] << std::endl;
        if ((c[j] - sum) > EPSILON)
            throw new std::logic_error("Not dual feasible!");
    }
}

const double Lab5_Widget::value() const
{
    return -a(M, N + M);
}

void Lab5_Widget::is_optimal() const
{
    const auto& x = primal();
    const auto& y = dual();
    const auto _value = value();

    double valuej = 0.0;
    for (std::size_t j = 0; j != N; ++j)
        valuej += c[j] * x[j];
    double valuei = 0.0;
    for (std::size_t i = 0; i != M; ++i)
        valuei += y[i] * b[i];

    if ((std::abs(_value - valuei) > EPSILON) || (std::abs(_value - valuej) > EPSILON))
        throw new std::logic_error("Not optimal!");

}

const std::vector< double > Lab5_Widget::dual() const
{
    std::vector< double > result(M);
    for (std::size_t i = 0; M != i; ++i)
        result[i] = -a(M, N + i);

    return result;
}
