#include "main_widget.hxx"
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "lab1/lab1_widget.hxx"
#include "lab2/lab2_widget.hxx"
#include "lab3/lab3_widget.hxx"

Main_Widget::Main_Widget(QWidget *parent) :
  QWidget(parent),
  buttons(new QButtonGroup(this)),
  description(new QLabel(this)),

  current(-1)
{
  labs.push_back(new Lab1_Widget(this));
  labs.push_back(new Lab2_Widget(this));
  labs.push_back(new Lab3_Widget(this));

  buttons->setExclusive(true);

  QVBoxLayout *main_layout(new QVBoxLayout(this));
  QHBoxLayout *buttons_layout(new QHBoxLayout(nullptr));

  for (auto i(0); labs.size() != i; ++i)
  {
    QPushButton *button(new QPushButton(trUtf8("#%1").arg(1 + i), this));
    button->setCheckable(true);
    buttons_layout->addWidget(button);
    buttons->addButton(button, i);
  }

  description->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  main_layout->addLayout(buttons_layout);
  main_layout->addWidget(description);

  for (Describable *w : labs)
  {
    main_layout->addWidget(w);
    w->hide();
  }

  connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(launch_lab()));
}

void Main_Widget::launch_lab()
{
  if (-1 != current)
  {
    labs[current]->hide();
    labs[current]->reinit();
    description->clear();
  }

  current = buttons->checkedId();

  // current != -1 by design
  labs[current]->show();
  description->setText(static_cast<const Describable *>(labs[current])->getDescription());
  description->updateGeometry();
  updateGeometry();
}
