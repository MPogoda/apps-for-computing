#ifndef DESCRIBABLE_H
#define DESCRIBABLE_H

#include <QWidget>
#include <QString>

class Describable : public QWidget
{
  Q_OBJECT

  private:
    const QString description;
  public:
    Describable(const QString& _description, QWidget *parent = nullptr)
      : QWidget(parent), description(_description) { }
    const QString& getDescription() const { return description; }
    virtual void reinit() = 0;
};

#endif
