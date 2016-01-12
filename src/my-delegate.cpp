#include "my-delegate.hh"

#include <QApplication>
#include <QBoxLayout>
#include <QEvent>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter> 
#include <QPen>
#include <QPixmap>
#include <QStyleOptionButton>

#include "my-model.hh"
#include "widgets/thumbnail-editor.hh"

MyDelegate::MyDelegate(QObject * parent)
    : QStyledItemDelegate(parent),
      m_page_nb_position_x(0), m_page_nb_position_y(1),
        m_page_nb_margin_x(10), m_page_nb_margin_y(10)
{

}

void MyDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    QFontMetrics fm(option.font);
    QString display_index = QString("%1").arg(index.row()+1);
    double text_width = fm.width(display_index);
    double text_height = fm.height();
    QRect text_rect(0, 0, text_width, text_height);

    double x = m_page_nb_position_x * (option.rect.width() - text_width - 2. * m_page_nb_margin_x) + m_page_nb_margin_x;
    double y = m_page_nb_position_y * (option.rect.height() - text_height - 2. * m_page_nb_margin_y) + m_page_nb_margin_y;

    text_rect.moveTo(option.rect.x() + x, option.rect.y() + y);

    painter->drawText(text_rect , Qt::AlignCenter, display_index);
}

QSize MyDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    QSize icon = index.data(Qt::SizeHintRole).toSize();
    return QSize( icon.width() + 40, icon.height() + 11);
}

QWidget * MyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    ThumbnailEditor * te = new ThumbnailEditor(parent);
    connect(te, &ThumbnailEditor::edit, this, &MyDelegate::edit);
    return te;
}

void MyDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void MyDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    MyModel * m = dynamic_cast<MyModel *>(model);
    if(m == nullptr)
    {
        return;
    }

    ThumbnailEditor * th_editor = dynamic_cast<ThumbnailEditor *>(editor);
    if(th_editor == nullptr)
    {
        return;
    }

    if(th_editor->rotateThumbnail())
    {
        m->rotateLeft(index);
        th_editor->done();
    }

    if(th_editor->deleteThumbnail())
    {
        m->deleteFileAndRemoveRow(index.row());
        th_editor->done();
    }
}

void MyDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
}

void MyDelegate::edit()
{
    emit commitData(qobject_cast<QWidget *>(sender()));
}

void MyDelegate::setPageNbPosition(double x, double y, double margin_x, double margin_y)
{
    m_page_nb_position_x = x;
    m_page_nb_position_y = y;
    m_page_nb_margin_x = margin_x;
    m_page_nb_margin_y = margin_y;
}
