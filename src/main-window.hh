#ifndef MAIN_WINDOW
#define MAIN_WINDOW

#include <QDockWidget>
#include <QListView>
#include <QMainWindow>
#include <QVBoxLayout>

#include "my-delegate.hh"
#include "my-model.hh"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    QListView * list_view;
    MyModel * my_model;
    MyDelegate * my_delegate;

};

#endif // MAIN_WINDOW
