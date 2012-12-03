#ifndef LAB3_WIDGET_H
#define LAB3_WIDGET_H

class QWidget;
/**
 * for std::function
 */
#include <functional>
/**
 * for Describable interface, obv
 */
#include "../describable/describable.hxx"

class Lab4_Widget : public Describable
{
    Q_OBJECT

public:
    /**
     * @brief Default ctor for QObject childs >_>
     * @param parent    Parent of this widget
     */
    Lab4_Widget(QWidget *parent = nullptr);
    /**
     * @brief Destructor :>
     */
    virtual ~Lab4_Widget();
private:
    /**
     * @brief reinit    reimplemented method from Describable interface. It's used to restore init state of widget
     */
    virtual void reinit();
};

#endif // LAB3_WIDGET_H
// kate: indent-mode cstyle; indent-width 4; replace-tabs on;
