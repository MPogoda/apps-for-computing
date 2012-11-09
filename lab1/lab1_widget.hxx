#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QTemporaryFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace bnu = boost::numeric::ublas;

class Lab1_Widget : public QWidget
{
    Q_OBJECT

public:
    Lab1_Widget(QWidget *parent = nullptr);
    ~Lab1_Widget();

private:
    QVBoxLayout *main_layout;
    QHBoxLayout *buttons_layout;
    QPushButton *input_button;
    QPushButton *output_button;
    QLabel      *label;
    QPushButton *open_button;

    QTemporaryFile *script;
    bnu::matrix< qreal > A;

    bool prepare_script();

    bool read_matrix(const QString& filename);
    QString matrix_to_string() const;
    bool compile_latex_string(const QString& input, const QString& filename) const;
    int determinant_sign(const bnu::permutation_matrix< std::size_t >& pm) const;
    qreal determinant( const bnu::matrix<qreal>& A) const;
    void solve(const qreal epsilon = 0.00001);
private slots:
    void open_and_solve();
    void view_input();
    void view_output();
};
#endif // WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
