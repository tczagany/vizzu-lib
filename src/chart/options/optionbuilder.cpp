
#include "optionbuilder.h"

#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsBuilder::OptionsBuilder(Options &options)
	: setter(options),
	  scaleType{ Scale::X }
{}

OptionsBuilder &OptionsBuilder::operator<<(const Data::DataTable &table)
{
	setter.setTable(&table);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Scale::Type &type)
{
	scaleType = type;
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const ShapeType::Type &t)
{
	setter.setShape(t);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Base::Align::Type &alignType)
{
	setter.setAlign(alignType);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Polar &polar)
{
	setter.setPolar(polar.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Horizontal &horizontal)
{
	setter.setHorizontal(horizontal.value);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const Data::SeriesIndex &index)
{
	setter.addSeries(scaleType, index);
	return *this;
}

OptionsBuilder &OptionsBuilder::operator<<(const char *seriesName)
{
	setter.addSeries(scaleType, seriesName);
	return *this;
}
