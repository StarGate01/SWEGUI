#include "DataProbe.hpp"

using namespace probe;

ProbeColumns DataProbe::cols;

DataProbe::DataProbe()
{ }

DataProbe::DataProbe(float px, float py)
    : x(px), y(py)
{
    sprite.setOrigin(16.f, 16.f);
}

sf::Sprite& DataProbe::getSprite()
{
    return sprite;
}

void DataProbe::fill_row(const Gtk::TreeRow& row)
{
    row[cols.col_x] = x;
    row[cols.col_y] = y;
}