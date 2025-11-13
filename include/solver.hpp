#ifndef SOLVER_H
#define SOLVER_H

#include <functional>
#include <vector>

// Abstract base class
class Solver
{
public:
    /*
        Solves the IVP
        y'(t) = F(t, y)
        y(t0) = y0
    */
    virtual std::vector<std::vector<double>> solveIvp(
        const std::function<std::vector<double>(double, std::vector<double>)>& F,
        double t0,
        double t_f,
        std::vector<double> y0,
        double h) = 0;

    virtual std::vector<double> solveIvpStep(
        const std::function<std::vector<double>(double, std::vector<double>)>& F,
        double t,
        std::vector<double> y,
        double h) = 0;

    virtual ~Solver() = default; // Virtual destructor
};

// Concrete class: Euler's method
class Euler : public Solver
{
public:
    std::vector<std::vector<double>> solveIvp(
        const std::function<std::vector<double>(double, std::vector<double>)>& F,
        double t0,
        double t_f,
        std::vector<double> y0,
        double h) override;

    std::vector<double> solveIvpStep(
        const std::function<std::vector<double>(double, std::vector<double>)>& F,
        double t,
        std::vector<double> y,
        double h) override;
};

#endif // SOLVER_H