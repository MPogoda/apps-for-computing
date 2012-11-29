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
    Describable(const QString& _description, QWidget *parent = nullptr);
    virtual ~Describable();
    virtual void reinit() = 0;
    const QString& getDescription() const;
};

#endif
