#include "advancedoptions.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

OptionsSetter &ExistsHandler::addSeries(Scale::Type scaleType,
										 const Data::SeriesIndex &index,
										 std::optional<size_t> pos)
{
	Base::addSeries(scaleType, index, pos);
	handleExists();
	return *this;
}

OptionsSetter &ExistsHandler::deleteSeries(Scale::Type scaleType,
											const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleType, index);
	handleExists();
	return *this;
}

OptionsSetter &ExistsHandler::setHorizontal(bool horizontal)
{
	Base::setHorizontal(horizontal);
	handleExists();
	return *this;
}

void ExistsHandler::handleExists()
{
	if (forcedExistsSeries)
		options.getScales().visitAll(
		[=](Scale::Type id, const Scale& scale)
		{
			if (scale.discretesIds().empty()
				&& scale.continousId()
				&& scale.continousId()->getType() == Data::SeriesType::Exists)
				Base::deleteSeries(id, Data::SeriesIndex(Data::SeriesType::Exists));

			if ((Diag::isAxis(id) || id == Scale::Size)
				&& !scale.isEmpty()
				&& !scale.continousId())
			{
				Base::addSeries(id, Data::SeriesIndex(Data::SeriesType::Exists));
			}

		});
}

OptionsSetter &AdvancedOptions::deleteSeries(
	Scale::Type scaleType,
	const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleType, index);

	if(!options.getScales().anyAxisSet()
		&& (	(ShapeType::Type)options.shapeType.get() != ShapeType::Circle
			||	(ShapeType::Type)options.shapeType.get() != ShapeType::Circle)
		)
	{
		Base::setShape(ShapeType::Rectangle);
	}
	return *this;
}

OptionsSetter &OrientationSelector::addSeries(
	Scale::Type scaleType,
	const Data::SeriesIndex &index,
	std::optional<size_t> pos)
{
	Base::addSeries(scaleType, index, pos);
	fixHorizontal();
	return *this;
}

OptionsSetter &OrientationSelector::deleteSeries(Scale::Type scaleType,
												 const Data::SeriesIndex &index)
{
	Base::deleteSeries(scaleType, index);
	fixHorizontal();
	return *this;
}

OptionsSetter &OrientationSelector::setHorizontal(bool horizontal)
{
	auto horOver = horizontalOverride();
	Base::setHorizontal(horOver ? *horOver : horizontal);
	return *this;
}

void OrientationSelector::fixHorizontal()
{
	auto horOver = horizontalOverride();
	if (horOver) Base::setHorizontal(*horOver);
}

std::optional<bool> OrientationSelector::horizontalOverride() const
{
	if (options.getScales().anyAxisSet()
		&& options.shapeType.get().type() != ShapeType::Circle)
	{
		auto &x = options.getScales().at(Scale::X);
		auto &y = options.getScales().at(Scale::Y);

		if (x.isEmpty() && !y.isPseudoDiscrete()) return true;
		if (y.isEmpty() && !x.isPseudoDiscrete()) return false;

		if (!x.discretesIds().empty() && y.discretesIds().empty() && !y.isPseudoDiscrete()) return true;
		if (!y.discretesIds().empty() && x.discretesIds().empty() && !x.isPseudoDiscrete()) return false;

		if (!x.discretesIds().empty() && !y.discretesIds().empty())
		{
			if (x.isPseudoDiscrete() && !y.isPseudoDiscrete()) return true;
			if (y.isPseudoDiscrete() && !x.isPseudoDiscrete()) return false;
		}
	}
	return std::nullopt;
}
