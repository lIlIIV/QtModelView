#ifndef MY_MODEL_HH
#define MY_MODEL_HH

#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QList>
#include <QMap>
#include <QModelIndex>
#include <QPixmap>
#include <QVariant>

#include "widgets/thumbnail-editor.hh"


class MyData: public QObject
{
    Q_OBJECT
public:
    MyData();
    MyData(const QFileInfo & fileInfo);
    MyData(const QFileInfo & fileInfo, const QPixmap & thumbnail, int checkState = Qt::Checked);
    MyData(const MyData &) = default;

    void loadThumbnailAsync(const QFileInfo & fileInfo);


    QFileInfo m_fileInfo;
    QPixmap m_thumbnail;
    int m_checkState;

signals:

    void thumbnailLoaded(const QFileInfo & fileInfo);
};


class MyModel: public QAbstractListModel 
{
	Q_OBJECT
public:
	MyModel(QObject * parent = NULL);
	~MyModel();
	int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList & indexes) const override;
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) override;
    Qt::DropActions supportedDropActions() const override;


    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild) override;

    bool canDropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent) const override;

    void rotateLeft(const QModelIndex & index);

    bool addRow(MyData * data, int position = -1);

    bool removeSelected();

    bool setDirectory(const QString & directory_path);

    bool deleteFileAndRemoveRow(int index);



    //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	//QHash<int, QByteArray> QAbstractItemModel::roleNames() const;
signals:
    void select(int row);
    void rowNbChanged(int row_nb);

public slots:
    void selectAll(int state);
    void rotateChecked();
//    void action(ThumbnailEditor::Action action);

private slots:
    void UpdateThumbnail(const QFileInfo & fileInfo);
    void retrieveFiles();

private:
    QModelIndex findByFilePath(const QFileInfo & file_path);


    // QList elements should be copyable QObject and subclasses arn't
    QList<MyData *> m_files;
    QFileSystemWatcher m_watcher;
    QDir m_dir;

    int m_dropperItemIndex;

    Qt::CheckState m_checkState;
};

#endif
