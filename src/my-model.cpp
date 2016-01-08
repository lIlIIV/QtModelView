#include "my-model.hh"

#include <future>

#include <QBrush>
#include <QDir>
#include <QFileInfoList>
#include <QImage>
#include <QMessageBox>
#include <QMimeData>
#include <QModelIndexList>
#include <QPointer>
#include <QString>
#include <QTimer>
#include <QTransform>

#include "tools/dispatch.hh"

namespace {
    const int A4_H = 140;
    const int A4_W = (A4_H*21)/29;
    const QString default_image = ":images/unstarted.png";
}

MyData::MyData()
    : m_thumbnail(QPixmap(default_image)), m_checkState(false)
{
    m_thumbnail = m_thumbnail.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

MyData::MyData(const QFileInfo & fileInfo)
    : m_fileInfo(fileInfo), m_thumbnail(QPixmap(default_image)), m_checkState(false)
{
    m_thumbnail = m_thumbnail.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

MyData::MyData(const QFileInfo & fileInfo, const QPixmap & thumbnail, int checkState)
    : m_fileInfo(fileInfo), m_thumbnail(thumbnail), m_checkState(checkState)
{
    m_thumbnail = m_thumbnail.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void MyData::loadThumbnailAsync(const QFileInfo & fileInfo)
{
    QPointer<QObject> weak_this(this);

    dispatchAsync(weak_this, [this, weak_this, fileInfo] {
        QImage img;
        bool res = img.load(fileInfo.absoluteFilePath());
        if(!res)
        {

        }

        img = img.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        QPixmap  pmap =  QPixmap::fromImage(img);

        dispatchAsyncMain(weak_this, [this, res, pmap, fileInfo] {
//             FIXME
             if (res == false)
             {
                 QTimer::singleShot(1000, this, [this, fileInfo]{
                    loadThumbnailAsync(fileInfo);
                 });
             }
             else
            if(res)
            {
                m_thumbnail = pmap;
                thumbnailLoaded(fileInfo);
            }
        });
    });
}


MyModel::MyModel(QObject * parent)
    : QAbstractListModel(parent)
{
    QDir scanner("/Users/liliaivanova/.Ipso/Scanner");
   //QDir scanner("/Users/liliaivanova/Desktop/Numbers");
////    QDir scanner("/Users/liliaivanova/Desktop");

//    QStringList filters;
//    filters << "*.jpg" << "*.png";

//    QFileInfoList fileList = scanner.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

//    for(int i = 0; i < fileList.count(); i ++)
//    {
//        MyData * data = new MyData(fileList[i]);
//        data->loadThumbnailAsync(fileList[i]);

//        m_files.append(data);
////        m_files << data;
////        filesInfo_ << fileList.at(i);
////        QPixmap image;
////        image.load(default_image);
////        thumbnails_ << image.scaled(W, H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //, Qt::KeepAspectRatio, Qt::SmoothTransformation);
////        checkState_ << Qt::Unchecked;
////        loadThumbnailAsync(fileList[i]);
//    }

    retrieveFiles();
    bool out = m_watcher.addPath(scanner.absolutePath());
    if(!out)
    {
        // LOG/ERROR here
    }

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &MyModel::retrieveFiles);
}

MyModel::~MyModel()
{
	for(int i = 0; i < m_files.count(); i ++)
	{
		delete m_files[i];
	}
}

int MyModel::rowCount(const QModelIndex & parent) const
{
    return m_files.count();
}

QVariant MyModel::data(const QModelIndex & index, int role) const
{
    //  valid index belongs to a model, and has non-negative row and column numbers.
    if(!index.isValid())
    {
        return QVariant();
    }

    int row = index.row();

    // do we need that?
    if(row >= rowCount())
    {
        return QVariant();
    }

    switch(role)
	{
    case Qt::CheckStateRole:
        return m_files[row]->m_checkState;
        break;
    case Qt::DecorationRole:
        return m_files[row]->m_thumbnail;
        break;
 //   case Qt::DisplayRole:
 //       return row + 1;
//		break;
    case Qt::ToolTipRole:
        return m_files[row]->m_fileInfo.absoluteFilePath();
		break;
    case Qt::SizeHintRole:
        return QSize(A4_W, A4_H);
    break;
    default:
        return QVariant();
		break;
	}	
}


bool MyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if(!index.isValid())
    {
        return true;
    }

	int row = index.row();

	switch(role)
	{
		case Qt::DisplayRole:
		break;
		case Qt::ToolTipRole:
            m_files[row]->m_fileInfo = QFileInfo(value.toString());
            emit dataChanged(index, index, {Qt::ToolTipRole});
		break;
        case Qt::DecorationRole:
            m_files[row]->m_thumbnail = value.value<QPixmap>();
            emit dataChanged(index, index, {Qt::DecorationRole});
        break;
        case Qt::CheckStateRole:
            m_files[row]->m_checkState = value.toInt();
            emit dataChanged(index, index, {Qt::CheckStateRole});
        break;
    	default:
    	break;
    }
    return true;
}

Qt::ItemFlags MyModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if(index.isValid())
    {
        return defaultFlags | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
    }
    else
    {
        return defaultFlags | Qt::ItemIsDropEnabled;
    }
}


QStringList MyModel::mimeTypes() const
{
    QStringList types;
    types << "image/my-type";
    return types;
}

QMimeData * MyModel::mimeData(const QModelIndexList & indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for(int i = 0; i < indexes.count(); i ++)
    {
        if (indexes[i].isValid())
        {
            QPixmap pixmap = data(indexes[i], Qt::DecorationRole).value<QPixmap>();

            int check_State = data(indexes[i], Qt::CheckStateRole).toInt();

            QString tool_tip = data(indexes[i], Qt::ToolTipRole).toString();

            stream << pixmap << check_State << tool_tip;
        }
    }

    mimeData->setData("image/my-type", encodedData);
    return mimeData;
}

bool MyModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (!data->hasFormat("image/my-type"))
    {
        return false;
    }

    int endRow;

    if (row != -1)
    {
        endRow = row;
    }
    else if (parent.isValid())
    {
        endRow = parent.row();
    }
    else
    {
        endRow = 0;
    }

    QByteArray encodedData = data->data("image/my-type");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    // FIXME get all elements and insert them in one call
    while (!stream.atEnd())
    {
        QPixmap pixmap;
       int check_state;
       QString tool_tip;

       stream >> pixmap >> check_state >> tool_tip;

       beginInsertRows(QModelIndex(), endRow, endRow);

       MyData * newRow = new MyData(tool_tip, pixmap, check_state);
       m_files.insert(endRow, newRow);

      endInsertRows();

       ++endRow;
    }

    return true;
}

bool MyModel::addRow(MyData * data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_files.append(data);
    endInsertRows();
    return true;
}

 Qt::DropActions MyModel::supportedDropActions() const
 {
     return Qt::MoveAction;
 }

 bool MyModel::removeRows(int row, int count, const QModelIndex & parent)
 {
    int currentRowNumber =  rowCount();
    if(row < 0 || row + count > currentRowNumber)
    {
        return false;
    }

    beginRemoveRows(parent, row, row + count);
    for(int i = row; i < row + count; i ++)
    {
        delete m_files[i];
        m_files.removeAt(i);
    }
    endRemoveRows();

    return true;
 }

 bool MyModel::insertRows(int row, int count, const QModelIndex & parent)
 {
    return QAbstractItemModel::insertRows(row, count, parent);
 }


 bool MyModel::canDropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) const
 {
    if (!parent.isValid() && column==-1 && row==-1)
    {
        return false;
    }
    return QAbstractItemModel::canDropMimeData(data, action, row, column, parent);
 }

 bool MyModel::moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild)
 {
     return QAbstractItemModel::moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
 }

// void MyModel::loadThumbnailAsync(const QFileInfo & file_path)
// {
//     QPointer<QObject> weak_this(this);

//     dispatchAsync(weak_this, [this, weak_this, file_path] {
//         QImage img;
//         bool res = img.load(file_path.absoluteFilePath());
//         img = img.scaled(W, H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//         QPixmap  pmap =  QPixmap::fromImage(img);

//         dispatchAsyncMain(weak_this, [this, res, pmap, file_path] {
////             FIXME
////             if (res == false)
////             {
////                 QTimer::singleShot(1000, this, [this, file_path]{
////                    loadThumbnailAsync(file_path);
////                 });
////             }
////             else
//             if(res)
//             {
//                 QModelIndex index = findByFilePath(file_path);
//                 if(index.isValid())
//                 {
//                     setData(index, QVariant(pmap), Qt::DecorationRole);
//                 }
//             }
//         });
//     });
// }

 QModelIndex MyModel::findByFilePath(const QFileInfo & file_path)
 {
     QModelIndex start = index(0, 0);
     QVariant value(file_path.absoluteFilePath());

     QModelIndexList items = match(start, Qt::ToolTipRole, value, 1, Qt::MatchExactly);
     if(items.isEmpty())
     {
         return QModelIndex();
     }
     else
     {
         return items[0];
     }
 }

 void MyModel::rotateLeft(const QModelIndex & index)
 {
    if(!index.isValid())
    {
        return;
    }

    QString file_path = index.data(Qt::ToolTipRole).toString();
    QImage image;
    if(image.load(file_path))
    {
        QTransform transformer;
        image = image.transformed(transformer.rotate(90));

        if (image.save(file_path) == true)
        {
            image = image.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            QPixmap  pmap =  QPixmap::fromImage(image);
            setData(index, pmap, Qt::DecorationRole);
            return;
        }
    }

    QMessageBox msgBox;
    msgBox.setText("erreur");
    msgBox.setInformativeText("la rotation a échoué");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
    // QMessageBox::warning(this->parent(), "erreur", "la rotation a échoué");
 }

 void MyModel::selectAll(int state)
 {
    for(int i = 0; i < rowCount(); i++)
    {
        if(m_files[i]->m_checkState != state)
        {
            setData(index(i, 0), state, Qt::CheckStateRole);
        }
    }
 }

 void MyModel::rotateChecked()
 {
     for(int i = 0; i < rowCount(); i++)
     {
         if(m_files[i]->m_checkState == Qt::Checked)
         {
             rotateLeft(index(i, 0));
         }
     }
 }

 bool MyModel::removeSelected()
 {
    int rows = rowCount();
    for(int i = 0; i < rows; i++)
    {
        if(m_files[i]->m_checkState == Qt::Checked)
        {
            if(!removeRow(i))
            {
                return false;
            }
            rows--;
            i--;
        }
    }
    return true;
 }

 void MyModel::retrieveFiles(const QString &path)
 {
     //QDir scanner("/Users/liliaivanova/Desktop/Numbers");
     QDir scanner("/Users/liliaivanova/.Ipso/Scanner");

     QStringList filters;
     filters << "*.jpg" << "*.png";

     QFileInfoList fileList = scanner.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

     QList<QString> files;

     for(int i = 0; i < fileList.count(); i ++)
     {
        MyData * data = new MyData(fileList[i]);
        //data->loadThumbnailAsync(fileList[i]);
        files.append(fileList[i].absoluteFilePath());

        QModelIndex in = findByFilePath(fileList[i]);

        if( !in.isValid() )
        {
            //m_files.append(data);
            addRow(data);
            connect(data, &MyData::thumbnailLoaded, this, &MyModel::UpdateThumbnail);
            data->loadThumbnailAsync(fileList[i]);
        }
        else
        {
            delete data;
        }
     }

    int rows = rowCount();
    for(int i = 0; i < rows; i++)
    {
         QString file_path = data(index(i,0), Qt::ToolTipRole).toString();
         if(!files.contains(file_path))
         {
             if(!removeRow(i))
             {
                 // TODO Log errror here
                 break;
             }
             rows--;
             i--;
         }
     }
}

void MyModel::UpdateThumbnail(const QFileInfo & fileInfo)
{
    QModelIndex in = findByFilePath(fileInfo.absoluteFilePath());

    if( in.isValid() )
    {
        emit dataChanged(in, in, {Qt::DecorationRole});
    }
    else
    {
        //
    }
}


//void MyModel::action(ThumbnailEditor::Action action)
//{
//    switch(action)
//    {
//    case ThumbnailEditor::Action::RotateLeft:
//        rotateChecked();
//        break;
//    case ThumbnailEditor::Action::RotateRight:
//        break;
//    case ThumbnailEditor::Action::Delete:
//        removeSelected();
//        break;
//    default:
//        break;

//    }
//}
