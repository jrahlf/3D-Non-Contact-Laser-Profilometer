#include <QApplication>
#include <QThread>
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <iostream>
#include <thread>
#include <fstream>
#include "mainwindow.h"
#include "myThread.h"
#include "debuglog.h"
#include "errorlog.h"
#include "infolog.h"
#include <unistd.h>
#include <signal.h>
#include "../../microcontroller/transform.h"
#include "../../microcontroller/pcInterface.h"
#include "../../microcontroller/checksum.h"
#include "slantCorrection.h"
#include <engine.h>
#include <QCommandLineParser>
#include <sstream>
#include <fcntl.h>
#include <termios.h>
#include <mutex>

using namespace std;

QApplication* APP;
MainWindow* mainWindow;
debugLog* dLog;
InfoLog* iLog;
ErrorLog* eLog;

Engine* engine = 0;
std::mutex matlabMutex;
std::string matlabPlotCommand;

QFile* dataFile;
QFile* rawDataFile;
QTextStream* dataFileStream;
QTextStream* rawDataFileStream;


double ySlantCoeff;
std::string serialPath;
bool doLogData = true;
bool corrected = false;
QString targetDir;
int failedChecksumCounter = 0;
int malformedLinesCounter = 0;



void closeLogFiles();
void correctDataFile();

void sigHandler(int signal){
    closeLogFiles();
    engClose(engine);
    APP->exit();
    exit(0);
}

bool getData(string line, double& x, double& y, double& m){
    std::istringstream i(line);
    return (i >> x) && (i >> y) && (i >> m);
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    APP = &a;

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption noLogOption("no-log", "disables logging");
    QCommandLineOption noGuiOption("no-gui", "Suppresses the gui");
    QCommandLineOption noMatlabOption("no-matlab", "does not launch MATLAB at startup");
    QCommandLineOption dirOption("target-directory", "specifies the output directory", "directory");
    QCommandLineOption serialOption("serial", "specifies the device or file to read from", "serial");
    QCommandLineOption noCorrectOption("no-correction", "disables slant correction");
    QCommandLineOption slantCorrectOptionY("slantnessY", "correct measurements along X axis using specified linear coefficient [micrometer/mm]",
                                           "slantY", "9.1");
    QCommandLineOption onlyCorrectFileOption("only-correct", "correct the specified data file by the specified slant coefficients");

    parser.addOption(noLogOption);
    parser.addOption(dirOption);
    parser.addOption(serialOption);
    parser.addOption(noGuiOption);
    parser.addOption(noMatlabOption);
    parser.addOption(slantCorrectOptionY);
    parser.addOption(noCorrectOption);
    parser.addOption(onlyCorrectFileOption);


    debugLog d;
    //d.show();
    InfoLog i;
    ErrorLog e;
    //e.show();
    MainWindow w;

    mainWindow = &w;
    dLog = &d;
    iLog = &i;
    eLog = &e;

    myThread* thread = new myThread;

    
    /*QObject::connect(thread, SIGNAL(appendDLog(QString)), dLog, SLOT(append(QString)));
    QObject::connect(thread, SIGNAL(appendELog(QString)), eLog, SLOT(append(QString)));*/


    signal(SIGTERM, sigHandler);
    signal(SIGINT, sigHandler);

    parser.process(a);

    doLogData = !parser.isSet(noLogOption);

    if(parser.isSet(serialOption)){
        serialPath = parser.value(serialOption).toStdString();
    }else{
        serialPath = "/dev/ttyUSB0";
        system("/home/jonas/warwick/3dProfilometer/setupUart.sh");
    }

    if(parser.isSet(dirOption)){
        targetDir = parser.value(dirOption);
    }

    //if(parser.isSet(slantCorrectOptionY)){
        bool ok = true;
        ySlantCoeff = parser.value(slantCorrectOptionY).toDouble(&ok);
        if(!ok){
            cerr << "slant value is bad" << endl;
            exit(1);
        }
    //}

    corrected = true;
    if(parser.isSet(noCorrectOption)){
        corrected = false;
        cout << "not correcting measurements" << endl;
    }else{
        cout << "using " << ySlantCoeff << " as Y axis slant correction factor" << endl;
    }

    if(parser.isSet(onlyCorrectFileOption)){
        if(!corrected){
            cerr << "no slant correction file specified!" << endl;
            exit(1);
        }
        correctDataFile();
        exit(0);
    }

    thread->start();
    if(parser.isSet(noGuiOption)){
        //thread->wait();
        return a.exec();
    }

    if(doLogData && !parser.isSet(noMatlabOption)){
        engine = engOpen(0);
    }
    if(!engine){
        cerr << "Not opening MATLAB engine" << endl;
    }else{
        engEvalString(engine, "cd /home/jonas/3dLaserProfilometer/matlab");
    }

    w.registerButtonHandler(thread);
    QObject::connect(thread, SIGNAL(setX(double)), mainWindow, SLOT(setPositionX(double)));
    QObject::connect(thread, SIGNAL(setY(double)), mainWindow, SLOT(setPositionY(double)));
    QObject::connect(thread, SIGNAL(setZ(double)), mainWindow, SLOT(setMeasurement(double)));
    QObject::connect(thread, SIGNAL(appendILog(QString)), iLog, SLOT(append(QString)));

    i.show();
    w.show();
    return a.exec();
}

void closeLogFiles(){
    cout << "flushing buffers" << endl;
    dataFileStream->flush();
    rawDataFileStream->flush();
    dataFile->flush();
    dataFile->close();
    QTextStream stream(rawDataFile);
    if(doLogData && failedChecksumCounter+malformedLinesCounter > 0){
        stream << "Number of checksum errors: " << failedChecksumCounter << endl;
        stream << "Number of malformed lines: " << malformedLinesCounter << endl;
    }
    rawDataFile->flush();
    rawDataFile->close();

    if(dataFile->size() == 0){
        dataFile->remove();
    }
    if(rawDataFile->size() == 0){
        rawDataFile->remove();
    }
}

void openLogFiles(){
    QString dirName;
    if(targetDir.isEmpty()){
        dirName = "/home/jonas/warwick/3dProfilometer/dataLog/";
        dirName +=  QDate::currentDate().toString("dd_MM_yyyy");
    }else{
        dirName = targetDir;
    }
    QDir dir;
    if(doLogData){
        dir.mkpath(dirName);
    }
    QString fileName = QTime::currentTime().toString();
    if(corrected){
        fileName += "-corrected";
    }
    fileName += ".txt";

    dataFile = new QFile(dirName+"/"+fileName);
    if(doLogData){
        dataFile->open(QIODevice::WriteOnly | QIODevice::Text);
    }
    matlabPlotCommand = string("clf; plotDataFileFast ")+dataFile->fileName().toStdString();

    dirName += "/raw";
    if(doLogData){
        dir.mkpath(dirName);
    }

    fileName = QTime::currentTime().toString();
    fileName += ".txt";

    rawDataFile = new QFile(dirName+"/"+fileName);
    if(doLogData){
        rawDataFile->open(QIODevice::WriteOnly | QIODevice::Text);
    }
    malformedLinesCounter = 0;
    failedChecksumCounter = 0;
}

void execMatlab(string what){
    if(engine == 0){
        return;
    }
    if(matlabMutex.try_lock()){
        engEvalString(engine, what.c_str());
        matlabMutex.unlock();
    }
}

void execMatlabAsync(string what){
    std::thread matlabThread(execMatlab, what);
    matlabThread.detach();
}

void myThread::handleUpdateButton(){
    string s = "echo \"GETM\" > ";
    s += serialPath;
    s += "\n";
    system(s.c_str());

    execMatlabAsync(matlabPlotCommand);
}

void correctDataFile(){
    openLogFiles();
    QTextStream dataLogStream(dataFile);
    QTextStream rawDataStream(rawDataFile);
    dataFileStream = &dataLogStream;
    rawDataFileStream = &rawDataStream;

    int fd = open(serialPath.c_str(), 0);
    sleep(2); //required to make flush work, for some reason
    tcflush(fd,TCIOFLUSH);
    close(fd);

    ifstream file;
    file.open(serialPath, ios::in);
    if(!file){
        cerr << "cannot open serial" << endl;
        sigHandler(1);
    }

    std::string line;
    double x,y,m;
    int i = 0;
    while(file){
        if(i++ == 20){
            //break;
        }
        std::getline(file, line);
        getData(line, x, y, m);
        double corr = y*ySlantCoeff - 418;
        m -= corr;
        if(m > 700 || m < -700){
            break;
        }
        dataLogStream << x << " " << y << " " << m << endl;
    }

    closeLogFiles();

}


static const int BUF_SIZE = 512;
static char REAL_BUF[BUF_SIZE];
static char* const bufStart = REAL_BUF;
static char* buffer = bufStart;

void myThread::run(){
    int fd = open(serialPath.c_str(), 0);
    sleep(2); //required to make flush work, for some reason
    tcflush(fd,TCIOFLUSH);
    close(fd);

    ifstream serial;
    serial.open(serialPath, ios::in);
    if(!serial){
        cerr << "cannot open serial" << endl;
        sigHandler(1);
    }
    openLogFiles();
    QTextStream dataLogStream(dataFile);
    QTextStream rawDataStream(rawDataFile);
    dataFileStream = &dataLogStream;
    rawDataFileStream = &rawDataStream;

    if(corrected){
        rawDataStream << "using slant correction" << endl;
    }

    while(true){
        //QElapsedTimer timer;
        //qint64 nanoSec;
        //timer.start();


        if(serial.eof()){
            //serial.clear();
            sigHandler(0);
        }

        buffer = bufStart;
        memset(buffer, 0, BUF_SIZE);
        serial.getline(buffer, BUF_SIZE, '\n');

        char type = buffer[0];//serial.get(); //first char, type
        /*if(type == '\n'){
            continue;
        }*/
        char shallChecksum = buffer[1];//serial.get(); //second char, either checksum or comma
        //if this is a comma -> check is checksum
        //if not -> this is the first character
        char comma = buffer[2];//serial.get();
        int useChecksum = comma == ',' ? 1 : 0;

        if(useChecksum){
            buffer = buffer+3;
            char isChecksum = Checksum::getFor(buffer, strlen(buffer), type);
            if(isChecksum != shallChecksum){
                failedChecksumCounter++;
                //cout << type << ": failed checksum: should be " << shallChecksum << " is " << isChecksum << " length " << strlen(buffer) << endl;
                cerr << "checksum failed for " << (buffer-3) << endl;
                if(type == 'P' || type == 'M' || type == 'K'){
                    continue;
                }
            }
            //cout << type << ": checksum ok for " << buffer << endl;
        }else{
            buffer = buffer+2;
           //serial.getline(buffer+1, BUF_SIZE, '\n');
           //buffer[0] = comma;
        }

        QString qline(buffer);
        QStringList list = qline.split(",", QString::SkipEmptyParts);
        QString log;
        int x,y;
        double xd, yd;
        double m;
        bool ok;
        switch(type){

        case Prefix::COMMAND:
            if(qline == QString(PC::ucCommand::FLUSH_DATA)){
                closeLogFiles();
                openLogFiles();
                dataLogStream.setDevice(dataFile);
                rawDataStream.setDevice(rawDataFile);
            }
            break;

        case Prefix::DEBUG:
            log = "DEBUG - " + qline;
            emit(appendILog(log));
            rawDataStream << qline << endl;
            break;

        case Prefix::ERROR:
            log = "ERROR - " + qline;
            emit(appendILog(log));
            rawDataStream << qline << endl;
            break;

        case Prefix::INFO:
            log = /*"INFO - " +*/ qline;
            emit(appendILog(log));
            rawDataStream << qline << endl;
            break;

        case Prefix::POSITION:
            if(list.length() != 2){
                malformedLinesCounter++;
                continue;
            }
            x = list.at(0).toInt(&ok);
            if(!ok){
                continue;
            }
            y = list.at(1).toInt(&ok);
            if(!ok){
                break;
            }
            xd = Transform::encoderToMM(x);
            yd = Transform::encoderToMM(y);
            emit(setX(xd));
            emit(setY(yd));
            break;

        case Prefix::MEASUREMENT:
            if(list.length() != 3){
                malformedLinesCounter++;
                continue;
            }
            x = list.at(0).toInt(&ok);
            if(!ok){
                break;
            }
            y = list.at(1).toInt(&ok);
            if(!ok){
                break;
            }
            m = list.at(2).toDouble(&ok);
            if(!ok){
                break;
            }
            xd = Transform::encoderToMM(x);
            yd = Transform::encoderToMM(y);
            if(corrected){
                double corr = yd*ySlantCoeff - 400;
                m -= corr;
            }
            emit(setX(xd));
            emit(setY(yd));
            emit(setZ(m));
            if(doLogData){
                //cout << xd << " " << yd << " " << m << endl;
                dataLogStream << xd << " " << yd << " " << m << endl;
            }
            break;

        case Prefix::CONTROL:
            //cout << qline.toStdString() << endl;
            break;

        default:
            if(doLogData){
                rawDataStream << qline << endl;
                cout << "got " << qline.toStdString() << " type " << type
                        << " comma " << comma
                           << " chck " << shallChecksum << " useCheck " << useChecksum << endl;
            }
            break;

        }

        //nanoSec = timer.nsecsElapsed();
        //cout << "needed " << nanoSec << "nanoseconds = " << nanoSec/1000 << " usecs" << endl;
    }
}
