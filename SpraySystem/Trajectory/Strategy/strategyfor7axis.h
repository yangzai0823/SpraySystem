#ifndef STRATEGYFOR7AXIS_H
#define STRATEGYFOR7AXIS_H

#include "basestrategy.h"

class StrategyFor7Axis : public BaseStrategy
{
public:
    StrategyFor7Axis();
    void work() override;
};

#endif // STRATEGYFOR7AXIS_H
