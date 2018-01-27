/**
 * @file ProbeColumns.cpp
 * @brief Implements the functionality defined in ProbeColumns.hpp
*/

#include "ProbeColumns.hpp"

using namespace probe;

ProbeColumns::ProbeColumns()
{
    this->add(this->col_name);
    this->add(this->col_x);
    this->add(this->col_y);
};