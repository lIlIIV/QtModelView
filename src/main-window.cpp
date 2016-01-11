#include "main-window.hh"

#include <QDataWidgetMapper>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

#include "my-view.hh"
#include "my-style.hh"
#include "widgets/clickable.hh"
#include "widgets/thumbnail-editor.hh"

namespace {
    const int A4_W = 400; //(A4_H*21)/29;
    const int A4_H = (29*A4_W)/21;
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
    list_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Enable Drag and Drop mode : QAbstractItemView::NoDragDrop, .. QAbstractItemView::DragDrop, QAbstractItemView::InternalMove
    list_view->setDragDropMode(QAbstractItemView::InternalMove);

    // Show Drop indicator
    list_view->setDropIndicatorShown(true);

    list_view->setStyle(new MyStyle(list_view->style()));

    QFile styleFile(":style/style.qss");
    if (styleFile.open(QIODevice::ReadOnly))
    {
        QString StyleSheet(styleFile.readAll());
        list_view->setStyleSheet(StyleSheet);
    }
    // ..

    // Set the model

    list_view->setModel(my_model);

    // Set the delegate

    list_view->setItemDelegate(my_delegate);

    list_view->setEditTriggers(QAbstractItemView::DoubleClicked);

    // Set the Single Item View

    MyView * my_view = new MyView();

    my_view->setModel(my_model);
    my_view->setSelectionModel(list_view->selectionModel());
    my_view->setMinimumSize(QSize(A4_W, A4_H));

    //my_view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    QCheckBox * selectAll = new QCheckBox("Select all");
    connect(selectAll, &QCheckBox::stateChanged, my_model, &MyModel::selectAll);

    QPixmap pixmap(":images/unstarted.png");
    Clickable * deleteAll = new Clickable(pixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(deleteAll, &Clickable::clicked, my_model, &MyModel::removeSelected);

    QHBoxLayout * topLeft = new QHBoxLayout();

    topLeft->addWidget(selectAll);
    topLeft->addWidget(deleteAll);

    topLeft->addSpacing(5);
    topLeft->addStretch();

    QVBoxLayout * left = new QVBoxLayout();
    left->addLayout(topLeft);
    left->addWidget(list_view);


    QFrame * frame = new QFrame;



    QHBoxLayout * frameLayout = new QHBoxLayout(frame);
    frameLayout->addLayout(left);
    frameLayout->addWidget(my_view);
    frameLayout->addStretch();

    setCentralWidget(frame);

    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setWindowTitle(tr("My Models"));
}

MainWindow::~MainWindow()
{

}
