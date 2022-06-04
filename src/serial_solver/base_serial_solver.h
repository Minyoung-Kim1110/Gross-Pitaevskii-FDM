#pragma once
#include <complex>
#include <vector>
#include <functional>
#include "src/domain/base_domain.h"
#include "src/initial_condition/initial_condition.h"
class BaseSolver
{
public:
    BaseSolver() = default;
    BaseSolver(InitialCondition initialCondition, std::function<double(double, double)> potential, double g, BaseDomain domain);

protected:
    InitialCondition initialCondition;
    BaseDomain *domain;
    std::function<double(double, double)> potential_func;
    double g;
    std::complex<double> temporal_equation(int i, int j, int k);
};