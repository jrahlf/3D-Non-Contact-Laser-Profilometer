#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QtCore>

class myThread : public QThread{
    Q_OBJECT
signals:
    void appendDLog(QString);
    void appendELog(QString);
    void appendILog(QString);
    void setX(double);
    void setY(double);
    void setZ(double);

private slots:
    void handleUpdateButton();

public:
    void run();
};



#endif // MYTHREAD_H
