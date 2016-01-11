#include "thumbnail-editor.hh"

#include <QHBoxLayout>
#include <QPixmap>
#include <QVBoxLayout>

ThumbnailEditor::ThumbnailEditor(QWidget * parent)
    : QWidget(parent), m_rotate(false), m_delete(false)
{
    QPixmap pixmap(":images/rotate.png");

    Clickable * rotate = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    pixmap.load(":images/bin.png");
    Clickable * dele = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    connect(rotate, &Clickable::clicked, this, [this]{
        m_rotate = true;
        emit edit();
    });

    connect(dele, &Clickable::clicked, this, [this]{
        m_delete = true;
        emit edit();
    });

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(dele);
    layout->addStretch();
    layout->addWidget(rotate);
    layout->setSpacing(5);

    QVBoxLayout * v_layout = new QVBoxLayout(this);
    v_layout->addLayout(layout);
    v_layout->addStretch();
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
