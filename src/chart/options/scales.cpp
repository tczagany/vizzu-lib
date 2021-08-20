#include "scales.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Scales::Scales()
{
	for (auto type = 0u; type < Scale::Type::id_size; type++)
		scales[type] = Scale::makeScale((Scale::Type)type);

	reset();
}

bool Scales::anyAxisSet() const
{
	return anyScaleOfType(Scale::X) || anyScaleOfType(Scale::Y);
}

bool Scales::oneAxisSet() const
{
	return anyAxisSet() && !bothAxisSet();
}

bool Scales::bothAxisSet() const
{
	return anyScaleOfType(Scale::X) && anyScaleOfType(Scale::Y);
}

bool Scales::isEmpty() const
{
	for (const auto &scale : scales)
		if(!scale.isEmpty()) return false;
	return true;
}

bool Scales::anyScaleOfType(Scale::Type type) const
{
	return !scales[type].isEmpty();
}

Data::DataCubeOptions::IndexSet Scales::getDimensions() const
{
	Data::DataCubeOptions::IndexSet dimensions;

	for (const auto &scale : scales)
		scale.collectDimesions(dimensions);

	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getSeries() const
{
	Data::DataCubeOptions::IndexSet series;

	for (const auto &scale : scales) 
		if (scale.continousId()) 
	{
		const auto &index = *scale.continousId();
		series.insert(index);
	}

	return series;
}

Data::DataCubeOptions::IndexSet Scales::getDimensions(
	const std::vector<Scale::Type> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet dimensions;
	for (auto scaleType : scaleTypes)
		scales[(Scale::Type)scaleType].collectDimesions(dimensions);
	return dimensions;
}

Data::DataCubeOptions::IndexSet Scales::getRealSeries(const std::vector<Scale::Type> &scaleTypes) const
{
	Data::DataCubeOptions::IndexSet series;
	for (auto scaleType : scaleTypes)
		scales[(Scale::Type)scaleType].collectRealSeries(series);
	return series;
}

Data::DataCubeOptions Scales::getDataCubeOptions() const
{
	return Data::DataCubeOptions(getDimensions(), getSeries());
}

std::pair<bool, Scale::OptionalContinousIndex>
Scales::addSeries(Scale::Type type,
					   const Data::SeriesIndex &index,
					   std::optional<size_t> pos)
{
	return scales[type].addSeries(index, pos);
}

bool Scales::removeSeries(Scale::Type type, const Data::SeriesIndex &index)
{
	return scales[type].removeSeries(index);
}

bool Scales::clearSeries(Scale::Type type)
{
	if (scales[type].isEmpty()) return false;
	scales[type].reset();
	return true;
}

bool Scales::isSeriesUsed(const Data::SeriesIndex &index) const
{
	for (const auto &scale : scales)
		if (scale.isSeriesUsed(index))
			return true;
	return false;
}

bool Scales::isSeriesUsed(const std::vector<Scale::Type> &scaleTypes, const Data::SeriesIndex &index) const
{
	for (auto scaleType : scaleTypes)
		if(scales[(Scale::Type)scaleType].isSeriesUsed(index)) 
			return true;
	return false;
}

size_t Scales::count(const Data::SeriesIndex &index) const
{
	size_t cnt = 0;
	for (const auto &scale : scales)
		if (scale.isSeriesUsed(index)) cnt++;
	return cnt;
}

std::list<Scale::Type> Scales::find(const Data::SeriesIndex &index) const
{
	std::list<Scale::Type> res;
	visitAll([&](Scale::Type type, const Scale& scale)
	{
		if (scale.isSeriesUsed(index)) res.push_back(type);
	});
	return res;
}

std::list<Scales::Pos> Scales::findPos(const Data::SeriesIndex &index) const
{
	std::list<Scales::Pos> res;
	visitAll([&](Scale::Type type, const Scale& scale)
	{
		auto pos = scale.findPos(index);
		if (pos >= 0) res.push_back({ type, pos });
	});
	return res;
}

const Scale &Scales::at(const Scale::Type &type) const
{
	return scales[type];
}

Scale &Scales::at(const Scale::Type &type)
{
	return scales.at(type);
}

Scale::Type Scales::getEmptyAxisId() const
{
	if (at(Scale::X).isEmpty()) return Scale::X;
	if (at(Scale::Y).isEmpty()) return Scale::Y;
	return Scale::Type::id_size;
}

void Scales::reset()
{
	for (auto &scale: scales) scale.reset();
}

bool Scales::operator==(const Scales &other) const
{
	return scales == other.scales;
}

void Scales::visitAll(
	const std::function<void(Scale::Type, const Scale &)> &visitor) const
{
	for (auto type = 0u; type < (size_t)Scale::Type::id_size; type++)
	{
		const auto &scale = scales[(Scale::Type)type];
		visitor((Scale::Type)type, std::ref(scale));
	}
}
