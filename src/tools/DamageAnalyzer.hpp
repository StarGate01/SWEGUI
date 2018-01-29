#ifndef DAMAGE_ANALYZER_HPP
#define DAMAGE_ANALYZER_HPP

#include <list>
#include <map>

//TODO: Add class to scons (cpp file)

class DamageAnalyzer
{
    private:
        std::list<Coordinate> get_shore_cells();
    public:
        std::list<std::map<Coordinate, float>> get_max_height();
        
};
#endif