#include "clog.h"

#include <QMutex>
#include <QApplication>
#include <QDate>
#include <QTextStream>
#include <iostream>
#include <QDebug>

const char PATH_LogPath[] = "/Logs/";
const char Suffix[] = ".log";
const int ExpirDay = 10;
QMutex m_mutex;

QString logDir;

CLog *CLog::m_CLog = NULL;

CLog *CLog::getInstance()
{
    if(m_CLog==NULL)
    {
        m_mutex.lock();
        if(m_CLog==NULL)
        {
            m_CLog = new CLog();
        }
        m_mutex.unlock();
    }

     return m_CLog;
}


bool CLog::createDir(QString dirPath)
{
    QFileInfo fileInfo(dirPath);
    if(!fileInfo.exists()){
        QDir tmpDir;
        return tmpDir.mkpath(dirPath);
    }

    return true;
}


void CLog::log(CLog::CLOG_LEVEL nLevel, QString data)
{
    m_mutex.lock();

    QString subDir;
    switch (nLevel)
    {
        case CLOG_LEVEL::RINFO:
            subDir = "Info/";
            break;
        case CLOG_LEVEL::REEROR:
            subDir = "Error/";
            break;
        case CLOG_LEVEL::RWARNING:
            subDir = "Error/";
            break;
    }

    QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString current_date_time = QDateTime::currentDateTime().toString("hh:mm:ss ddd");
    QString message = QString("[%1]， %2").arg(current_date_time).arg(data);

    QString fileName = QString("%1%2").arg(current_date).arg(Suffix);
    QString  logDir = qApp->applicationDirPath()+QString(PATH_LogPath) + subDir;

    if(createDir(logDir)){
        QString fullPath = logDir+fileName;
        QFile logfile(fullPath);
        //logfile.setFileName(fileName);
         if(logfile.open(QFile::WriteOnly|QFile::Append|QFile::Text))
         {
            QTextStream txt_stream(&logfile);
            txt_stream<<message<<"\r\n";
            logfile.flush();
            logfile.close();
         }
    }
    m_mutex.unlock();
}

CLog::CLog()
{
    logDir = qApp->applicationDirPath()+QString(PATH_LogPath);

    this->clear();
}

void CLog::clear()
{
    QDateTime now= QDateTime::currentDateTime();
    QDir tmpDir(logDir);
    QFileInfoList dirlist = tmpDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (QFileInfo dirItem, dirlist)
    {
        tmpDir.setPath(dirItem.absoluteFilePath());
        QFileInfoList fileList = tmpDir.entryInfoList(QDir::Files);
        foreach(QFileInfo fileItem , fileList)
        {
            QDateTime dt = fileItem.lastModified();
            if(dt.addDays(ExpirDay)<now)
            {
                fileItem.dir().remove(fileItem.fileName());
            }
        }
    }
}

