#include "describable.hxx"

Describable::Describable(const QString& _description, QWidget *parent) :
  QWidget(parent),
  description(_description)
{
}

Describable::~Describable()
{
}

const QString& Describable::getDescription() const
{
  return description;
}
