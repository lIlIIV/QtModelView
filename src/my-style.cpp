#include "my-style.hh"

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QPoint>
#include <QStyleOption>

MyStyle::MyStyle(QStyle * style)
    : QProxyStyle(style),
        m_position_x(6), m_position_y(5), m_color("#a6a6a6")
{

}

void MyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget) const
{
    if (element == QStyle::PE_IndicatorItemViewItemDrop)
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        QColor color(m_color);
        QPen pen(color);
        pen.setWidth(3);
        color.setAlpha(50);
        QBrush brush(color);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(QPoint(option->rect.topLeft().x() + m_position_x, option->rect.topLeft().y() - m_position_y),
                          QPoint(option->rect.topRight().x() - m_position_x, option->rect.topLeft().y() - m_position_y));
    }
    else
    {
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

void MyStyle::setDropIndicatorPosition(int x, int y)
{
    m_position_x = x;
    m_position_y = y;
}

void MyStyle::setDropIndicatorColor(QColor color)
{
    m_color = color;
}
