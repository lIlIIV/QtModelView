#include "my-view.hh"

#include <QVBoxLayout>
#include <QPixmap>
#include <QScrollBar>


MyView::MyView( QWidget * parent)
    : QAbstractItemView(parent)
{
    m_document = new DocumentViewer(this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_document);
//    QGridLayout * layout = new QGridLayout( this->viewport() );
//    layout->addWidget(m_document, 0, 0);

    //m_document->setAlignment( Qt::AlignCenter );
    //m_document->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

//    document_->setText( tr("<i>No data.</i>") );

}


QModelIndex MyView::indexAt( const QPoint &point ) const
{
    return currentIndex();
}

void MyView::scrollTo( const QModelIndex &index, ScrollHint hint)
{
    Q_UNUSED(index);
    Q_UNUSED(index);
}

QRect MyView::visualRect( const QModelIndex &index ) const
{
    if( selectionModel()->selection().indexes().count() != 1 )
    {
        return QRect();
    }
    if( currentIndex() != index )
    {
        return QRect();
    }
    return rect();
}

int MyView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

bool MyView::isIndexHidden( const QModelIndex &index ) const
{
    if( currentIndex() != index )
    {
        return true;
    }
    return false;
}

QModelIndex MyView::moveCursor( CursorAction cursorAction,
                        Qt::KeyboardModifiers modifiers )
{
    return currentIndex();
}

void MyView::setSelection( const QRect &rect, QItemSelectionModel::SelectionFlags flags )
{
    Q_UNUSED(rect);
    Q_UNUSED(flags);
}

int MyView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

QRegion MyView::visualRegionForSelection( const QItemSelection &selection ) const
{
    return visualRect(currentIndex());
}

void MyView::dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);
    updateImage();
}

void MyView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    updateImage();
}

void MyView::updateImage()
{
    //QPixmap * pixmap;
    QString file_name;
    switch( selectionModel()->selection().indexes().count() )
    {
    case 0:
      //document_->setText( tr("<i>No data.</i>") );
      m_document->displayNoData();
      break;
    case 1:
      //pixmap = new QPixmap(model()->data(currentIndex(), Qt::ToolTipRole).toString());
      //document_->setPixmap(*pixmap);
      file_name = model()->data(currentIndex(), Qt::ToolTipRole).toString();
      m_document->loadDocument(file_name);
      break;
    default:
      // Too many items selected.
      // document_->setText( tr("<i>Too many items selected.<br> Can only show one item at a time.</i>") );
      m_document->displayNoData();
    break;
    }
}
