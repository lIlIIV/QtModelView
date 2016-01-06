#include "my-delegate.hh"

#include <QApplication>
#include <QBoxLayout>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter> 
#include <QPen>
#include <QPixmap>
#include <QStyleOptionButton>


#include "clickable.hh"
#include "my-model.hh"
#include "widgets/thumbnail-editor.hh"

MyDelegate::MyDelegate(QObject * parent)
    : QStyledItemDelegate(parent)
{

}

void MyDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{

    QStyledItemDelegate::paint(painter, option, index);

//      OK
//    painter->save();
//    if (option.state & QStyle::State_Selected)
//    {
//        // painter->fillRect(option.rect, option.palette.highlight());
//        painter->fillRect(option.rect, QBrush(QColor("#ed783d")));
//    }
//    else
//    {
//        painter->fillRect(option.rect, QBrush(QColor("#6db8de")));

//    }

//    // ..

//    QPixmap pm = index.data(Qt::DecorationRole).value<QPixmap>();

//    painter->drawPixmap( option.rect.x() + 20, option.rect.y() + 4, pm.width(), pm.height(), pm);

//    QString display_index = index.data(Qt::DisplayRole).toString();

//    QRect text_rect(option.rect.x() + 2, option.rect.y() + option.rect.height() - 18 , 15, 15 );
    
//    painter->drawText(text_rect , Qt::AlignBottom, display_index );

   // TESTS

//    int checked_state = index.data(Qt::CheckStateRole).toInt();

//////    QRect checkbox_rect(option.rect.x() + 2, option.rect.y() + 4, 15, 15 );

////    QStyleOptionButton checkboxstyle;

////    checkboxstyle.rect = option.rect;

//////    checkboxstyle.rect = checkbox_rect;

//////    checkboxstyle.rect.setLeft(option.rect.x() + 2);

////    checkboxstyle.state = (checked_state == Qt::Checked) ? (QStyle::State_On | QStyle::State_Enabled) :
////                                                           (QStyle::State_Off | QStyle::State_Enabled);

////    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);


//    // Fill style options with item data
//    const QStyle * style = QApplication::style();
//    QStyleOptionButton opt;
//    opt.state |= (checked_state == Qt::Checked) ? QStyle::State_On : QStyle::State_Off;
//    opt.state |= QStyle::State_Enabled;
//    //opt.text = text;
//    opt.rect = option.rect;

//    // Draw item data as CheckBox
//    style->drawControl(QStyle::CE_CheckBox, &opt, painter);

//    painter->restore();


////    int checked_state = index.model()->data(index, Qt::CheckStateRole).toInt();

////    QStyleOptionButton checkboxstyle;

////    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

////    checkboxstyle.rect = option.rect;

////    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);

////    if(checked_state == Qt::Checked)
////    {
////        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
////    }
////    else
////    {
////        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;
////    }

////    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);

}

QSize MyDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
//    QPixmap pm = index.data(Qt::DecorationRole).value<QPixmap>();
    return QSize(160, 180);
}

QWidget * MyDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);

    ThumbnailEditor * te = new ThumbnailEditor(parent);
    connect(te, &ThumbnailEditor::action, this, &MyDelegate::setRotateLeft);
    return te;

//    QPixmap pixmap(":images/completed.png");
//    Clickable * rotate = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation), parent);
//    connect(rotate, &Clickable::clicked, this, &MyDelegate::setRotateLeft);
//    rotate->Saved(true);
//    return rotate;
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

    m->rotateLeft(index);
//    Clickable * cl =  dynamic_cast<Clickable *>(editor);
//    if(!cl->isSaved())
//    {
//        m->rotateLeft(index);
//        cl->Saved(true);
//    }

}

void MyDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect.x() + 2, option.rect.y() + 2, editor->width(), editor->height());
}

void MyDelegate::setRotateLeft()
{
    emit commitData(qobject_cast<QWidget *>(sender()));
}

bool MyDelegate::eventFilter(QObject * editor, QEvent * event)
{
//    Clickable * cl =  dynamic_cast<Clickable *>(editor);
//    if(cl != nullptr && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick))
//    {
//        QMouseEvent * me = dynamic_cast<QMouseEvent *>(event);
//        if(me != nullptr)
//        {
//            if( me->button() == Qt::LeftButton)
//            {
//                cl->Saved(false);
//            }
//        }
//    }
    return QStyledItemDelegate::eventFilter(editor, event);
}

bool MyDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
