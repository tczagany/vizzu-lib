#include "optionssetter.h"

#include <cmath>

#include "base/io/log.h"
#include "base/text/smartstring.h"
#include "data/table/datatable.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter::OptionsSetter(Options &options,
    const OnFinished::Listener &onFinished) :
    options(options)
{
	if (onFinished) this->onFinished.attach(onFinished);
	changed = false;
	table = nullptr;
}

OptionsSetter::~OptionsSetter()
{
	if (changed) onFinished();
}

void OptionsSetter::setTable(const Data::DataTable *table)
{
	this->table = table;
}

OptionsSetter &OptionsSetter::addSeries(Scale::Type scaleType,
    const std::string &seriesName,
    std::optional<size_t> pos)
{
	if (Text::SmartString::startsWith(seriesName, "$"))
	{
		auto typeStr = seriesName.substr(1, std::string::npos);
		auto type = Data::SeriesType::fromString(typeStr);
		addSeries(scaleType, Data::SeriesIndex(type), pos);
	}
	else if (table)
	{
		auto index = table->getIndex(seriesName);
		addSeries(scaleType, index, pos);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(Scale::Type scaleType,
    const std::string &seriesName)
{
	if (Text::SmartString::startsWith(seriesName, "$"))
	{
		auto typeStr = seriesName.substr(1, std::string::npos);
		auto type = Data::SeriesType::fromString(typeStr);
		deleteSeries(scaleType, Data::SeriesIndex(type));
	}
	else if (table)
	{
		auto index = table->getIndex(seriesName);
		deleteSeries(scaleType, index);
	}
	else
		throw std::logic_error("no table set for options");

	return *this;
}

OptionsSetter &OptionsSetter::addSeries(Scale::Type scaleType,
										const Data::SeriesIndex &index,
										std::optional<size_t> pos)
{
	auto res = options.getScales().addSeries(scaleType, index, pos);
	changed |= res.first;
	if (res.first && res.second && onContinousReplaced)
		onContinousReplaced(scaleType, *res.second);
	return *this;
}

OptionsSetter &OptionsSetter::deleteSeries(
	Scale::Type scaleType, const Data::SeriesIndex &index)
{
	changed |= options.getScales().removeSeries(scaleType, index);
	return *this;
}

OptionsSetter &OptionsSetter::clearSeries(Scale::Type scaleType)
{
	changed |= options.getScales().clearSeries(scaleType);
	return *this;
}

OptionsSetter &OptionsSetter::setShape(const ShapeType::Type &type)
{
	changed |= options.shapeType.set(type);
	return *this;
}

OptionsSetter &OptionsSetter::setAlign(const Base::Align::Type &alignType)
{
	changed |= options.alignType.set(alignType);
	return *this;
}

OptionsSetter &OptionsSetter::setPolar(bool value)
{
	changed |= options.polar.set(Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::setSplitted(bool value)
{
	changed |= options.splitted.set(Math::FuzzyBool(value));
	return *this;
}

OptionsSetter &OptionsSetter::rotate(double ccwQuadrant)
{
	changed |= options.angle.set(options.angle.get() + ccwQuadrant * M_PI / 2);
	return *this;
}

OptionsSetter &OptionsSetter::setAngle(double ccwQuadrant)
{
	changed |= options.angle.set(ccwQuadrant * M_PI / 2);
	return *this;
}

OptionsSetter &OptionsSetter::setHorizontal(bool horizontal)
{
	changed |= options.horizontal.set(Math::FuzzyBool(horizontal));
	return *this;
}

OptionsSetter &OptionsSetter::setFilter(const Data::Filter &filter)
{
	changed |= options.dataFilter.set(filter);
	return *this;
}

OptionsSetter &OptionsSetter::setLabelLevel(Scale::Type scaleType,
    int level)
{
	auto &scale = options.getScales().at(scaleType);
	changed |= scale.labelLevel.set(level);
	return *this;
}

OptionsSetter &OptionsSetter::setSorted(bool value)
{
	changed |= options.sorted.set(value);
	return *this;
}

OptionsSetter &OptionsSetter::setReverse(bool value)
{
	changed |= options.reverse.set(value);
	return *this;
}

OptionsSetter &OptionsSetter::setRange(Scale::Type scaleType,
	Type::PhysicalValue<Math::Range<double> > range)
{
	auto &scale = options.getScales().at(scaleType);
	changed |= scale.range.set(range);
	return *this;
}

OptionsSetter &OptionsSetter::setStackable(Scale::Type scaleType, bool value)
{
	auto &scale = options.getScales().at(scaleType);
	if (scale.stackable() != value)
	{
		const_cast<bool&>(scale.stackable()) = value;
		changed = true;
	}
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(
    const std::optional<std::string> &title)
{
	changed = options.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setLegend(std::optional<Scale::Type> type)
{
	changed = options.legend.set(type);
	return *this;
}

OptionsSetter &OptionsSetter::setTitle(Scale::Type scaleType,
    const std::string &title)
{
	auto &scale = options.getScales().at(scaleType);
	changed |= scale.title.set(title);
	return *this;
}

OptionsSetter &OptionsSetter::setBubbleChartAlgorithm(
    BubbleChartAlgorithm value)
{
	changed = options.bubbleChartAlgorithm.set(value);
	return *this;
}

void OptionsSetter::replaceOptions(const Options &options)
{
	if (options != this->options) {
		this->options = options;
		changed = true;
	}
}
