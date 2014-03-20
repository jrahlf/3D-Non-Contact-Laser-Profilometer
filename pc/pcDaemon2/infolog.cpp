#include "infolog.h"
#include "ui_infolog.h"

InfoLog::InfoLog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InfoLog)
{
    ui->setupUi(this);
}

InfoLog::~InfoLog()
{
    delete ui;
}

void InfoLog::append(QString s){
    ui->log->append(s);
    QTextCursor c =  ui->log->textCursor();
    c.movePosition(QTextCursor::End);
    ui->log->setTextCursor(c);
}
