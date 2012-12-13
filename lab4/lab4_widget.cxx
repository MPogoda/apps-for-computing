#include "lab4_widget.hxx"
#include <QSize>
#include <QString>
#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QLabel>
#include <iostream>
#include <vector>
#include <utility>
#include <array>
#include <cmath>
#include <vector>
#include <stdexcept>

Lab4_Widget::Lab4_Widget(QWidget *parent)
    : Describable(trUtf8("1. Определить систему направлений равную координатным осям.\n"
          "2. Найти оптимальную точку x1 вдоль направления S_N\n"
          "   с помощью одномерного поиска\n"
          "3. Итеративно произвести одномерные поиски вдоль направлений\n"
          "   S_1, S_2, ..., S_N. Полученную точку назвать x2\n"
          "4. Заменить S_1 <- S_2, S_2 <- S_3, ..., S_{N-1} <- S_N\n"
          "5. Вычислить S_N = (x2 - x1); S_N = S_N / |S_N|\n"
          "6. Если критерий остановки не выполняется, принять x0=x2 и перейти к шагу 2\n"
          ),
        parent),

      list(new QListWidget(this)),
      x1( new QDoubleSpinBox(this) ),
      x2( new QDoubleSpinBox(this) ),
      f( new QLabel(nullptr)),
      df( new QLabel(nullptr)),
      dx( new QLabel(nullptr)),
      delta( new QDoubleSpinBox(this))
{
    QLabel *x1_lbl( new QLabel( trUtf8("x1: "), this));
    x1_lbl->setBuddy(x1);
    x1_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    x1->setMinimum(-5.0);
    x1->setMaximum(5.0);
    x1->setSingleStep(0.1);
    x1->setValue(-1.0);

    QLabel *x2_lbl( new QLabel( trUtf8("x2: "), this));
    x2_lbl->setBuddy(x2);
    x2_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    x2->setMinimum(-5.0);
    x2->setMaximum(5.0);
    x2->setSingleStep(0.1);
    x2->setValue(3.0);

    QLabel *delta_lbl( new QLabel( trUtf8("Delta: "), this));
    delta_lbl->setBuddy(delta);
    delta_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    delta->setMinimum(0.0);
    delta->setMaximum(1.0);
    delta->setDecimals(4);
    delta->setSingleStep(0.01);
    delta->setValue(0.05);

    QPushButton *button( new QPushButton(trUtf8("Run!"), this));

    connect(button, SIGNAL(clicked()), this, SLOT(run()));

    QHBoxLayout *top_layout( new QHBoxLayout(nullptr));

    top_layout->addWidget(x1_lbl);
    top_layout->addWidget(x1);
    top_layout->addWidget(x2_lbl);
    top_layout->addWidget(x2);
    top_layout->addWidget(delta_lbl);
    top_layout->addWidget(delta);
    top_layout->addWidget(button);

    list->setSelectionMode(QAbstractItemView::NoSelection);

    QVBoxLayout *main_layout( new QVBoxLayout(this));
    main_layout->addLayout(top_layout);
    main_layout->addWidget(list);

    QHBoxLayout *bottom_layout( new QHBoxLayout(nullptr));

    QLabel *f_lbl( new QLabel(trUtf8("f( x_opt ) = "), this));
    f_lbl->setBuddy(f);
    f_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    f->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    QLabel *dx_lbl( new QLabel(trUtf8("error of x is "), this));
    dx_lbl->setBuddy(dx);
    dx_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    dx->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    QLabel *df_lbl( new QLabel( trUtf8("error of f(x) is "), this));
    df_lbl->setBuddy(df);
    df_lbl->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    df->setFrameStyle(QFrame::Sunken | QFrame::Panel);

    bottom_layout->addWidget(f_lbl);
    bottom_layout->addWidget(f);
    bottom_layout->addWidget(dx_lbl);
    bottom_layout->addWidget(dx);
    bottom_layout->addWidget(df_lbl);
    bottom_layout->addWidget(df);

    main_layout->addLayout(bottom_layout);

    setMinimumSize(QSize(431, 407));
}

void Lab4_Widget::run()
{
    list->clear();

    const static std::function< qreal( std::array< qreal, 2>) > f =
            [](std::array< qreal, 2 > x)
    {
        return  std::pow(x[0], 3) +
                std::pow(x[1], 3) -
                3.0 * x[0] * x[1];
    };

    const std::array< qreal, 2 > x0 = {x1->value(), x2->value() };
    const std::array< qreal, 2 > real_x = { 1.0, 1.0 };
    const qreal real_f = f(real_x);

    try {
        const auto result = run(f, x0, delta->value());

        for (const auto& x : result.second) {
            add_row(x);
        }

        this->f->setText(QString::number( result.first ));
        df->setText( QString::number( real_f - result.first ));
        dx->setText( QString::number( vecAbs(result.second.back(), real_x )));


    } catch (const std::logic_error* e)
    {
        QMessageBox::warning(this, QString::null, trUtf8("Bad optimization.\n"
              "We came out of local minimum area"));
        return;
    }

}

Lab4_Widget::~Lab4_Widget()
{
}

void Lab4_Widget::reinit()
{
  x1->setValue(-1.0);
  x2->setValue(3.0);
  delta->setValue(0.5);
  list->clear();
}

template< std::size_t N >
const
double
Lab4_Widget::vecAbs( std::array< double, N > a,
        std::array< double, N > b)
{
    double result( 0.0 );
    for (std::size_t i( 0 ); N != i; ++i)
        result += std::pow((a[i] - b[i]), 2);

    return std::sqrt(result);
}

template< std::size_t N >
const
std::pair< double,
           std::vector< std::array< double, N > > >
Lab4_Widget::run(const std::function< double(std::array< double, N >) >& f,
                   const std::array< double, N >& x0,
                   const double delta,
                   const std::pair< double, double>& epsilon)
{
    std::array< std::array< double, N >, N > S;
    std::vector< std::array< double, N > >    xs;

    for (std::size_t i( 0 ); N != i; ++i)
        for (std::size_t j( 0 ); N != j; ++j)
            if (i != j)
                S[i][j] = 0.0;
            else
                S[i][i] = 1.0;

    auto x( x0 );
    auto _f = f( x );

    xs.push_back(x);
    // forever :>
    for (;;)
    {
        std::size_t k( N - 1);
        std::array< double, N > x_before;
        double f_before;
        do
        {
            auto& s = S[k];
            const auto& g = [&](double lambda)
            {
                return f( wrap_s(x, lambda, s ));
            };

            const auto dskp_result( run(g, 0.0, delta,  epsilon) );

            x = wrap_s( x, dskp_result.first, s);
            xs.push_back(x);
            _f = dskp_result.second;

            k = ++k % N;

            if (0 == k)
            {
                x_before = x; f_before = _f;
            }

        } while (N - 1 != k);

        for (std::size_t i( 0 ); i < N - 1; ++i)
            std::swap( S[i], S[i + 1]);
        for (std::size_t i( 0 ); i < N; ++i)
            S.back()[i] = x[i] - x_before[i];

        if ((std::abs(f_before - _f) < epsilon.second) || (vecAbs(x, x_before) < epsilon.first))
            return std::make_pair( _f, xs);
    }
}

const
std::pair< std::array< double, 3 >,
           std::array< double, 3 > >
Lab4_Widget::sven_method( const std::function< double(double) >& f,
             const double x0,
             double delta )
{
    const double epsilon( 0.000001 );
    if (delta < epsilon)
        throw new std::logic_error("delta must be greater than zero");

    const auto x_minus( x0 - delta );
    const auto x_plus( x0 + delta );
    const auto f_minus( f(x_minus) );
    const auto f_plus( f(x_plus) );

    auto x_cur( x0 );
    auto f_cur( f(x_cur) );

    double x_prev;
    double x_next;
    double f_prev;
    double f_next;

    if ((f_minus < f_cur + epsilon) && (f_cur + epsilon > f_plus))
    {
        //NON UNIMODAL
        throw new std::logic_error("function f must be unimodal");
    } else if (f_minus + epsilon > f_cur)
    {
        x_prev = x_minus; f_prev = f_minus;
        x_next = x_plus;  f_next = f_plus;
    } else // f_minus < f_cur
    {
        // change direction
        delta *= -1.0;
        x_prev = x_plus;  f_prev = f_plus;
        x_next = x_minus; f_next = f_minus;
    }

    while (f_cur + epsilon > f_next)
    {
        x_prev = x_cur;  f_prev = f_cur;
        x_cur  = x_next; f_cur  = f_next;

        delta *= 2.0;
        x_next += delta;
        f_next = f(x_next);
    }

    auto x_mid = 0.5 * (x_cur + x_next);
    auto f_mid = f(x_mid);

    if (f_mid < f_cur + epsilon)
    {
        x_prev = x_cur; f_prev = f_cur;
        x_cur  = x_mid; f_cur  = f_mid;
    } else
    {
        x_next = x_mid; f_next = f_mid;
    }

    if (delta < 0)
    {
        std::swap(x_prev, x_next);
        std::swap(f_prev, f_next);
    }

    const std::array< double, 3 > xs = { x_prev, x_cur, x_next };
    const std::array< double, 3 > fs = { f_prev, f_cur, f_next };

    return std::make_pair( xs, fs);
}

const
std::pair< double, double >
Lab4_Widget::run(const std::function< double(double) >& f,
                  double x0,
                  const double delta,
                  const std::pair< double, double >& epsilon)
{
    const auto sven_result( sven_method(f, x0, delta) );

    auto x_1( sven_result.first[0] );
    auto x_2( sven_result.first[1] );
    auto x_3( sven_result.first[2] );
    auto f_1( sven_result.second[0] );
    auto f_2( sven_result.second[1] );
    auto f_3( sven_result.second[2] );

    auto x_ast( x_2 + delta * (f_1 - f_3) / (2.0 * (f_1 - 2.0 * f_2 + f_3)) );
    auto f_ast( f(x_ast) );

    while ((std::abs(f_ast - f_2) > epsilon.second) ||
           (std::abs(x_ast - x_2) > epsilon.first))
    {
        if (f_ast < f_2)
        {
            if (x_ast < x_2)
            {
                x_3 = x_2; f_3 = f_2;
            } else
            {
                x_1 = x_2; f_1 = f_2;
            }

            x_2 = x_ast; f_2 = f_ast;
        } else
        {
            if (x_ast < x_2)
            {
                x_1 = x_ast; f_1 = f_ast;
            } else
            {
                x_3 = x_ast; f_3 = f_ast;
            }
        }

        const auto a_1( (f_2 - f_1) / (x_2 - x_1) );
        const auto a_2( ((f_3 - f_1)/(x_3 - x_1) - a_1)/(x_3 - x_2) );

        x_ast = 0.5 * (x_1 + x_2) - a_1 / (2 * a_2);
        f_ast = f(x_ast);
    }

    if (f_ast < f_2)
        return std::make_pair(x_ast, f_ast);
    else
        return std::make_pair(x_2, f_2);
}

template< std::size_t N >
const
std::array< double, N >
Lab4_Widget::wrap_s( std::array< double, N > x,
        const double lambda,
        const std::array< double, N >& s)
{
    for (std::size_t i( 0 ); N != i; ++i)
        x[i] += lambda * s[i];

    return x;
}

template< std::size_t N >
void
Lab4_Widget::add_row(const std::array< double, N > x)
{
    QString result = "(";
    for (std::size_t i(0); i + 1 < N; ++i)
        result += QString("%1, ").arg(x[i]);

    result += QString("%1)").arg(x[N - 1]);

    list->addItem(result);
}
