
#ifndef COSIMJNI_COSIM_UNIT_CONVERSION_HPP
#define COSIMJNI_COSIM_UNIT_CONVERSION_HPP

#include <cosim.h>

namespace
{

const double sec2nano = 1e9;
const double nano2sec = 1.0 / sec2nano;

cosim_time_point to_cosim_time_point(double time_point)
{
    return static_cast<cosim_time_point>(time_point * sec2nano);
}

cosim_duration to_cosim_duration(double duration)
{
    return static_cast<cosim_duration>(duration * sec2nano);
}

double to_seconds(cosim_time_point duration)
{
    return static_cast<double>(duration) * nano2sec;
}

} // namespace

#endif
