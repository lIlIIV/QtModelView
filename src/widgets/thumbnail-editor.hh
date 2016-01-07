#ifndef THUMBNAIL_EDITOR_HH
#define THUMBNAIL_EDITOR_HH

#include <QWidget>

#include "clickable.hh"



class ThumbnailEditor: public QWidget
{
    Q_OBJECT
    Q_ENUMS(Actions)
public:
    ThumbnailEditor(QWidget * parent = nullptr);

    enum class Action
    {
        RotateLeft = 0,
        RotateRight,
        Delete
    };
    QSize sizeHint() const;

signals:
    void action(Action);

private:
    Clickable * m_rotate_left;
    Clickable * m_rotate_rigth;
    Clickable * m_delete;
};

#endif // THUMBNAIL_EDITOR_HH
