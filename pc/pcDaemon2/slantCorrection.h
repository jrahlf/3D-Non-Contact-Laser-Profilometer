#pragma once

#include <string>

namespace SlantCorrect{
bool init(std::string);
void correct(double x, double y, double& z);
}
