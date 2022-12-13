#ifndef CALIEXTRAAXISWIDGET_H
#define CALIEXTRAAXISWIDGET_H

#include <QString>
#include <QWidget>
#include <array>
#include <memory>
#include <string>

#include "Forms/Calibration/basecaliwidget.h"
#include "Util/jsonParser/jsonparser.hpp"

namespace Ui {
class caliExtraAxisWidget;
}

class RobotOperator;
class MCOperator;

class caliExtraAxisWidget : public baseCaliWidget {
  Q_OBJECT

 public:
  explicit caliExtraAxisWidget(const std::string& prefix,
                               QWidget* parent = nullptr);
  ~caliExtraAxisWidget();
  void setDevice(RobotOperator* robot, MCOperator* _motionController);

 private:
  int readDeviceData(std::array<float, 4>& data);
  void recordData(const std::array<float, 4>& data);
  void clearResult() override;
  void deleteLastItem();
  void updateTree() override;
  void calculate();

 private slots:
  void on_btn_record_clicked();

  void on_btn_calculate_clicked();

  void on_btn_delete_clicked();

 private:
  Ui::caliExtraAxisWidget* ui;
  // device
  RobotOperator* _robot;
  MCOperator* _motionController;
  // keyprefix
  std::string _jsonPrefix;
};

#endif  // CALIEXTRAAXISWIDGET_H
