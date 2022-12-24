#ifndef MCOPERATOR_H
#define MCOPERATOR_H

#include <QHostAddress>
#include <QTcpSocket>
#include <memory>

#include "DataAccess/Models/MotionController/motioncontroller.h"
#include "Device/MotionController/mcdatapaser.h"
#include "Device/deviceoperator.h"
#include "Util/Socket/qtsocketclient.h"
#include <QSemaphore>
#include <QTimer>
#include <QVariant>
#include <mutex>

using MCData = vws::MCData;

Q_DECLARE_METATYPE(MCData) // 注册结构体
class MCOperator : public QObject, public DeviceOperator {
  Q_OBJECT
public:
  MCOperator(std::shared_ptr<MotionController> mc);
  virtual ~MCOperator();
  int init();
  int start();
  void close();
  int getState();
  /** @brief 开始接受运动控制器信号*/
  void startReceive();

  /** @brief 运动控制器请报错复位*/
  void reset();

  /** @brief 向机器人发送运动轨迹*/
  void sendTrajParam(float zeropoint, float offset, uint8_t axisNum);
  struct TrajParamData {
    float zeropoint;
    float offset;
    float dpos_start;
    float dpos_mid;
    float dpos_stop;
    uint8_t axisNum;
  };
  void sendTrajParam(TrajParamData);

  /** @brief 反馈轨迹发送结果
   * @param success: 1成功，-1失败
   */
  void sendRbtResult(float success);
  std::vector<float> getChainEncoders(bool &success);
  std::vector<float> getRealTimeEncoder(bool &success);
  void SevenAxisReset();

private:
  MCData data_master;
  MCData data_slave;
  QtSocketClient *master_socket;
  QtSocketClient *slave_socket;
  mcdatapaser *dataparser_master;
  mcdatapaser *dataparser_slave;
  QTimer *timer;
  int preheart;
  int failcount = 0;
  bool state;
  int count; // 0-10000;

  bool stop = false;
  std::mutex send_mutex;

  bool trySendData(uint8_t order, float v1, float v2, bool &flag,
                   uint8_t axisNum = 0);
  void sendData(uint8_t order, float v1, float v2, uint8_t aixsNUm = 0);
  bool trySendData(uint8_t order, std::vector<float>, bool &flag,
                   uint8_t axisNum);
  void sendData(uint8_t order, std::vector<float>, uint8_t aixsNUm);

  bool waitData(bool &flag);

public slots:
  void readyRead_Slot(QByteArray buf);
  void checkState_Slot();
signals:
  void getTrajParam_Signal(quint8 axisNum);
  void sendToRBT_Signal(quint8 axisNum);
  void mcWarning_Signal(quint8 axisNum, std::vector<int32_t> errData);
  void connect_master_Signal(QString ip, int port);
  void connect_slave_Signal(QString ip, int port);
};

#endif // MCOPERATOR_H
