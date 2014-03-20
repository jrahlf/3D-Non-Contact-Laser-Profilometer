#include "errorlog.h"
#include "ui_errorlog.h"

ErrorLog::ErrorLog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ErrorLog)
{
    ui->setupUi(this);
}

ErrorLog::~ErrorLog()
{
    delete ui;
}

void ErrorLog::append(QString s){
    ui->log->append(s);
    QTextCursor c =  ui->log->textCursor();
    c.movePosition(QTextCursor::End);
    ui->log->setTextCursor(c);
}
