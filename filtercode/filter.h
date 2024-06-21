#ifndef D__ITI_GP_FILTERC___FILTER_H_
#define D__ITI_GP_FILTERC___FILTER_H_

#include <iostream>
#include <vector>
#include <numeric>

class MovingAverageFilter {
public:
    MovingAverageFilter(size_t window_size);
    double filter(double new_value);
private:
    size_t window_size_;           // The number of samples to average over
    std::vector<double> values_;   // The vector to store the values
};
























#endif // D__ITI_GP_FILTERC___FILTER_H_