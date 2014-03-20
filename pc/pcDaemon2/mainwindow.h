#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myThread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void registerButtonHandler(myThread* receiver);


public slots:
    void setPositionX(double x);
    void setPositionY(double y);
    void setMeasurement(double m);
    
private:

};

#endif // MAINWINDOW_H
