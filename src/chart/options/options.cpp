
#include "options.h"

#include "base/math/trig.h"

using namespace Vizzu;
using namespace Vizzu::Diag;

Options::Options()
{
	alignType.set(Base::Align::None);
	polar.set(false);
	angle.set(0.0);
	horizontal.set(true);
	sorted.set(false);
	reverse.set(false);
	bubbleChartAlgorithm.set(BubbleChartAlgorithm::slow);
}

void Options::reset()
{
	scales.reset();
	title.set(title.get().get().has_value() 
		? Title(std::string()) : Title());
}

const Scale *Options::subAxisOf(Scale::Type type) const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Rectangle:
		return type == mainAxisType()
				? &subAxis()
				: nullptr;

	case ShapeType::Type::Area:
		return	type == mainAxisType() ? &subAxis() :
				type == subAxisType() ? &mainAxis() :
				nullptr;

	case ShapeType::Type::Line:
		return type == subAxisType()
				|| (type == Scale::Type::Size && scales.anyAxisSet())
			? &scales.at(Scale::Type::Size)
			: nullptr;

	case ShapeType::Type::Circle:
		// todo: should return 2 scale (size + other axis)
		if (type == Scale::Type::Size && scales.anyAxisSet()) {
			return &scales.at(Scale::Type::Size);
		} else if (isAxis(type)) {
			if (scales.at(type).isPseudoDiscrete() && type == mainAxisType())
				return  &subAxis();
			else
				return &scales.at(Scale::Type::Size);
		} else return nullptr;

	default:
		return nullptr;
	}
}

Scale::Type Options::stackAxisType() const
{
	switch ((ShapeType::Type)shapeType.get())
	{
	case ShapeType::Type::Area:
	case ShapeType::Type::Rectangle: return subAxisType();

	default:
	case ShapeType::Type::Circle:
	case ShapeType::Type::Line: return Scale::Type::Size;
	}
}

bool Options::operator==(const Options &other) const
{
	return polar.get() == other.polar.get()
	        && angle.get() == other.angle.get()
	        && shapeType.get() == other.shapeType.get()
			&& horizontal.get() == other.horizontal.get()
			&& splitted.get() == other.splitted.get()
			&& dataFilter.get() == other.dataFilter.get()
			&& alignType.get() == other.alignType.get()
			&& splitted.get() == other.splitted.get()
			&& scales == other.scales
	        && sorted.get() == other.sorted.get()
	        && reverse.get() == other.reverse.get()
			&& title.get() == other.title.get()
			&& legend.get() == other.legend.get();
}

Scale::Type Options::getHorizontalScale() const
{
	return (Math::rad2quadrant(angle.get()) % 2) == 0
			? Scale::Type::X : Scale::Type::Y;
}

Scale::Type Options::getVerticalScale() const
{
	return getHorizontalScale() == Scale::Type::X
			? Scale::Type::Y : Scale::Type::X;
}

bool Options::isShapeValid(const ShapeType::Type &shapeType) const
{
	if (scales.anyAxisSet()
		&& mainAxis().discreteCount() > 0)
	{
		return true;
	}
	else return shapeType == ShapeType::Rectangle
				|| shapeType == ShapeType::Circle;
}
