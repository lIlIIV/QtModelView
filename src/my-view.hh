#ifndef MY_VIEW_HH
#define MY_VIEW_HH

#include <QAbstractItemView>
#include <QLabel>

#include "widgets/document-viewer.hh"

class MyView : public QAbstractItemView
{
    Q_OBJECT
public:

  MyView( QWidget *parent = 0 );

  QModelIndex indexAt(const QPoint & point) const override;
  void scrollTo(const QModelIndex & index, ScrollHint hint = EnsureVisible) override;
  QRect visualRect(const QModelIndex & index) const override;

  DocumentViewer * documentViewer() { return m_document;}

protected:
  int horizontalOffset() const override;
  bool isIndexHidden(const QModelIndex & index) const override;
  QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
  void setSelection(const QRect & rect, QItemSelectionModel::SelectionFlags flags) override;
  int verticalOffset() const override;
  QRegion visualRegionForSelection(const QItemSelection & selection) const override;

protected slots:
  void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ()) override;
  void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected) override;

private:
  void updateImage();
  DocumentViewer * m_document;

};

#endif
