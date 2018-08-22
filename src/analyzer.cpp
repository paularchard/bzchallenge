#include <analyzer.hpp>

std::pair<date, double> 
analyzer::highest(void) const
{
    return std::make_pair(date(), 0.); 
}

std::pair<date, double>
analyzer::lowest(void) const
{
    return std::make_pair(date(), 0.);
}

double
analyzer::mean_value(void) const
{
    return 0.;
}

double
analyzer::median_value(void) const
{
    return 0.;
}

double
analyzer::stddev_value(void) const
{
    return 0.;
}
