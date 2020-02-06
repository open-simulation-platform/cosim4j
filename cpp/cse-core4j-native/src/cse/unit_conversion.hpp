
#ifndef CSECOREJNI_UNIT_CONVERSION_HPP
#define CSECOREJNI_UNIT_CONVERSION_HPP

#include <cse.h>

namespace
{

const double sec2nano = 1e9;
const double nano2sec = 1.0 / sec2nano;

cse_time_point to_cse_time_point(double time_point)
{
    return static_cast<cse_time_point>(time_point * sec2nano);
}

cse_duration to_cse_duration(double duration)
{
    return static_cast<cse_duration>(duration * sec2nano);
}

double to_seconds(cse_time_point duration)
{
    return static_cast<double>(duration) * nano2sec;
}

} // namespace

#endif
