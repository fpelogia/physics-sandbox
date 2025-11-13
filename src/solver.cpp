#include "solver.hpp"
#include <iostream>

std::vector<std::vector<double>> Euler::solveIvp(
    const std::function<std::vector<double>(double, std::vector<double>, std::vector<double>)>& F,
    double t0,
    double t_f,
    std::vector<double> y0,
    double h,
    std::vector<double> params
)
{
    /*
        Solves the IVP
        y'(t) = F(t, y)
        y(t0) = y0
        using Euler's method:
            y_{k+1} = y_k + h * F(t_k, y_k)
    */

    double t = t0;
    std::vector<std::vector<double>> y_values;
    std::vector<double> y = y0;
    
    while (t <= t_f)
    {
        y = solveIvpStep(F, t, y, h, params);
        y_values.push_back(y);
        t = t + h;
    }

    return y_values;
}

std::vector<double> Euler::solveIvpStep(
    const std::function<std::vector<double>(double, std::vector<double>, std::vector<double>)>& F,
    double t,
    std::vector<double> y,
    double h,
    std::vector<double> params
)
    {

    int N_DIMENSIONS = y.size();
    
    std::vector<double> y_new = {}; 
    y_new.resize(N_DIMENSIONS);

    std::vector<double> F_t_y = F(t, y, params);

    for(int i = 0; i < N_DIMENSIONS; i++){
        /*
        y_{k+1} = y_k + h * F(t_k, y_k)
        */
       y_new[i] =  y[i] + h * F_t_y[i];
    }

    return y_new;
}