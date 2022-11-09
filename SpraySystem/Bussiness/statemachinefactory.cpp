#include "statemachinefactory.h"
#include <iostream>


StateMachineFactory::StateMachineFactory()
{

}

StateMachineFactory::~StateMachineFactory()
{
    delete machine;

    delete stateIDLE;
    delete waitLaserSignal;
    delete processHeadImg;
    delete headProcessDone;
    delete processTrailImg;
    delete stateIDLE;

    delete tranWaitSignal;
    delete tranProcessHeadImg;
    delete tranHeadProcessDone;
    delete tranProcessTrailImg;
    delete tranIDLE;
}

QStateMachine *StateMachineFactory::GetStateMachine(QString name)
{
    this->name = name;

    machine = new QStateMachine();
    stateIDLE = new QState(machine);
    waitLaserSignal = new QState(machine);
    processHeadImg = new QState(machine);
    headProcessDone = new QState(machine);
    processTrailImg = new QState(machine);

    tranWaitSignal = stateIDLE->addTransition(this,SIGNAL(cameraSignalOn()),waitLaserSignal);
    tranProcessHeadImg = waitLaserSignal->addTransition(this,SIGNAL(laserSignalOnAndImgReady()),processHeadImg);
    tranHeadProcessDone = waitLaserSignal->addTransition(this,SIGNAL(headDone()),headProcessDone);
    tranProcessTrailImg = waitLaserSignal->addTransition(this,SIGNAL(processTrailImg()),processHeadImg);
    tranIDLE = waitLaserSignal->addTransition(this,SIGNAL(trailDone()),stateIDLE);

    waitLaserSignal->assignProperty(machine,"state",SignalState::State::WaitLaserSignalOn);
    processHeadImg->assignProperty(machine,"state",SignalState::State::ProcessHeadImg);
    headProcessDone->assignProperty(machine,"state",SignalState::State::HeadImgDone);
    processHeadImg->assignProperty(machine,"state",SignalState::State::ProcessTrailImg);
    stateIDLE->assignProperty(machine,"state",SignalState::State::IDLE);
    return machine;
}

void StateMachineFactory::Start()
{
//    std::cout<<name<<",启动"<<std::endl;
    machine->start();
}

SignalState::State StateMachineFactory::GetState()
{
    auto state = machine->property("state");
    return state.value<SignalState::State>();
}

