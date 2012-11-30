#ifndef WIDGET_H
#define WIDGET_H

class QWidget;
class QString;
class QTemporaryFile;
class QPushButton;
class QLabel;

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include "../describable/describable.hxx"

namespace bnu = boost::numeric::ublas;

class Lab1_Widget : public Describable
{
    Q_OBJECT

public:
    Lab1_Widget(QWidget *parent = nullptr);
private:
    QPushButton *input_button;
    QPushButton *output_button;
    QLabel      *label;
    QPushButton *open_button;

    QTemporaryFile *script;
    bnu::matrix< double > A;

    bool prepare_script();

    bool read_matrix(const QString& filename);
    QString matrix_to_string() const;
    bool compile_latex_string(const QString& input, const QString& filename) const;
    int determinant_sign(const bnu::permutation_matrix< std::size_t >& pm) const;
    double determinant( const bnu::matrix< double >& A) const;
    void solve(const double epsilon = 0.00001);

    virtual void reinit();
private slots:
    void open_and_solve();
    void view_input();
    void view_output();
};
#endif // WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
