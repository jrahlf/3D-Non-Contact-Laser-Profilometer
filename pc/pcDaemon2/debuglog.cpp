#include "debuglog.h"
#include "ui_debuglog.h"
#include <QTextCursor>

debugLog::debugLog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::debugLog)
{
    ui->setupUi(this);
}

debugLog::~debugLog(){
    delete ui;
}

void debugLog::append(QString s){
    ui->log->append(s);
    QTextCursor c =  ui->log->textCursor();
    c.movePosition(QTextCursor::End);
    ui->log->setTextCursor(c);
}
