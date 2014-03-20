#ifndef INFOLOG_H
#define INFOLOG_H

#include <QMainWindow>

namespace Ui {
class InfoLog;
}

class InfoLog : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit InfoLog(QWidget *parent = 0);
    ~InfoLog();

public slots:
    void append(QString);
    
private:
    Ui::InfoLog *ui;
};

#endif // INFOLOG_H
