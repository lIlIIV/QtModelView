#include "main-window.hh"

#include <QDataWidgetMapper>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

#include "my-view.hh"
#include "my-style.hh"
#include "tools/dispatch.hh"
#include "widgets/document-viewer.hh"

namespace {
    const QString default_image = ":images/unstarted.png";
}

MainWindow::MainWindow()
    : grid_view(new QListView(this)),
        single_file_view(new SingleFileView(this)),
        my_model(new MyModel(this)),
        my_delegate(new MyDelegate(this)),
        selection_model(new QItemSelectionModel(my_model, this)),
        m_rows(0)
{
    resize(1200, 600);

    QFile styleFile(":style/style.qss");
    QString styleSheet;
    if (styleFile.open(QIODevice::ReadOnly))
    {
        styleSheet = styleFile.readAll();
        // add log here
    }

    // Single file view
    // Set the model to thumbnail view
    single_file_view->setModel(my_model);
    // Set the delegate
    single_file_view->setThumbnailDelegate(my_delegate);
    // Set the stylesheet
    single_file_view->setThumbnailStyleSheet(styleSheet);
    // Set the selection model
    single_file_view->setSelectionModel(selection_model);

    // Grid view
    initGridView(styleSheet);
    // Set the grid view
    grid_view->setModel(my_model);
    // Set the delegate
    grid_view->setItemDelegate(my_delegate);
    // Set the selection model
    grid_view->setSelectionModel(selection_model);

    // actions
    initSelectAll(styleSheet);

    initDleleteSelectedButton();

    initRotateSelectedButton();

    initZoomButtons();

    initViewSelector();

    QLabel * row_nb = new QLabel(QString("- %1 -").arg(my_model->rowCount()));
    row_nb->setAlignment(Qt::AlignCenter);
    row_nb->setStyleSheet(styleSheet);
    connect(my_model, &MyModel::rowAdded, this, [this, row_nb]
        (int row)
        {
            row_nb->setText(QString("- %1 -").arg(++m_rows));

//            // FIXME This is very slow
//            if(my_model->index(row, 0).isValid())
//            {
//                selection_model->select(my_model->index(row, 0), QItemSelectionModel::Select);
//            }
        }
    );

    connect(my_model, &MyModel::rowRemoved, this, [this, row_nb]{
            row_nb->setText(QString("- %1 -").arg(--m_rows));
        }
    );

//    QString scanner("/Users/liliaivanova/.Ipso/Scanner");
    QString scanner("/Users/liliaivanova/Desktop/Numbers");
    ////QString scanner("/Users/liliaivanova/Desktop");
    my_model->setDirectory(scanner);

//    if (my_model->rowCount() > 0)
//    {
//        select_all->setChecked(true);
//    }


    connect(my_model, &MyModel::dragStarted, this, [this]{
       grid_view->setSpacing(20);
    });

    connect(my_model, &MyModel::dragEnded, this, [this]{
       grid_view->setSpacing(5);
    });

    QHBoxLayout * zoom_layout = new QHBoxLayout();

    zoom_layout->addStretch();
    zoom_layout->addWidget(zoom_in);
    zoom_layout->addWidget(zoom_out);
    zoom_layout->addSpacing(5);

    QHBoxLayout * top = new QHBoxLayout();

    top->addSpacing(5);
    top->addWidget(view_selector);
    top->addWidget(select_all);
    top->addWidget(delete_selected);
    top->addWidget(rotate_selected);
    top->addStretch();
    top->addLayout(zoom_layout);
    // ...

    QFrame * frame = new QFrame;

    QVBoxLayout * frameLayout = new QVBoxLayout(frame);
    frameLayout->addLayout(top);
    frameLayout->addLayout(stackedLayout);
    frameLayout->addWidget(row_nb);

    setCentralWidget(frame);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("Scanner"));
}

MainWindow::~MainWindow()
{
    end();
}

void MainWindow::initGridView(const QString & style_sheet)
{
    grid_view->hide();
    // Display Icons
    grid_view->setViewMode(QListView::IconMode);

    // Use Grid Layout
    // grid_view->setGridSize(QSize(160, 200));

    // Items can be moved by the user only to grid positions
    grid_view->setMovement(QListView::Snap);

    grid_view->setResizeMode(QListView::Adjust);

    // Set size
    //grid_view->setMaximumWidth(1200);
    //grid_view->setMinimumWidth(1200);
    grid_view->setSpacing(5);

    // Remove horisontal scroll

    grid_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set selection mode : QAbstractItemView::ExtendedSelection, QAbstractItemView::SingleSelection, QAbstractItemView::ContiguousSelection
    grid_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Enable Drag and Drop mode : QAbstractItemView::NoDragDrop, .. QAbstractItemView::DragDrop, QAbstractItemView::InternalMove
    grid_view->setDragDropMode(QAbstractItemView::InternalMove);

    // Show Drop indicator
    grid_view->setDropIndicatorShown(true);

    // Set style
    grid_view->setStyle(new MyStyle(grid_view->style()));

    grid_view->setStyleSheet(style_sheet);

    // (Un)Set Focus

    grid_view->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::initDleleteSelectedButton()
{
    QPixmap pixmap(":images/bin.png");
    delete_selected = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(delete_selected, &Clickable::clicked, this, [this] {
        QStringList files;
        QModelIndexList indexes = selection_model->selectedIndexes();
        foreach (QModelIndex index, indexes)
        {
            files << my_model->data(index, Qt::ToolTipRole).toString();
        }

        foreach (QString file, files)
        {
            my_model->deleteFile(file);
        }

        foreach (QModelIndex index, indexes)
        {
            if(index.isValid())
            {
                selection_model->select(index, QItemSelectionModel::Deselect);
            }
        }
    });
}

void MainWindow::initViewSelector()
{
    view_selector = new QCheckBox();

    QFile styleFile(":style/view-selector.qss");
    QString styleSheet;
    if (styleFile.open(QIODevice::ReadOnly))
    {
        styleSheet = styleFile.readAll();
        // add log here
    }

    view_selector->setStyleSheet(styleSheet);
    view_selector->setChecked(true);

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(grid_view);
    stackedLayout->addWidget(single_file_view);
    stackedLayout->setCurrentIndex(0);

    connect(view_selector, &QCheckBox::stateChanged, this, [this] {
        if(view_selector->checkState() == Qt::Checked)
        {
            stackedLayout->setCurrentIndex(0);
            grid_view->setFocus();
        }
        else
        {
            stackedLayout->setCurrentIndex(1);
            single_file_view->setFocusOnThumbnailList();
        }
    });
}

void MainWindow::initSelectAll(const QString & style_sheet)
{
    select_all = new QCheckBox();
    select_all->setStyleSheet(style_sheet);

    connect(select_all, &QCheckBox::stateChanged, this, [this] {

        QItemSelectionModel::SelectionFlag flag;

        if(select_all->checkState() == Qt::Checked)
        {
            flag = QItemSelectionModel::Select;
        }
        else
        {
            flag = QItemSelectionModel::Deselect;
        }

        int rows = my_model->rowCount();

        QModelIndex topLeft = my_model->index(0,0);
        QModelIndex bottomRight = my_model->index(rows-1,0);
        QItemSelection selection(topLeft, bottomRight);
        selection_model->select(selection, flag);
    });
}

void MainWindow::initRotateSelectedButton()
{
    QPixmap pixmap(":images/rotate.png");
    rotate_selected = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    connect(rotate_selected, &Clickable::clicked, this, [this] {
        QModelIndexList indexes = selection_model->selectedIndexes();
        foreach (QModelIndex index, indexes)
        {
            my_model->rotate(index);
        }
    });
}

void MainWindow::initZoomButtons()
{
    QPixmap pixmap(":images/plus.png");
    zoom_in = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_in, &Clickable::clicked, single_file_view->documentViewer(), &DocumentViewer::zoomIn);

    pixmap.load(":images/minus.png");
    zoom_out = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_out, &Clickable::clicked, single_file_view->documentViewer(), &DocumentViewer::zoomOut);
}
