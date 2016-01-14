#include "my-model.hh"


#include <QBrush>
#include <QFileInfoList>
#include <QFile>
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
    : m_thumbnail(QPixmap(default_image)), m_checkState(Qt::Unchecked)
{
    m_thumbnail = m_thumbnail.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

MyData::MyData(const QFileInfo & fileInfo)
    : m_fileInfo(fileInfo), m_thumbnail(QPixmap(default_image)), m_checkState(Qt::Unchecked)
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
        QPixmap  pmap;
        bool res = img.load(fileInfo.absoluteFilePath());
        if(res)
        {
            img = img.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            pmap =  QPixmap::fromImage(img);
        }
        dispatchAsyncMain(weak_this, [this, res, pmap, fileInfo] {
            if (res)
            {
                m_thumbnail = pmap;
                thumbnailLoaded(fileInfo);
            }
            else
            {
                QTimer::singleShot(1000, this, [this, fileInfo] {
                    loadThumbnailAsync(fileInfo);
                });
            }
        });
    });
}

void MyData::rotate()
{
    QPointer<QObject> weak_this(this);

    dispatchAsync(weak_this, [this, weak_this] {
        QImage img;
        QPixmap  pmap;
        QString file_path = m_fileInfo.absoluteFilePath();
        bool res = img.load(file_path);
        if(res)
        {
            QTransform transformer;
            img = img.transformed(transformer.rotate(90));
            if (img.save(file_path) == true)
            {
                img = img.scaled(A4_W, A4_H, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                pmap =  QPixmap::fromImage(img);

                dispatchAsyncMain(weak_this, [this, res, pmap] {
                    if (res)
                    {
                        m_thumbnail = pmap;
                        thumbnailLoaded(m_fileInfo);
                    }
                    else
                    {
                        QTimer::singleShot(1000, this, [this] {
                            rotate();
                        });
                    }
                });
            }
        }
    });
}

void MyData::deleteFile()
{
    QPointer<QObject> weak_this(this);

    dispatchAsync(weak_this, [this, weak_this] {
        QString file_path = m_fileInfo.absoluteFilePath();
        QFile file(file_path);
        file.remove();

        //TODO check the result
    });
}

MyModel::MyModel(QObject * parent)
    : QAbstractListModel(parent)
{
    // QString scanner("/Users/liliaivanova/.Ipso/Scanner");
    QString scanner("/Users/liliaivanova/Desktop/Numbers");
    ////QString scanner("/Users/liliaivanova/Desktop");
    //setDirectory(scanner);
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
//    case Qt::CheckStateRole:
//        return m_files[row]->m_checkState;
//        break;
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
        //return QSize( m_files[row]->m_thumbnail.width() + 40, m_files[row]->m_thumbnail.height() + 11);
        return QSize( m_files[row]->m_thumbnail.width() + 11, m_files[row]->m_thumbnail.height() + 11);
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
//        case Qt::CheckStateRole:
//            m_files[row]->m_checkState = value.toInt();
//            emit dataChanged(index, index, {Qt::CheckStateRole});
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
        return defaultFlags /*| Qt::ItemIsUserCheckable*/ | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
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
    emit dragStarted();
    return mimeData;
}

bool MyModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
    emit dragEnded();
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

    while (!stream.atEnd())
    {
        QPixmap pixmap;
        int check_state;
        QString tool_tip;

        stream >> pixmap >> check_state >> tool_tip;

        MyData * new_row = new MyData(tool_tip, pixmap, check_state);

        addRow(new_row, endRow, false);

        ++endRow;
    }

    return true;
}

bool MyModel::addRow(MyData * data, int position, bool signal_new_row)
{
    if (data == nullptr)
    {
        return false;
    }
    if (position < 0)
    {
        position = rowCount();
    }
    beginInsertRows(QModelIndex(), position, position);
    m_files.insert(position, data);
    endInsertRows();

    // when a new row is added it shall be selected
    // ! we dont want to signal new row if it has been draged
    if(signal_new_row)
    {
        emit rowAdded(position);
    }
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
        row--;
        count--;
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
 }


void MyModel::rotate(const QModelIndex & index)
{
    if(index.isValid())
    {
        m_files[index.row()]->rotate();
    }
}

bool MyModel::deleteFile(int row)
{
    if(row < 0 || row < rowCount())
    {
        return false;
    }

    QFile file(m_files[row]->m_fileInfo.absoluteFilePath());
    if(!file.remove())
    {
        return false;
    }

    return true;
}

 void MyModel::retrieveFiles()
 {
     if(!m_dir.exists())
     {
         return;
     }

     QStringList filters;
     filters << "*.jpg" << "*.png";

     QFileInfoList fileList = m_dir.entryInfoList(filters, QDir::Files | QDir::NoSymLinks);

     QList<QString> files;

     //int rows_nb_before = rowCount();

     for(int i = 0; i < fileList.count(); i ++)
     {
        MyData * data = new MyData(fileList[i]);
        files.append(fileList[i].absoluteFilePath());

        QModelIndex in = findByFilePath(fileList[i]);

        if( !in.isValid() )
        {
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
                return;
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

bool MyModel::setDirectory(const QString & directory_path)
{
    m_dir.setPath(directory_path);
    if(!m_dir.exists())
    {
        if(!m_dir.mkdir(directory_path))
        {
            // Log error here
            return false;
        }
        m_dir.cd(directory_path);
    }

    retrieveFiles();
    bool out = m_watcher.addPath(m_dir.absolutePath());
    if(!out)
    {
        // LOG/ERROR here
        return false;
    }

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, [this] {
        retrieveFiles();
    });
    return true;
}


void MyModel::deleteFile(const QString & file_path)
{
    QModelIndex index =  findByFilePath(file_path);
    if(index.isValid())
    {
        m_files[index.row()]->deleteFile();
    }
}
