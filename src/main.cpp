#include <iostream>

#include <QApplication>
#include <QIcon>
#include <QImage>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QStringList>

#include "main-window.hh"



int main(int args, char * argv[])
{
	QApplication app(args, argv);
    app.setApplicationName("Model View test");
	
    MainWindow mainW;
    mainW.show();

    QFile styleFile(":style/style.qss");
    if (!styleFile.open(QIODevice::ReadOnly))
    {
        printf("Unable to load styles file\n");
        return -1;
    }

    QString StyleSheet(styleFile.readAll());
    app.setStyleSheet(StyleSheet);
    styleFile.close();
 
	return app.exec();
}


// QImage img(":images/completed.png");
// img = img.scaled(30,30);
// QIcon ico(QPixmap::fromImage(img));
// QPushButton *button = new QPushButton;
// button->setIcon(ico);
// button->show();
