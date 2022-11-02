#include "plcdatapaser.h"

PLCDataPaser::PLCDataPaser()
{

}

void PLCDataPaser::DataPaser(QByteArray buf, PLCData &data)
{
    auto hexArr = buf.toHex();
    std::vector<int> laserVec;
    for(int i=0; i<=hexArr.length() - 4; i+=4){
       auto hex= QString("0x%1%2%3%4").arg(hexArr[i+2]).arg(hexArr[i+3]).arg(hexArr[i]).arg(hexArr[i+1]);
       //auto num =  QString("%1").arg(hex,0,10).toInt();
       bool ok;
       auto num = hex.toUInt(&ok,16);
       laserVec.push_back(num);
    }

    data.laser1 = laserVec.at(0);
    data.laser2 = laserVec.at(1);
    data.laser3 = laserVec.at(2);
    data.laser4 = laserVec.at(3);
}
