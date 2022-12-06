#ifndef CALIEXTRAAXISWIDGET_H
#define CALIEXTRAAXISWIDGET_H

#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <string>

#include "Util/jsonParser/jsonparser.hpp"

namespace Ui {
class caliExtraAxisWidget;
}

class RobotOperator;
class MCOperator;

// TODO exception handler
class caliExtraAxisWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliExtraAxisWidget(const QString& prefix,
                               QWidget* parent = nullptr);
  ~caliExtraAxisWidget();
  void setDevice(RobotOperator* robot, MCOperator* _motionController);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readData();
  void writeData();
  int readDeviceData(std::array<float, 4>& data);
  void readResult();
  void clearResult();
  void writeResult();

  void recordData(const std::array<float, 4>& data);
  void deleteLastItem();
  void updateTreeView();
  void calculate();
  void dumpJson();

 signals:
  void updateTreeView(const QByteArray&);

 private slots:
  void on_btn_record_clicked();

  void on_btn_calculate_clicked();

  void on_btn_delete_clicked();

 private:
  Ui::caliExtraAxisWidget* ui;
  // device
  RobotOperator* _robot;
  MCOperator* _motionController;
  // file name
  QString _dataFilePath;
  QString _resFilePath;
  // QJsonDoc
  QString _jsonPrefix;
  QString _dataMainKey;
  QString _resultMainKey;
  std::unique_ptr<jsonParser> _dataDoc;
  std::unique_ptr<jsonParser> _resultDoc;
};

#endif  // CALIEXTRAAXISWIDGET_H
