#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../microcontroller/transform.h"
#include "myThread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdMeasurement->setDigitCount(7);
    ui->lcdX2->setDigitCount(7);
    ui->lcdY2->setDigitCount(7);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::registerButtonHandler(myThread* receiver){
    QObject::connect(ui->updateButton, SIGNAL(clicked()), receiver, SLOT(handleUpdateButton()));
}

void MainWindow::setPositionX(double mm){
    //ui->lcdX->display(x);
    //float mm = Transform::encoderToMM(x);
    ui->lcdX2->display(mm);
}

void MainWindow::setPositionY(double mm){
    //ui->lcdY->display(y);
    //float mm = Transform::encoderToMM(y);
    ui->lcdY2->display(mm);
}

void MainWindow::setMeasurement(double m){
    ui->lcdMeasurement->display(m);
}
