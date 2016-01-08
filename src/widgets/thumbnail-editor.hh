#ifndef THUMBNAIL_EDITOR_HH
#define THUMBNAIL_EDITOR_HH

#include <QWidget>

#include "widgets/clickable.hh"

class ThumbnailEditor: public QWidget
{
    Q_OBJECT
public:
    ThumbnailEditor(QWidget * parent = nullptr);
    QSize sizeHint() const;

    void done();
    bool rotateThumbnail();
    bool deleteThumbnail();

signals:
    void edit();

private:
    bool m_rotate;
    bool m_delete;
};

#endif // THUMBNAIL_EDITOR_HH
