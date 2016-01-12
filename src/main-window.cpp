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
#include "widgets/clickable.hh"
#include "widgets/document-viewer.hh"

namespace {
    const int A4_W = 730; //400; //(A4_H*21)/29;
    const int A4_H = 320; //(29*A4_W)/21;
    const QString default_image = ":images/unstarted.png";
}

MainWindow::MainWindow()
    : list_view(new QListView(this)),
      my_model(new MyModel(this)),
      my_delegate(new MyDelegate(this))
{
    resize(600, 600);


    // Init list view

    // Display Icons
    list_view->setViewMode(QListView::IconMode);

    // Use Grid Layout
    //list_view->setGridSize(QSize(160,200));

    // Items can be moved by the user only to grid positions
    list_view->setMovement(QListView::Snap);

    // Set size
    list_view->setMaximumWidth(170);
    list_view->setMinimumWidth(170);
    list_view->setSpacing(10);

    // Remove horisontal scroll

    list_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set selection mode : QAbstractItemView::ExtendedSelection, QAbstractItemView::SingleSelection, QAbstractItemView::ContiguousSelection
    list_view->setSelectionMode(QAbstractItemView::SingleSelection);

    // Enable Drag and Drop mode : QAbstractItemView::NoDragDrop, .. QAbstractItemView::DragDrop, QAbstractItemView::InternalMove
    list_view->setDragDropMode(QAbstractItemView::InternalMove);

    // Show Drop indicator
    list_view->setDropIndicatorShown(true);

    list_view->setStyle(new MyStyle(list_view->style()));

    QFile styleFile(":style/style.qss");
    QString styleSheet;
    if (styleFile.open(QIODevice::ReadOnly))
    {
        styleSheet = styleFile.readAll();
        // add log here
    }
    list_view->setStyleSheet(styleSheet);

    // ..

    // Set the model

    list_view->setModel(my_model);

    // Set the delegate

    list_view->setItemDelegate(my_delegate);

    list_view->setEditTriggers(QAbstractItemView::DoubleClicked);



    // ..

    // Selection Model

    QItemSelectionModel * selection_model = list_view->selectionModel();
    selection_model->setModel(my_model);
    if(my_model->index(0,0).isValid())
    {
        selection_model->setCurrentIndex(my_model->index(0,0), QItemSelectionModel::Select);
    }

    connect(my_model, &MyModel::select, this, [this, selection_model]
        (int row)
        {
            if(my_model->index(row,0).isValid())
            {
                selection_model->clearSelection();
                selection_model->setCurrentIndex(my_model->index(row,0), QItemSelectionModel::Select);
            }
        }
    );

//    connect(selection_model, &QItemSelectionModel::selectionChanged, this, [this, selection_model]
//        (const QItemSelection & selected, const QItemSelection & deselected)
//        {
//            if(selected.isEmpty())
//            {
//                if(my_model->index(0, 0).isValid())
//                {
//                    selection_model->setCurrentIndex(my_model->index(0,0), QItemSelectionModel::Select);
//                }
//            }
//        }
//    );

    // Set the Single Item View

    MyView * my_view = new MyView();

    my_view->setModel(my_model);
    my_view->setSelectionModel(selection_model);
    my_view->setMinimumSize(QSize(A4_W, A4_H));

    // ..

    // Select all and delete

    QCheckBox * selectAll = new QCheckBox();
    connect(selectAll, &QCheckBox::stateChanged, my_model, &MyModel::selectAll);

//    connect(my_model, )


    QPixmap pixmap(":images/bin.png");
    Clickable * deleteAll = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(deleteAll, &Clickable::clicked, my_model, &MyModel::removeSelected);

    pixmap.load(":images/rotate.png");
    Clickable * rotate = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(rotate, &Clickable::clicked, my_model, &MyModel::rotateChecked);

    QHBoxLayout * topLeft = new QHBoxLayout();

    topLeft->addWidget(selectAll);
    topLeft->addWidget(deleteAll);
    topLeft->addWidget(rotate);

    topLeft->addSpacing(5);
    topLeft->addStretch();

    // ... ZOOM

    pixmap.load(":images/plus.png");
    Clickable * zoom_in = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_in, &Clickable::clicked, my_view->documentViewer(), &DocumentViewer::zoomIn);

    pixmap.load(":images/minus.png");
    Clickable * zoom_out = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_out, &Clickable::clicked, my_view->documentViewer(), &DocumentViewer::zoomOut);

    QHBoxLayout * zoom_layout = new QHBoxLayout();

    zoom_layout->addWidget(zoom_in);
    zoom_layout->addWidget(zoom_out);
    zoom_layout->addSpacing(5);
    zoom_layout->addStretch();


   // ...

    QVBoxLayout * left = new QVBoxLayout();
 //   left->addLayout(topLeft);
    left->addWidget(list_view);

    QVBoxLayout * right = new QVBoxLayout();
//    right->addLayout(zoom_layout);
    right->addWidget(my_view);


    QLabel * row_nb = new QLabel(QString("- %1 -").arg(my_model->rowCount()));
    row_nb->setAlignment(Qt::AlignCenter);
    row_nb->setStyleSheet(styleSheet);
    connect(my_model, &MyModel::rowNbChanged, this, [this, row_nb, my_view]
        (int rows)
        {
            row_nb->setText(QString("- %1 -").arg(rows));
        }
    );


    QFrame * frame = new QFrame;

//    QHBoxLayout * frameLayout = new QHBoxLayout(frame);
//    frameLayout->addLayout(left);
//    frameLayout->addLayout(right);
//    frameLayout->addStretch();

    QGridLayout * frameLayout = new QGridLayout(frame);
    frameLayout->addLayout(topLeft, 0, 0);
    frameLayout->addLayout(left, 1, 0);
    frameLayout->addLayout(zoom_layout, 0, 1);
    frameLayout->addWidget(my_view, 1, 1);
    frameLayout->addWidget(row_nb, 2, 0);

    setCentralWidget(frame);

    list_view->setFocusPolicy(Qt::NoFocus);
    list_view->setAutoScroll(true);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("My Models"));
}

MainWindow::~MainWindow()
{
    end();
}
