#include "autosplitter.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &AutoSplitter::addSeries(Scale::Type scaleType,
    const Data::SeriesIndex &index,
    std::optional<size_t> pos)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleType == options.mainAxisType()
		    && !options.subAxisOf(scaleType)->isSeriesUsed(index))
		{
			auto otherType = options.subAxisType();

			setter.addSeries(otherType, index);
			onFinished();
			setter.addSeries(scaleType, index, pos);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(otherType, index);
		}
		else
			setter.addSeries(scaleType, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle
	         && index.getType().isDiscrete())
	{
		if (scaleType != Scale::Size
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(Scale::Size, index);
			onFinished();
			setter.addSeries(scaleType, index, pos);
			setter.deleteSeries(Scale::Size, index);
		}
		else
			setter.addSeries(scaleType, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line
	         && index.getType().isDiscrete())
	{
		if (scaleType != Scale::Size
		    && !options.getScales().isSeriesUsed(index))
		{
			setter.addSeries(Scale::Size, index);
			if (scaleType != options.mainAxisType()) onFinished();
			setter.addSeries(scaleType, index, pos);
			if (scaleType == options.mainAxisType()) onFinished();
			setter.deleteSeries(Scale::Size, index);
		}
		else
			setter.addSeries(scaleType, index, pos);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleType == options.mainAxisType()
		    && !options.subAxisOf(scaleType)->isSeriesUsed(index))
		{
			auto other = options.subAxisType();

			setter.addSeries(other, index);
			setter.addSeries(scaleType, index, pos);
			onFinished();
			setter.deleteSeries(other, index);
		}
		else
			setter.addSeries(scaleType, index, pos);
	}
	else
		setter.addSeries(scaleType, index, pos);

	return *this;
}

OptionsSetter &AutoSplitter::deleteSeries(Scale::Type scaleType,
    const Data::SeriesIndex &index)
{
	if (options.shapeType.get() == ShapeType::Type::Rectangle
	    && index.getType().isDiscrete())
	{
		if (scaleType == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index))
		{
			auto other = options.subAxisType();

			setter.addSeries(other, index);
			if (options.shapeType.get() != ShapeType::Rectangle)
				onFinished();
			setter.deleteSeries(scaleType, index);
			onFinished();
			setter.deleteSeries(other, index);
		}
		else
			setter.deleteSeries(scaleType, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Circle)
	{
		auto subType = Scale::Size;

		if (scaleType != subType && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(subType, index);
			setter.deleteSeries(scaleType, index);
			onFinished();
			setter.deleteSeries(subType, index);
		}
		else
			setter.deleteSeries(scaleType, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Line)
	{
		auto subType = Scale::Size;

		if (scaleType != subType && index.getType().isDiscrete()
		    && options.getScales().count(index) == 1)
		{
			setter.addSeries(subType, index);
			if (scaleType == options.mainAxisType()) onFinished();
			setter.deleteSeries(scaleType, index);
			if (scaleType != options.mainAxisType()) onFinished();
			setter.deleteSeries(subType, index);
		}
		else
			setter.deleteSeries(scaleType, index);
	}
	else if (options.shapeType.get() == ShapeType::Type::Area
	         && index.getType().isDiscrete())
	{
		if (scaleType == options.mainAxisType()
		    && !options.subAxis().isSeriesUsed(index))
		{
			auto other = options.subAxisType();

			setter.addSeries(other, index);
			onFinished();
			setter.deleteSeries(scaleType, index);
			setter.deleteSeries(other, index);
		}
		else
			setter.deleteSeries(scaleType, index);
	}
	else
		setter.deleteSeries(scaleType, index);

	return *this;
}

OptionsSetter &AutoSplitter::setSplitted(bool split)
{
	if (split)
	{
		if (options.shapeType.get() == ShapeType::Area)
			setter.setSplitted(true);
	}
	else
		setter.setSplitted(false);

	return *this;
}

OptionsSetter &AutoSplitter::setShape(const ShapeType::Type &type)
{
	if (type != ShapeType::Area) setter.setSplitted(false);

	setter.setShape(type);

	return *this;
}
