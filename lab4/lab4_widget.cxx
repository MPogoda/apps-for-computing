#include "lab4_widget.hxx"
#include <QSize>
#include <QString>
#include <QWidget>
#include <QtCore>

#include <array>
#include <utility>
#include <cassert>

const
std::pair< std::array< qreal, 3 >,
           std::array< qreal, 3 > >
sven_method( const std::function< qreal(qreal) >& f,
             const qreal x0,
             qreal delta )
{
    const qreal epsilon( 0.000001 );
    assert (delta > 0);

    const qreal x_minus( x0 - delta );
    const qreal x_plus( x0 + delta );
    const qreal f_minus( f(x_minus) );
    const qreal f_plus( f(x_plus) );

    qreal x_cur( x0 );
    qreal f_cur( f(x_cur) );

    qreal x_prev;
    qreal x_next;
    qreal f_prev;
    qreal f_next;

    if ((f_minus < f_cur + epsilon) && (f_cur + epsilon > f_plus))
    {
        //NON UNIMODAL
        assert (false);
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

    qreal x_mid = 0.5 * (x_cur + x_next);
    qreal f_mid = f(x_mid);

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

    const std::array< qreal, 3 > xs = { x_prev, x_cur, x_next };
    const std::array< qreal, 3 > fs = { f_prev, f_cur, f_next };

    return std::make_pair( xs, fs);
}

const
std::pair< qreal, qreal >
dsk_powell_method(const std::function< qreal(qreal) >& f,
                  qreal x0,
                  const qreal delta,
                  const std::pair< qreal, qreal >& epsilon)
{
    const std::pair<
            std::array< qreal, 3 >,
            std::array< qreal, 3> > sven_result( sven_method(f, x0, delta) );

    qreal x_1( sven_result.first[0] );
    qreal x_2( sven_result.first[1] );
    qreal x_3( sven_result.first[2] );
    qreal f_1( sven_result.second[0] );
    qreal f_2( sven_result.second[1] );
    qreal f_3( sven_result.second[2] );

    qreal x_ast( x_2 + delta * (f_1 - f_3) / (2.0 * (f_1 - 2.0 * f_2 + f_3)) );
    qreal f_ast( f(x_ast) );

    while ((qAbs(f_ast - f_2) > epsilon.second) || (qAbs(x_ast - x_2) > epsilon.first))
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

        const qreal a_1( (f_2 - f_1) / (x_2 - x_1) );
        const qreal a_2( ((f_3 - f_1)/(x_3 - x_1) - a_1)/(x_3 - x_2) );

        x_ast = 0.5 * (x_1 + x_2) - a_1 / (2 * a_2);
        f_ast = f(x_ast);
    }

    if (f_ast < f_2)
        return std::make_pair(x_ast, f_ast);
    else
        return std::make_pair(x_2, f_2);
}

template< std::size_t N >
const std::array< qreal, N >
wrap_s( std::array< qreal, N > x,
        const qreal lambda,
        const std::array< qreal, N >& s)
{
    for (auto i( 0 ); N != i; ++i)
        x[i] += lambda * s[i];

    return x;
}

template< std::size_t N >
const qreal
vecAbs( std::array< qreal, N > a,
        std::array< qreal, N > b)
{
    qreal result( 0.0 );
    for (auto i( 0 ); N != i; ++i)
        result += qPow((a[i] - b[i]), 2);

    return qSqrt(result);
}

template< std::size_t N >
const std::pair< std::array< qreal, N >,
           qreal >
powell_method(std::function< qreal(std::array< qreal, N >) > f,
              const std::array< qreal, N >& x0,
              const qreal delta,
              const std::pair< qreal, qreal>& epsilon)
{
    std::array< std::array< qreal, N >, N > S;

    for (auto i( 0 ); N != i; ++i)
        for (auto j( 0 ); N != j; ++j)
            if (i != j)
                S[i][j] = 0.0;
            else
                S[i][i] = 1.0;

    std::array< qreal, N > x( x0 );
    qreal _f = f( x );

    forever
    {
        std::size_t k( N - 1);
        std::array< qreal, N > x_before;
        qreal f_before;
        do
        {
            const std::array< qreal, N >& s = S[k];
            const std::function< qreal(qreal) > g = [&f, &x, &s](qreal lambda)
            {
                return f( wrap_s(x, lambda, s ));
            };

            std::pair< qreal, qreal > dskp_result( dsk_powell_method(g,
                                                                     0.0,
                                                                     delta,
                                                                     epsilon) );

            x = wrap_s( x, dskp_result.first, s);
            _f = dskp_result.second;

            k = ++k % N;

            if (0 == k)
            {
                x_before = x; f_before = _f;
            }

        } while (N - 1 != k);

        for (auto i( 0 ); i < N - 1; ++i)
            std::swap( S[i], S[i + 1]);
        for (auto i( 0 ); i < N; ++i)
            S.back()[i] = x[i] - x_before[i];

        if ((qAbs(f_before - _f) < epsilon.second) || (vecAbs(x, x_before) < epsilon.first))
            return std::make_pair( x, _f);
    }
}


Lab4_Widget::Lab4_Widget(QWidget *parent)
    : Describable(QString::null,
        parent)
{

    setMinimumSize(QSize(431, 407));

    std::function< qreal( std::array<qreal, 2>)> f = [](std::array< qreal, 2 > x)
    {
        return (x.front() - 1.0) * (x.front() - 1.0) + (x.back() - 2.0) * (x.back() - 2.0);
    };


    std::array< qreal, 2 > x0 = { 0.0, 0.0 };

    auto result =
            powell_method(f, x0, 0.5, std::make_pair(0.01, 0.01));
}

Lab4_Widget::~Lab4_Widget()
{
}

void Lab4_Widget::reinit()
{
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
