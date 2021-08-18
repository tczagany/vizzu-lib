#include "chart/animator/styles.h"

#include "base/refl/struct.h"
#include "base/math/interpolation.h"

using namespace Vizzu;
using namespace Vizzu::Anim::Morph;

class StyleMorph : public ::Anim::IElement
{
public:
	StyleMorph(const Style::IParam &source, const Style::IParam &target, 
		Style::IParam &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		if (factor <= 0.0) actual = source;
		else if (factor >= 1.0) actual = target;
		else {
			actual = source;
			actual *= (1.0 - factor);
			actual += target/* * factor*/;
		}
	}

	const Style::IParam &source;
	const Style::IParam &target;
	Style::IParam &actual;
};

StyleMorphFactory::StyleMorphFactory(
	const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual,
    ::Anim::Group &group,
	const ::Anim::Options &options) :
	group(group), options(options)
{
	pSource = reinterpret_cast<const std::byte *>(&source);
	pTarget = reinterpret_cast<const std::byte *>(&target);
	pActual = reinterpret_cast<std::byte *>(&actual);

	actual.visit(*this);
}

Style::Visitor &StyleMorphFactory::operator()(
	Style::IParam &value, 
	const std::string &)
{
	//todo: interpolate the folloving styles also
/*	   !std::is_same_v<typename T::value_type, Gfx::Font::Style>
	&& !std::is_same_v<typename T::value_type, Text::NumberFormat>
	&& !std::is_same_v<typename T::value_type, Styles::MarkerLabel::Format>
	&& !std::is_same_v<typename T::value_type, Gfx::ColorPalette>
	)*/
	auto offset = reinterpret_cast<std::byte *>(&value) - pActual;
	const auto &source = *reinterpret_cast<const Style::IParam *>(pSource + offset);
	const auto &target = *reinterpret_cast<const Style::IParam *>(pTarget + offset);

	if (source != target)
	{
		auto morph = std::make_unique<StyleMorph>
			(source, target, value);

		group.addElement(std::move(morph), options);
	}
	return *this;
}

Style::Visitor &StyleMorphFactory::operator()(
	Style::Group &group, 
	const std::string &)
{
	group.visit(*this);
	return *this;
}
