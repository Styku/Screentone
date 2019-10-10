#ifndef SCREEN_H
#define SCREEN_H

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

using namespace std;

class ScreenParams
{
public:
    struct Gamma
    {
        Gamma(double r = 1.0, double g = 1.0, double b = 1.0) : r{r}, g{g}, b{b} {}
        double r, g, b;
        static constexpr double EPSILON = 0.000001;

        friend bool operator!=(const Gamma& lhs, const Gamma& rhs)
        {
            return fabs(lhs.r - rhs.r) > EPSILON || fabs(lhs.g - rhs.g) > EPSILON || fabs(lhs.b - rhs.b) > EPSILON;
        }

        Gamma& operator+=(const Gamma& rhs)
        {
            this->r+=rhs.r;
            this->g+=rhs.g;
            this->b+=rhs.b;
            return *this;
        }

        friend Gamma operator+(Gamma lhs, const Gamma& rhs)
        {
          lhs += rhs;
          return lhs;
        }

        Gamma& operator-=(const Gamma& rhs)
        {
            this->r-=rhs.r;
            this->g-=rhs.g;
            this->b-=rhs.b;
            return *this;
        }

        friend Gamma operator-(Gamma lhs, const Gamma& rhs)
        {
          lhs -= rhs;
          return lhs;
        }

        Gamma& operator/=(const double& rhs)
        {
            this->r/=rhs;
            this->g/=rhs;
            this->b/=rhs;
            return *this;
        }

        friend Gamma operator/(Gamma lhs, const double& rhs)
        {
          lhs /= rhs;
          return lhs;
        }
    };

    ScreenParams(std::string display_id="DP-1-1");

    void refresh();

    Gamma set(unsigned int temp, unsigned int transition = 1000);

    Gamma computeStep(Gamma new_gamma, unsigned int steps = 30) const
    {
        return (new_gamma - gamma)/static_cast<double>(steps);
    }

    Gamma computeStep(unsigned int temperature, unsigned int steps = 30) const
    {
        return computeStep(tempToGamma(temperature), steps);
    }


    static Gamma tempToGamma(unsigned int temp);

private:
    std::string display_id;
    Gamma gamma;
    std::mutex execution;
};

#endif // SCREEN_H
