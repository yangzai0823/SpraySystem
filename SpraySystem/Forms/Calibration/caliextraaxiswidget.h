#ifndef CALIEXTRAAXISWIDGET_H
#define CALIEXTRAAXISWIDGET_H

#include <rapidjson/document.h>

#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <string>

namespace Ui {
class caliExtraAxisWidget;
}
// namespace rapidjson {
// class Document;
// }

class RobotOperator;
class MCOperator;
class QJsonDocument;

// TODO exception handler
class caliExtraAxisWidget : public QWidget {
  Q_OBJECT

 public:
  explicit caliExtraAxisWidget(QWidget* parent = nullptr);
  ~caliExtraAxisWidget();
  void setDevice(RobotOperator* robot, MCOperator* _motionController);
  void setJsonPrefix(const QString& prefix);

 private:
  void ensureFileExist();
  int ensureJsonStruct();
  void readData();
  void writeData();
  int readDeviceData(std::array<float, 4>& data);
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

 public:
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
  std::unique_ptr<rapidjson::Document> _dataDoc;
  std::unique_ptr<rapidjson::Document> _resultDoc;
};

#endif  // CALIEXTRAAXISWIDGET_H
