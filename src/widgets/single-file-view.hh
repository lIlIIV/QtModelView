#ifndef SINGLE_FILE_VIEW
#define SINGLE_FILE_VIEW

#include <QAbstractItemDelegate>
#include <QAbstractItemModel>
#include <QListView>
#include <QWidget>

#include "my-view.hh"

class SingleFileView: public QWidget
{
Q_OBJECT
public:
    SingleFileView(QWidget * parent = 0);
    ~SingleFileView();

    void setThumbnailStyleSheet(const QString & style_sheet);
    void setModel(QAbstractItemModel * model);
    void setThumbnailDelegate(QAbstractItemDelegate * delegate);
    void setSelectionModel(QItemSelectionModel * selection_model);
    DocumentViewer * documentViewer();
    void setFocusOnThumbnailList();

private:
    void initThumbnails();
    void initSingleFileView();

    QListView * m_thumbnails;
    MyView * m_file_view;
};

#endif // SINGLE_FILE_VIEW
