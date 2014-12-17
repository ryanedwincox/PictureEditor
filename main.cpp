#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );

    MainWindow* mainwindow = new MainWindow();
    mainwindow->setWindowTitle("Picture Editor");
    //mainwindow->setWindowIcon(QIcon(QPixmap("icon.png")));
    mainwindow->show();

    app.exec();
}
