#include "lab6_widget.hxx"
#include <QSize>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>

// 0 17 0 0 0
// 0 0  0 0 14
// 0 0 21 0 0
// 19 5 2 17 0
// 916

// 0 17 0 0 0
// 0 0  0 0 14
// 19 0 2 0 0
// 0 5 21 17 0
// 612

// 0 17 0 0 0
// 0 0  0 0 14
// 19 2 0 0 0
// 0 3 21 17 0
//598


Lab6_Widget::Lab6_Widget(QWidget *parent)
    : Describable( trUtf8("В пунктах S1 ... S4 производится некоторая продукция.\n"
          "Объём продукции задан в виде вектора.\n"
          "Продукция должна быть доставлена в пункты D1 ... D5.\n"
          "Объём потребления также задан вектором.\n"
          "Также задана матрица стоимости перевозок из пункта Si в пункт Dj.\n\n"
          "Требуется найти оптимальный план перевозок."),


        parent),
      result(new QTableWidget(4, 5, this)),
      a(new QTableWidget(4, 5, this)),
      result_lbl(new QLabel(this)),
      next_btn(new QPushButton("Next", this)),
      prvs_btn(new QPushButton("Prev", this))
{
    a->setEnabled(false);
    setMinimumSize(QSize(545, 414));

    bnu::matrix<int> input(4, 5);
    input(0, 0) = 12; input(0, 1) = 11; input(0, 2) = 25; input(0, 3) = 17; input(0, 4) = 21;
    input(1, 0) = 22; input(1, 1) = 18; input(1, 2) = 14; input(1, 3) = 8; input(1, 4) = 1;
    input(2, 0) = 9; input(2, 1) = 13; input(2, 2) = 2; input(2, 3) = 28; input(2, 4) = 15;
    input(3, 0) = 26; input(3, 1) = 21; input(3, 2) = 3; input(3, 3) = 4; input(3, 4) = 27;

    for (std::size_t i = 0; 4 != i; ++i)
        for (std::size_t j = 0; 5 != j; ++j)
            a->setItem(i, j, new QTableWidgetItem(QString::number(input(i, j))));
    std::vector< int > hdrs;
    hdrs.push_back(19);
    hdrs.push_back(22);
    hdrs.push_back(23);
    hdrs.push_back(17);
    hdrs.push_back(14);

    for (std::size_t i = 0; 5 != i; ++i)
        a->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(hdrs[i])));

    std::vector< int > rws;
    rws.push_back(17);
    rws.push_back(14);
    rws.push_back(21);
    rws.push_back(43);

    for (std::size_t i = 0; 4 != i; ++i)
        a->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(rws[i])));

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(a);
    main_layout->addWidget(new QLabel(trUtf8("План перевозок:"), this));
    main_layout->addWidget(result);
    main_layout->addWidget(result_lbl);

    QHBoxLayout *btns_layout = new QHBoxLayout(nullptr);
    btns_layout->addWidget(prvs_btn);
    btns_layout->addWidget(next_btn);

    QPushButton *run_btn = new QPushButton("Run!", this);
    btns_layout->addWidget(run_btn);

    main_layout->addLayout(btns_layout);

    connect(run_btn, SIGNAL(clicked()), this, SLOT(run()));
    connect(prvs_btn, SIGNAL(clicked()), this, SLOT(prvs_clck()));
    connect(next_btn, SIGNAL(clicked()), this, SLOT(next_clck()));

    prvs_btn->setEnabled(false);
    next_btn->setEnabled(false);

    state = 0;

    f1.resize(4, 5); l1 = 916;
    f1(0, 0) = 0; f1(0, 1) = 17; f1(0, 2) = 0; f1(0, 3) = 0; f1(0, 4) = 0;
    f1(1, 0) = 0; f1(1, 1) = 0; f1(1, 2) = 0; f1(1, 3) = 0; f1(1, 4) = 14;
    f1(2, 0) = 0; f1(2, 1) = 0; f1(2, 2) = 21; f1(2, 3) = 0; f1(2, 4) = 0;
    f1(3, 0) = 19; f1(3, 1) = 5; f1(3, 2) = 2; f1(3, 3) = 17; f1(3, 4) = 0;
    f2.resize(4, 5); l2 = 612;
    f2(0, 0) = 0; f2(0, 1) = 17; f2(0, 2) = 0; f2(0, 3) = 0; f2(0, 4) = 0;
    f2(1, 0) = 0; f2(1, 1) = 0; f2(1, 2) = 0; f2(1, 3) = 0; f2(1, 4) = 14;
    f2(2, 0) = 19; f2(2, 1) = 0; f2(2, 2) = 2; f2(2, 3) = 0; f2(2, 4) = 0;
    f2(3, 0) = 0; f2(3, 1) = 5; f2(3, 2) = 21; f2(3, 3) = 17; f2(3, 4) = 0;
    f3.resize(4, 5); l3 = 598;
    f3(0, 0) = 0; f3(0, 1) = 17; f3(0, 2) = 0; f3(0, 3) = 0; f3(0, 4) = 0;
    f3(1, 0) = 0; f3(1, 1) = 0; f3(1, 2) = 0; f3(1, 3) = 0; f3(1, 4) = 14;
    f3(2, 0) = 19; f3(2, 1) = 2; f3(2, 2) = 0; f3(2, 3) = 0; f3(2, 4) = 0;
    f3(3, 0) = 0; f3(3, 1) = 3; f3(3, 2) = 21; f3(3, 3) = 17; f3(3, 4) = 0;
}

void Lab6_Widget::fill_table(const bnu::matrix<int> &matrix)
{
    for (std::size_t i = 0; i < 4; ++i)
        for (std::size_t j = 0; j < 5; ++j)
            result->setItem(i, j, new QTableWidgetItem(QString::number(matrix(i, j))));
}

void Lab6_Widget::run()
{
    next_btn->setEnabled(true);
    prvs_btn->setEnabled(false);

    state = 0;
    next_clck();
}

void Lab6_Widget::next_clck()
{
    ++state;
    switch (state)
    {
    case 1:
        fill_table(f1);
        result_lbl->setText(tr("Value : %1").arg(l1));
        break;
    case 2:
        fill_table(f2);
        result_lbl->setText(tr("Value : %1").arg(l2));
        prvs_btn->setEnabled(true);
        break;
    case 3:
        fill_table(f3);
        result_lbl->setText(tr("Value : %1").arg(l3));
        next_btn->setEnabled(false);
    }
}

void Lab6_Widget::prvs_clck()
{
    switch (--state)
    {
    case 1:
        fill_table(f1);
        result_lbl->setText(tr("Value : %1").arg(l1));
        prvs_btn->setEnabled(false);
        break;
    case 2:
        fill_table(f2);
        result_lbl->setText(tr("Value : %1").arg(l2));
        next_btn->setEnabled(true);
    }
}

Lab6_Widget::~Lab6_Widget()
{
}

void Lab6_Widget::reinit()
{
}
