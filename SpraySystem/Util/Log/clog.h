#ifndef CLOG_H
#define CLOG_H

#include <QObject>
#include <QDir>
#include <mutex>

class CLog : public QObject
{
    Q_OBJECT
public:
   static CLog* getInstance();

    enum CLOG_LEVEL{
        RINFO,          /*!<  提示*/
        RWARNING,       /*!<  警告*/
        REEROR,          /*!<  错误*/
        DEBUG           /*!<  调试*/
    };
    static bool createDir(QString dirPath);

    static void log(QString data, CLog::CLOG_LEVEL nLevel = CLOG_LEVEL::DEBUG);
private:
    static std::mutex _mutex;
    static CLog *m_CLog;
    CLog();
    static QString getLevelDesc(CLOG_LEVEL level);
    void clear();
signals:

};

#endif // CLOG_H
