#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QCheckBox>
#include <QDockWidget>
#include <QListView>
#include <QMainWindow>
#include <QStackedLayout>

#include "my-delegate.hh"
#include "my-model.hh"
#include "my-view.hh"
#include "widgets/clickable.hh"
#include "widgets/single-file-view.hh"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void initSingleFileView(const QString & style_sheet);
    void initGridView(const QString & style_sheet);
    void initSelectAll(const QString & style_sheet);
    void initDleleteSelectedButton();
    void initRotateSelectedButton();
    void initZoomButtons();
    void initViewSelector();

    QListView * grid_view;
    SingleFileView * single_file_view;

    MyModel * my_model;
    MyDelegate * my_delegate;
    QItemSelectionModel * selection_model;

    int m_rows;

    Clickable * delete_selected;
    Clickable * rotate_selected;
    QCheckBox * select_all;
    QCheckBox * view_selector;
    Clickable * zoom_in;
    Clickable * zoom_out;

    QStackedLayout * stackedLayout;

};

#endif // MAIN_WINDOW
