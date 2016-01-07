#include "thumbnail-editor.hh"

#include <QVBoxLayout>
#include <QPixmap>

ThumbnailEditor::ThumbnailEditor(QWidget * parent)
    : QWidget(parent)
{
    QPixmap pixmap(":images/completed.png");

    m_rotate_left = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    pixmap.load(":images/uncompleted.png");
    m_rotate_rigth = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    pixmap.load(":images/unstarted.png");
    m_delete = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    connect(m_rotate_left, &Clickable::clicked, this, [this]{
        emit action(Action::RotateLeft);
    });

    connect(m_rotate_rigth, &Clickable::clicked, this, [this]{
        emit action(Action::RotateRight);
    });

    connect(m_delete, &Clickable::clicked, this, [this]{
        emit action(Action::Delete);
    });

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_rotate_left);
    layout->addWidget(m_rotate_rigth);
    layout->addWidget(m_delete);
    layout->addSpacing(5);
    layout->setSizeConstraint(QLayout::SetFixedSize);
}

QSize ThumbnailEditor::sizeHint() const
{
  return QSize( 20, 55 );
}

