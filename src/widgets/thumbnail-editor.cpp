#include "thumbnail-editor.hh"

#include <QVBoxLayout>
#include <QPixmap>

ThumbnailEditor::ThumbnailEditor(QWidget * parent)
    : QWidget(parent), m_rotate(false), m_delete(false)
{
    QPixmap pixmap(":images/completed.png");

    Clickable * rotate = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    pixmap.load(":images/unstarted.png");
    Clickable * dele = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    connect(rotate, &Clickable::clicked, this, [this]{
        m_rotate = true;
        emit edit();
    });

    connect(dele, &Clickable::clicked, this, [this]{
        //emit action(Action::Delete);
        m_delete = true;
        emit edit();
    });

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(dele);

    layout->addWidget(rotate);
    layout->setSpacing(5);

    layout->setSizeConstraint(QLayout::SetFixedSize);
}

QSize ThumbnailEditor::sizeHint() const
{
  return QSize( 140, 140 );
}

void ThumbnailEditor::done()
{
    m_rotate = false;
    m_delete = false;
}

bool ThumbnailEditor::rotateThumbnail()
{
    return m_rotate;
}

bool ThumbnailEditor::deleteThumbnail()
{
    return m_delete;
}
