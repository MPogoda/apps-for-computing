#ifndef LAB5_WIDGET_H
#define LAB5_WIDGET_H

class QWidget;
class QTableWidget;
class QLabel;
#include "../describable/describable.hxx"
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;

class Lab5_Widget : public Describable
{
    Q_OBJECT

public:
    Lab5_Widget(QWidget *parent = nullptr);
    virtual ~Lab5_Widget();
    virtual void reinit();
private:
    QTableWidget *a_table;
    QTableWidget *b_vector;
    QTableWidget *c_vector;
    QLabel       *result_label;

    const static double constexpr EPSILON = 1e-10;
    bnu::matrix< double > a;
    std::vector< double > b;
    std::vector< double > c;
    std::size_t N;
    std::size_t M;
    std::vector< std::size_t > basis;

    void solve();
    const std::size_t bland() const;
    const std::size_t min_ratio_rule(const std::size_t q) const;
    void pivot(const std::size_t p, const std::size_t q);
    void check() const;
    void is_primal_feasible() const;
    void is_dual_feasible() const;
    void is_optimal() const;
    const std::vector< double > primal() const;
    const std::vector< double > dual()   const;
    const double value() const;
private slots:
    void run();

};

#endif // LAB5_WIDGET_H
