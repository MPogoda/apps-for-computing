#ifndef LAB4_WIDGET_H
#define LAB4_WIDGET_H
#include <functional>
#include <utility>
#include <array>
#include <vector>

class QWidget;
class QDoubleSpinBox;
class QLabel;
#include "../describable/describable.hxx"
class QListWidget;

class Lab4_Widget : public Describable
{
    Q_OBJECT

public:
    Lab4_Widget(QWidget *parent = nullptr);
    virtual ~Lab4_Widget();
    virtual void reinit();
private:
    QListWidget* list;
    QDoubleSpinBox* x1;
    QDoubleSpinBox *x2;
    QDoubleSpinBox *delta;
    QLabel *f;
    QLabel *df;
    QLabel *dx;

    template< std::size_t N >
    static
    const
    double
    vecAbs( std::array< double, N > a,
            std::array< double, N > b);

    template< std::size_t N >
    static
    const
    std::pair< double,
               std::vector< std::array< double, N > > >
    run(const std::function< double(std::array< double, N >) >& f,
                  const std::array< double, N >& x0,
                  const double delta = 0.05,
                  const std::pair< double, double>&
                      epsilon = std::make_pair(0.00001, 0.00001));


    static
    const
    std::pair< std::array< double, 3 >,
               std::array< double, 3 > >
    sven_method( const std::function< double(double) >& f,
                 const double x0 = 0.0,
                 double delta = 0.05);

    static
    const
    std::pair< double, double >
    run(const std::function< double(double) >& f,
        double x0 = 0.0,
        const double delta = 0.05,
        const std::pair< double, double >&
            epsilon = std::make_pair( 0.00001, 0.00001));

    template< std::size_t N >
    static
    const
    std::array< double, N >
    wrap_s( std::array< double, N > x,
            const double lambda,
            const std::array< double, N >& s);


    template< std::size_t N >
    void
    add_row(const std::array< double, N > x);

private slots:
    void run();

};

#endif // LAB4_WIDGET_H
