#ifndef STATEMACHINEFACTORY_H
#define STATEMACHINEFACTORY_H
#include <QStateMachine>
#include <QObject>
#include "Data/SignalState.h"


class StateMachineFactory: public QObject
{
private:
    QString name;
public:
    StateMachineFactory();
    ~StateMachineFactory();
    QStateMachine *GetStateMachine(QString name);
    void Start();
    SignalState::State GetState();
public:
    QStateMachine *machine;
    QState *stateIDLE;
    QState *waitLaserSignal;
    QState *processHeadImg;
    QState *headProcessDone;
    QState *processTrailImg;

    QSignalTransition *tranWaitSignal;
    QSignalTransition *tranProcessHeadImg;
    QSignalTransition *tranHeadProcessDone;
    QSignalTransition *tranProcessTrailImg;
    QSignalTransition *tranIDLE;

signals:
    void cameraSignalOn();
    void laserSignalOnAndImgReady();
    void headDone();
    void cameraSignalOffAndImgReady();
    void trailDone();
};

#endif // STATEMACHINEFACTORY_H
