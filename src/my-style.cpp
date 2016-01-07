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

//        QPalette palette;
//        QColor c(palette.highlightedText().color());
//        QPen pen(c);
//        pen.setWidth(2);
//        c.setAlpha(50);
//        QBrush brush(c);

//        painter->setPen(pen);
//        painter->setBrush(brush);
//        if(option->rect.height() == 0)
//        {
//            painter->drawEllipse(option->rect.topLeft(), 3, 3);
//            painter->drawLine(QPoint(option->rect.topLeft().x()+3, option->rect.topLeft().y()), option->rect.topRight());
//        }
//        else
//        {
//            painter->drawLine(QPoint(option->rect.topLeft().x()+3, option->rect.topLeft().y()), option->rect.topRight());
//            // painter->drawRoundedRect(option->rect, 5, 5);
//        }
        painter->drawLine(QPoint(option->rect.topLeft().x()+3, option->rect.topLeft().y()), option->rect.topRight());
    }
    else
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
