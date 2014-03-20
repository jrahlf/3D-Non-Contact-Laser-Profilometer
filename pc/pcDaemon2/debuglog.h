#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include <QMainWindow>

namespace Ui {
class debugLog;
}

class debugLog : public QMainWindow
{
    Q_OBJECT
    
public:
    Ui::debugLog *ui;
    explicit debugLog(QWidget *parent = 0);
    ~debugLog();

public slots:
    void append(QString);
    
private:

};

#endif // DEBUGLOG_H
