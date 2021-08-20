#include "overlappingfixer.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &OverlappingFixer::addSeries(
	Scale::Type scaleType,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	setter.addSeries(scaleType, index, pos);
	fixOverlap(false, (ShapeType::Type)options.shapeType.get());
	return *this;
}

OptionsSetter &OverlappingFixer::deleteSeries(
    Scale::Type scaleType,
    const Data::SeriesIndex &index)
{
	setter.deleteSeries(scaleType, index);
	fixOverlap(true, (ShapeType::Type)options.shapeType.get());
	return *this;
}

OptionsSetter &OverlappingFixer::setShape(const ShapeType::Type &type)
{
	if (options.shapeType.get() != ShapeType::Circle
	    && type == ShapeType::Circle)
	{
		setter.setShape(type);
		fixOverlap(false, type);
	}
	else if (options.shapeType.get() == ShapeType::Circle
	         && type != ShapeType::Circle)
	{
		fixOverlap(false, type);
		onFinished();
		setter.setShape(type);
	}
	else
	{
		setter.setShape(type);
		fixOverlap(false, (ShapeType::Type)options.shapeType.get());
	}
	return *this;
}

OptionsSetter &OverlappingFixer::setHorizontal(bool horizontal)
{
	setter.setHorizontal(horizontal);
	if (canOverlap((ShapeType::Type)options.shapeType.get()))
	{
		std::list<Data::SeriesIndex> ids;
		auto sub = options.subAxis();
		for (const auto &id : sub.discretesIds())
		{
			ids.push_back(id);
			setter.addSeries(options.mainAxisType(), id);
		}
		for (const auto &id : ids)
		{
			setter.deleteSeries(options.subAxisType(), id);
		}
	}
	return *this;
}

void OverlappingFixer::fixOverlap(bool byDelete, ShapeType::Type type)
{
	if (!options.getScales().anyAxisSet()) return;
	if (enableOverlap) return;

	if (!canOverlap(type))
	{
		removeOverlap(byDelete);
	}
}

void OverlappingFixer::removeOverlap(bool byDelete)
{
	auto usedSeries = options.getScales().getDimensions();

	for (auto series : usedSeries)
	{
		auto scaleTypes = options.getScales().find(series);
		bool usedOnAxis = false;
		bool usedOnSize = false;

		for (auto &scaleType : scaleTypes)
		{
			if (isAxis(scaleType)) usedOnAxis = true;
			if (scaleType == Scale::Size) usedOnSize = true;
		}

		if (!usedOnAxis)
		{
			if (byDelete)
			{
				for (auto &scaleType : scaleTypes)
					setter.deleteSeries(scaleType, series);
			}
			else
			{
				auto id = usedOnSize 
					? options.subAxisType()
				    : options.mainAxisType();
				setter.addSeries(id, series);
			}
		}
	}
}

