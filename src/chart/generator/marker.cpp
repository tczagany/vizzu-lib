#include "marker.h"

#include <iomanip>
#include <sstream>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "chart/main/style.h"

#include "scalestats.h"

using namespace Vizzu;
using namespace Vizzu::Diag;
using namespace Geom;

Marker::Id::Id(const Data::DataCube &data,
					const Scale::DiscreteIndices &discretesIds,
					const Data::MultiDim::MultiIndex &index)
{
	seriesId = data.subSliceID(discretesIds, index);
	itemSliceIndex = data.subSliceIndex(discretesIds, index);
	itemId = data.getData().unfoldSubSliceIndex(itemSliceIndex);
}

Marker::Marker(const Options &options,
						 const Styles::Chart &style,
						 const Data::DataCube &data,
						 const Data::DataTable &table,
						 ScalesStats &stats,
						 const Data::MultiDim::MultiIndex &index,
						 size_t idx)
	: index(index), idx(idx)
{
	enabled =  data.subCellSize() == 0
			|| !data.getData().at(index).subCells[0].isEmpty();

	auto &scales = options.getScales();

	auto color =
		getValueForScale(scales, Scale::Color, data, stats);

	auto lightness =
		getValueForScale(scales, Scale::Lightness, data, stats);

	if (scales.at(Scale::Color).isPseudoDiscrete())
	{
		colorBuilder = ColorBuilder(style.data.lightnessRange(),
		    *style.data.colorPalette,
			(int)color,
		    lightness);
	}
	else
	{
		colorBuilder = ColorBuilder(style.data.lightnessRange(),
		    *style.data.colorGradient,
		    color,
		    lightness);
	}
	sizeFactor = getValueForScale(scales, Scale::Size, data, stats,
								  options.subAxisOf(Scale::Size));
	sizeId = Id(data, scales.at(Scale::Size).discretesIds(), index);

	mainId = Id(data, options.mainAxis().discretesIds(), index);

	bool stackInhibitingShape = options.shapeType.get() == ShapeType::Area;
	if (stackInhibitingShape) {
		Data::SeriesList subIds(options.subAxis().discretesIds());
		subIds.remove(options.mainAxis().discretesIds());
		subId = Id(data, subIds, index);
		Data::SeriesList stackIds(options.subAxis().discretesIds());
		stackIds.section(options.mainAxis().discretesIds());
		stackId = Id(data, stackIds, index);
	}
	else {
		stackId = subId = Id(data, options.subAxis().discretesIds(), index);
	}

	position.x =
	size.x = getValueForScale(scales, Scale::X, data, stats,
							  options.subAxisOf(Scale::X),
							  !options.horizontal.get() && stackInhibitingShape);

	spacing.x = (options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales.at(Scale::X).isPseudoDiscrete()) ? 1 : 0;

	position.y =
	size.y = getValueForScale(scales, Scale::Y, data, stats,
							  options.subAxisOf(Scale::Y),
							  options.horizontal.get() && stackInhibitingShape);

	spacing.y = (!options.horizontal.get()
				&& options.getScales().anyAxisSet()
				&& scales.at(Scale::Y).isPseudoDiscrete()) ? 1 : 0;

	if (scales.at(Scale::Label).isEmpty())
		label = ::Anim::Weighted<Label>(Label(),0);
	else {
		auto value = getValueForScale(scales, Scale::Label, data, stats);
		auto sliceIndex = data.subSliceIndex(scales.at(Scale::Label).discretesIds(), index);
		if (scales.at(Scale::Label).isPseudoDiscrete())
			label = Label(sliceIndex, data, table);
		else
			label = Label(value,
						  *scales.at(Scale::Label).continousId(),
						  sliceIndex, data, table);
	}
}

void Marker::setNextMarker(uint64_t itemId,
							  Marker *marker,
							  bool horizontal,
							  bool main)
{
	double Point::*coord = horizontal ? &Point::x : &Point::y;

	if (marker)
	{
		(main ? nextMainMarkerIdx : nextSubMarkerIdx) = marker->idx;

		if (main) marker->prevMainMarkerIdx = idx;

		if (itemId != 0)
		{
			marker->position.*coord += position.*coord;
		}
	}
}

void Marker::setIdOffset(size_t offset)
{
	if ((bool)prevMainMarkerIdx) (*prevMainMarkerIdx).value += offset;
	if ((bool)nextMainMarkerIdx) (*nextMainMarkerIdx).value += offset;
	if ((bool)nextSubMarkerIdx) (*nextSubMarkerIdx).value += offset;
}

double Marker::getValueForScale(const Scales &scales,
									 Scale::Type type,
									 const Data::DataCube &data,
									 ScalesStats &stats,
									 const Scale *subScale,
									 bool inhibitStack) const
{
	const auto &scale = scales.at(type);

	if (scale.isEmpty()) return scale.defaultValue();

	Scale::DiscreteIndices sumBy;

	if (subScale) {
		if (inhibitStack)
		{
			for (auto id : subScale->discretesIds())
				if (scale.isSeriesUsed(id))
					sumBy.pushBack(id);
		}
		else {
			sumBy = subScale->discretesIds();
			for (auto id : scale.discretesIds())
				sumBy.remove(id);
		}
	}

	auto continuous = scale.continousId();

	double value;
	double singlevalue;
	auto id = Id(data, scale.discretesIds(), index);

	auto &stat = stats.scales[type];

	if (continuous)
	{
		singlevalue = (double)data.valueAt(index, *continuous);

		if (scale.stackable())
			value = (double)data.aggregateAt(index, sumBy, *continuous);
		else
			value = singlevalue;
	}
	else
	{
		if (scale.stackable())
			value = 1.0;
		else
			value = (double)id.itemId;
	}

	if (enabled) {
		if (scale.isPseudoDiscrete()) stat.track(id);
		else {
			if (continuous) stat.trackSingle(singlevalue);
			stat.track(value);
		}
	}
	return value;
}

Rect Marker::toRectangle() const
{
	return Rect(position - size, size);
}

void Marker::fromRectangle(const Rect &rect)
{
	position = rect.pos + rect.size;
	size = rect.size;
}

Math::Range<double> Marker::getSizeBy(bool horizontal) const
{
	return horizontal ? toRectangle().hSize() : toRectangle().vSize();
}

void Marker::setSizeBy(bool horizontal, const Math::Range<double> range)
{
	auto rect = toRectangle();
	if (horizontal) rect.setHSize(range);
	else rect.setVSize(range);
	fromRectangle(rect);
}

Marker::Label::Label(const Data::MultiDim::SubSliceIndex &index,
						  const Data::DataCube &data,
						  const Data::DataTable &table)
	: hasValue(false), value(0.0)
{
	indexStr = getIndexString(index, data, table);
}

Marker::Label::Label(double value,
						  const Data::SeriesIndex &continous,
						  const Data::MultiDim::SubSliceIndex &index,
						  const Data::DataCube &data,
						  const Data::DataTable &table)
	: hasValue(true), value(value)
{
	unit = table.getInfo(continous.getColIndex()).getUnit();
	indexStr = getIndexString(index, data, table);
}

bool Marker::Label::operator==(const Marker::Label &other) const {
	return hasValue == other.hasValue
			&& value == other.value
			&& unit == other.unit
			&& indexStr == other.indexStr;
}

std::string Marker::Label::getIndexString(
	const Data::MultiDim::SubSliceIndex &index,
	const Data::DataCube &data,
	const Data::DataTable &table) const
{
	std::string res;

	for (auto i = 0u; i < index.size(); i++)
	{
		if (!res.empty()) res += ", ";
		auto colIndex = data.getSeriesByDim(index[i].dimIndex).getColIndex();
		auto value = table.getInfo(colIndex).discreteValues()[index[i].index];
		res += value;
	}
	return res;
}
