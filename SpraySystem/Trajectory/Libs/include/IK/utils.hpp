#pragma once
#include <string>
namespace vws {
namespace robot {
class AbbIK;
}  // namespace robot

}  // namespace vws

vws::robot::AbbIK* createRobotIK_ABB(const std::string& filename);
vws::robot::AbbIK* createRobotIK_1600();