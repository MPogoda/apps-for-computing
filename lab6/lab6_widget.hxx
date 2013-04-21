#ifndef LAB6_WIDGET_H
#define LAB6_WIDGET_H

class QWidget;
class QTableWidget;
class QLabel;
class QPushButton;
#include "../describable/describable.hxx"
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>

namespace bnu = boost::numeric::ublas;

class Lab6_Widget : public Describable
{
    Q_OBJECT

public:
    Lab6_Widget(QWidget *parent = nullptr);
    virtual ~Lab6_Widget();
    virtual void reinit();
private:
    QTableWidget *a;
    QTableWidget *result;
    QLabel       *result_lbl;
    QPushButton  *next_btn;
    QPushButton  *prvs_btn;
    int state;
    bnu::matrix< int > f1, f2, f3;
    int l1, l2, l3;
    void fill_table(const bnu::matrix< int >& matrix);
private slots:
    void run();
    void next_clck();
    void prvs_clck();

};

#endif // LAB6_WIDGET_H
