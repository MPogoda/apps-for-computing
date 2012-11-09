#include "lab1_widget.hxx"
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QProcess>
#include <QMessageBox>

#include <cassert>

Lab1_Widget::Lab1_Widget(QWidget *parent) :
    QWidget(parent),
    main_layout(    new QVBoxLayout(this)),
    buttons_layout( new QHBoxLayout(nullptr)),
    input_button(   new QPushButton(trUtf8("View input"), this)),
    output_button(  new QPushButton(trUtf8("View output"), this)),
    label(          new QLabel(this)),
    open_button(    new QPushButton(trUtf8("Open file"), this))
{
    input_button->setEnabled(false);
    output_button->setEnabled(false);

    buttons_layout->addWidget(input_button);
    buttons_layout->addWidget(output_button);

    main_layout->addLayout(buttons_layout);
    main_layout->addWidget(label);
    main_layout->addWidget(open_button);

    connect(open_button,   SIGNAL(clicked()), this, SLOT(open_and_solve()));
    connect(input_button,  SIGNAL(clicked()), this, SLOT(view_input()));
    connect(output_button, SIGNAL(clicked()), this, SLOT(view_output()));

    if (!prepare_script())
        close();
}

Lab1_Widget::~Lab1_Widget()
{
    delete buttons_layout;
}

bool Lab1_Widget::prepare_script()
{
    static const QString SCRIPT("mask=tmp_$$\n"
                                "/bin/cp $1 $mask.tex\n"
                                "/usr/bin/latex $mask.tex\n"
                                "/usr/bin/dvipng -D 600 $mask.dvi -o $mask.png\n"
                                "/usr/bin/convert -scale 400x400 $mask.png $2.png\n"
                                "/bin/rm -f $mask.*\n");

    script = new QTemporaryFile(this);
    if (!script->open()) {
        qDebug() << trUtf8("Cannot open script file.");
        return false;
    }
    QTextStream outs(script);
    outs << SCRIPT;
    outs.flush();
    script->close();

    return true;
}

bool Lab1_Widget::read_matrix(const QString &filename)
{
    QFile file(filename);
    if (!file.exists()) {
        qDebug() << trUtf8("File doesn't exist");
        return false;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << trUtf8("Cannot open file for reading");
        return false;
    }
    std::size_t n;
    QTextStream in(&file);
    in >> n;
    if (in.atEnd()) {
        qDebug() << trUtf8("Empty file");
        return false;
    }

    auto result = new bnu::matrix< qreal >(n, n + 1, 0.0);

    for (auto i(0); n != i; ++i)
        for (auto j(0); n >= j; ++j) {
            QString _value;
            in >> _value;
            if (in.atEnd()) {
                qDebug() << trUtf8("Unexpected end of file");
                delete result;
                return false;
            }
            bool conversion_is_ok;
            auto value = _value.toDouble(&conversion_is_ok);
            if (!conversion_is_ok) {
                qDebug() << trUtf8("Incorrect value: ") + _value;
                delete result;
                return false;
            }
            result->insert_element(i, j, value);
        }

    A = *result;
    return true;
}

QString Lab1_Widget::matrix_to_string() const
{
    assert(A.size1() < A.size2());

    QString result("\\left(\\begin{array}{");
    for (auto i(0); A.size1() != i; ++i) result += "c";
    result += "|c}";
    for (auto i(0); A.size1() != i; ++i) {
        for (auto j(0); A.size2() != j; ++j) {
            result += QString("~%1~").arg(A(i, j), 0, 'f', 3);
            if (A.size2() - 1 != j)
                result += " & ";
        }
        result += "\\\\";
    }
    result += "\\end{array}\\right)";

    return result;
}

bool Lab1_Widget::compile_latex_string(const QString &input, const QString &filename) const
{
    QTemporaryFile file;
    if (!file.open()) {
        qDebug() << tr("Cannot open temporary file.");
        return false;
    }
    QTextStream out(&file);
    out << "\\documentclass[12pt]{scrartcl}\n\\usepackage[utf8]{inputenc}\n"
        "\\usepackage{concrete}\n\\begin{document}\n\\thispagestyle{empty}\n"
        "\\setbox0=\\hbox{$";
    out << input;
    out << "$}\n\\textheight=\\ht0\n\\advance\\textwidth by 2em\n"
        "\\advance\\textheight by 2\\dp0\n"
        "\\vbox{\\vss\\hbox{\\hss\\copy0\\hss}\\vss}\n\\end{document}";
    out.flush();
    file.close();

    QProcess process;
    QStringList arguments;
    arguments << script->fileName() << file.fileName() << filename;
    process.start("/bin/sh", arguments);

    return process.waitForFinished();
}

int Lab1_Widget::determinant_sign(const bnu::permutation_matrix< std::size_t >& pm) const
{
  auto pm_sign( 1 );
  const auto size( pm.size() );
  for (auto i( 0 ); size != i; ++i)
    if (i != pm(i))
      pm_sign *= -1;

  return pm_sign;
}

qreal Lab1_Widget::determinant( const bnu::matrix< qreal >& A) const
{
  bnu::matrix< qreal > m(A.size1(), A.size1(), 0.0);
  for (auto i( 0 ); A.size1() != i; ++i)
    for (auto j( 0 ); A.size1() != j; ++j)
      m(i, j) = A(i, j);

  bnu::permutation_matrix< std::size_t > pm(m.size1());
  auto det(1.0);
  if ( bnu::lu_factorize(m, pm))
  {
    det = 0.0;
  }
  else
  {
    for (auto i( 0 ); m.size1() != i; ++i)
      det *= m(i, i);
    det = det * determinant_sign( pm );
  }

  return det;
}

void Lab1_Widget::solve(const qreal epsilon)
{
    for (auto p(0); A.size1() != p; ++p) {
        auto divider(A(p, p));
        for (auto i(0); A.size1() != i; ++i) {
            if (i != p) {
                auto scale(A(i, p));
                for (auto j(0); A.size2() != j; ++j) {
                    A(i, j) = (A(p, j) * scale / divider) - A(i, j);
                }
            }
        }
    }

    for (auto p(0); A.size1() != p; ++p) {
        auto scale(A(p, p));
        for (auto j(0); A.size2() != j; ++j) {
            if ((epsilon < qAbs(A(p, j))) && (epsilon < qAbs(scale))) {
                A(p, j) /= scale;
            } else {
                A(p, j) = 0.0;
            }
        }
    }
}

void Lab1_Widget::open_and_solve()
{
    input_button->setEnabled(false);
    output_button->setEnabled(false);
    label->clear();

    auto filename = QFileDialog::getOpenFileName(this,
                    trUtf8("Select input file"),
                    "/home/");
    auto is_ok = read_matrix(filename);
    if (!is_ok) {
        qDebug() << trUtf8("There was an error while reading matrix.");
        QMessageBox::critical(this, trUtf8("Error"),
                              trUtf8("There was an error with reading from file"));
    } else {
        if (!compile_latex_string(matrix_to_string(), "/tmp/input")) {
            qDebug() << trUtf8("Cannot compile input matrix into PNG.");
            QMessageBox::critical(this, trUtf8("Error"),
                                  trUtf8("Cannot compile input matrix into PNG."));
            return;
        }

        if (0.0001 > qAbs(determinant(A)))
        {
          QMessageBox::critical(this, trUtf8("Error"),
              trUtf8("det(A) = 0!"));
          return;
        }

        solve();

        if (!compile_latex_string(matrix_to_string(), "/tmp/output")) {
            qDebug() << trUtf8("Cannot compile output matrix into PNG.");
            QMessageBox::critical(this, trUtf8("Error"),
                                  trUtf8("Cannot compile output matrix into PNG."));
            return;
        }

        view_input();
    }
}

void Lab1_Widget::view_input()
{
    label->setPixmap(QPixmap("/tmp/input.png"));
    input_button->setEnabled(false);
    output_button->setEnabled(true);
}

void Lab1_Widget::view_output()
{
    label->setPixmap(QPixmap("/tmp/output.png"));
    input_button->setEnabled(true);
    output_button->setEnabled(false);
}
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
