#include "filter.h"

MovingAverageFilter::MovingAverageFilter(size_t window_size) : window_size_(window_size)
{

}

double MovingAverageFilter::filter(double new_value) {
    // Check if the size of the vector has reached the window size
    if (values_.size() >= window_size_) {
        // Remove the oldest value to maintain the window size
        values_.erase(values_.begin());
    }
    // Add the new value
    values_.push_back(new_value);
    // Calculate and return the average of the values
    return std::accumulate(values_.begin(), values_.end(), 0.0) / values_.size();
}