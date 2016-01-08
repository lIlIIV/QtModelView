#include "widgets/clickable.hh"

Clickable::Clickable(const QPixmap & image, QWidget * parent)
    : QLabel(parent), saved_(false)
{
    setPixmap(image);
}

Clickable::~Clickable()
{
}

void Clickable::mousePressEvent(QMouseEvent * /*event*/)
{
    emit clicked();
}
