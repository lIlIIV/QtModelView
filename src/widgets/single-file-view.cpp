#include "single-file-view.hh"

#include <QHBoxLayout>

#include "my-style.hh"

namespace {
    const int A4_W = 730; //400; //(A4_H*21)/29;
    const int A4_H = 320; //(29*A4_W)/21;
    const int Thumbnail_view_W = 140;
    const int Thumbnail_view_spacing = 5;
}

SingleFileView::SingleFileView(QWidget * parent)
    : QWidget(parent),
      m_thumbnails(new QListView),
      m_file_view(new MyView)
{
    initThumbnails();
    initSingleFileView();

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_thumbnails);
    layout->addWidget(m_file_view);
}

SingleFileView::~SingleFileView()
{

}

void SingleFileView::initThumbnails()
{
    // Display Icons
    m_thumbnails->setViewMode(QListView::IconMode);

    // Use Grid Layout
    //list_view->setGridSize(QSize(160,200));

    // Items can be moved by the user only to grid positions
    m_thumbnails->setMovement(QListView::Snap);

    // Set size
    m_thumbnails->setMaximumWidth(Thumbnail_view_W);
    m_thumbnails->setMinimumWidth(Thumbnail_view_W);
    m_thumbnails->setSpacing(Thumbnail_view_spacing);

    // Remove horisontal scroll

    m_thumbnails->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_thumbnails->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Set selection mode : QAbstractItemView::ExtendedSelection, QAbstractItemView::SingleSelection, QAbstractItemView::ContiguousSelection
    m_thumbnails->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Enable Drag and Drop mode : QAbstractItemView::NoDragDrop, .. QAbstractItemView::DragDrop, QAbstractItemView::InternalMove
    m_thumbnails->setDragDropMode(QAbstractItemView::InternalMove);

    // Show Drop indicator
    m_thumbnails->setDropIndicatorShown(true);

    // (Un)Set Focus

    m_thumbnails->setFocusPolicy(Qt::StrongFocus);

    // set triggers
    m_thumbnails->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void SingleFileView::setThumbnailStyleSheet(const QString & style_sheet)
{
    // Set style
    m_thumbnails->setStyle(new MyStyle(m_thumbnails->style()));
    m_thumbnails->setStyleSheet(style_sheet);
}

void SingleFileView::initSingleFileView()
{
    m_file_view->setMinimumSize(QSize(A4_W, A4_H));
    m_file_view->setFocusPolicy(Qt::StrongFocus);
}

void SingleFileView::setThumbnailDelegate(QAbstractItemDelegate * delegate)
{
    m_thumbnails->setItemDelegate(delegate);
}


void SingleFileView::setModel(QAbstractItemModel * model)
{
    m_thumbnails->setModel(model);
    m_file_view->setModel(model);
}

void SingleFileView::setSelectionModel(QItemSelectionModel * selection_model)
{
    m_thumbnails->setSelectionModel(selection_model);
    m_file_view->setSelectionModel(selection_model);
}

DocumentViewer * SingleFileView::documentViewer()
{
    return m_file_view->documentViewer();
}

void SingleFileView::setFocusOnThumbnailList()
{
    m_thumbnails->setFocus();
}
