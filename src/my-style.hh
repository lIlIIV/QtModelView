#ifndef MY_STYLE_HH
#define MY_STYLE_HH

#include <QProxyStyle>

class MyStyle: public QProxyStyle
{
    Q_OBJECT
public:
    MyStyle(QStyle * style = 0);

    void drawPrimitive(PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0) const;
};

#endif // MY_STYLE_HH
