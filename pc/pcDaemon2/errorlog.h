#ifndef ERRORLOG_H
#define ERRORLOG_H

#include <QMainWindow>

namespace Ui {
class ErrorLog;
}

class ErrorLog : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ErrorLog(QWidget *parent = 0);
    ~ErrorLog();

public slots:
    void append(QString);
    
private:
    Ui::ErrorLog *ui;
};

#endif // ERRORLOG_H
