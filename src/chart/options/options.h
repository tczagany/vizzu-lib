#ifndef DIAGRAMOPTIONS_H
#define DIAGRAMOPTIONS_H

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "base/anim/interpolated.h"
#include "base/geom/rect.h"
#include "base/math/fuzzybool.h"
#include "base/math/range.h"
#include "base/refl/enum.h"
#include "base/util/templates.h"
#include "data/datacube/datafilter.h"
#include "data/table/datatable.h"

#include "align.h"
#include "scales.h"
#include "shapetype.h"

namespace Vizzu
{
namespace Diag
{

class Enum(BubbleChartAlgorithm)(slow, experimental);

class Options
{
public:
	typedef ::Anim::Interpolated<std::optional<std::string>> Title;
	typedef ::Anim::Interpolated<std::optional<Scale::Type>> Legend;

	Options();

	const Scales &getScales() const { return scales; }
	Scales &getScales() { return scales; }

	void reset();

	Scale::Type mainAxisType() const {
		return horizontal.get() ? Scale::X : Scale::Y;
	}

	Scale::Type subAxisType() const {
		return horizontal.get() ? Scale::Y : Scale::X;
	}

	const Scale &mainAxis() const {
		return scales.at(mainAxisType());
	}

	const Scale &subAxis() const {
		return scales.at(subAxisType());
	}

	const Scale *subAxisOf(Scale::Type type) const;
	Scale::Type stackAxisType() const;

	Util::ReadWrite<Title> title;
	Util::ReadWrite<Math::FuzzyBool> polar;
	Util::ReadWrite<double> angle;
	Util::ReadWrite<ShapeType> shapeType;
	Util::ReadWrite<Math::FuzzyBool> horizontal;
	Util::ReadWrite<Math::FuzzyBool> splitted;
	Util::ReadWrite<Base::Align::Type> alignType;
	Util::ReadWrite<Data::Filter> dataFilter;
	Util::ReadWrite<Math::FuzzyBool> sorted;
	Util::ReadWrite<Math::FuzzyBool> reverse;
	Util::ReadWrite<Legend> legend;

	Util::ReadWrite<BubbleChartAlgorithm> bubbleChartAlgorithm;

	bool operator==(const Options& other) const;

	Scale::Type getHorizontalScale() const;
	Scale::Type getVerticalScale() const;

	const Scale &getHorizontalAxis() const {
		return scales.at(getHorizontalScale());
	}

	const Scale &getVeritalAxis() const {
		return scales.at(getVerticalScale());
	}

	bool isShapeValid(const ShapeType::Type &) const;

private:

	Scales scales;
};

typedef std::shared_ptr<Options> DiagramOptionsPtr;

}
}

#endif
