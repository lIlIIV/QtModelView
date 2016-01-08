#include "my-style.hh"

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QPoint>
#include <QStyleOption>

MyStyle::MyStyle(QStyle * style)
    : QProxyStyle(style)
{

}

void MyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
{
    if (element == QStyle::PE_IndicatorItemViewItemDrop)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        QColor c("#a6a6a6");
        QPen pen(c);
        pen.setWidth(3);
        c.setAlpha(50);
        QBrush brush(c);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(QPoint(option->rect.topLeft().x() + 6, option->rect.topLeft().y() - 5), QPoint(option->rect.topRight().x() - 6, option->rect.topLeft().y() - 5));
    }
    else
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
