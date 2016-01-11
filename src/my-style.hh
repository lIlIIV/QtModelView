#ifndef MY_STYLE_HH
#define MY_STYLE_HH

#include <QColor>
#include <QProxyStyle>

class MyStyle: public QProxyStyle
{
    Q_OBJECT
public:
    MyStyle(QStyle * style = 0);

    void drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0) const override;

    void setDropIndicatorPosition(int x, int y);
    void setDropIndicatorColor(QColor color);

private:
    int m_position_x;
    int m_position_y;
    QColor m_color;
};

#endif // MY_STYLE_HH
