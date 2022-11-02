#ifndef STRATEGYFORPLAN_H
#define STRATEGYFORPLAN_H

#include "basestrategy.h"

class StrategyForPlan : public BaseStrategy
{
public:
    StrategyForPlan();
    void work() override;
};

#endif // STRATEGYFORPLAN_H
