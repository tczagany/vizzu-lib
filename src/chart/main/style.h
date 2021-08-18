#ifndef STYLE_H
#define STYLE_H

#include "base/anim/interpolated.h"
#include "base/math/fuzzybool.h"
#include "base/geom/rect.h"
#include "base/gfx/color.h"
#include "base/gfx/length.h"
#include "base/gfx/font.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"
#include "base/gfx/colortransform.h"
#include "base/gui/accessories.h"
#include "base/refl/enum.h"
#include "base/style/param.h"
#include "base/text/smartstring.h"
#include "chart/options/scale.h"
#include "chart/generator/colorbuilder.h"

namespace Vizzu
{
namespace Styles
{

template <typename T> using Param = ::Style::Param<T>;

class Enum(Visibility)(hidden, visible);
class Enum(Overflow)(hidden, visible);

struct Padding
{
	Param<Gfx::Length> paddingTop;
	Param<Gfx::Length> paddingRight;
	Param<Gfx::Length> paddingBottom;
	Param<Gfx::Length> paddingLeft;

	GUI::Margin toMargin(const Geom::Size &size) const {
		return { 
			paddingTop->get(size.y),
			paddingLeft->get(size.x),
			paddingBottom->get(size.y),
			paddingRight->get(size.x) 
		};
	}

	Geom::Rect contentRect(const Geom::Rect &rect) const {
		auto margin = toMargin(rect.size);
		return Geom::Rect(
			rect.pos + margin.topLeft(),
			Geom::Size(rect.size - margin.getSpace()).positive());
	}

	void visit(Style::Visitor &visitor) 
	{
		visitor
			(paddingTop, "paddingTop")
			(paddingBottom, "paddingBottom")
			(paddingLeft, "paddingLeft")
			(paddingRight, "paddingRight");
	}
};

struct Font 
{
	Param<::Anim::String> fontFamily;
	Param<::Anim::Interpolated<Gfx::Font::Style>> fontStyle;
	Param<Gfx::Font::Weight> fontWeight;
	Param<Gfx::Length> fontSize;
	const Font *fontParent = nullptr;

	double calculatedSize() const 
	{
		if (fontSize && fontSize->isAbsolute()) 
			return fontSize->get();
		
		if (fontSize && fontParent) 
			return fontSize->get(fontParent->calculatedSize());
		
		if (fontParent)
			return fontParent->calculatedSize();

		throw std::logic_error("internal error: no font parent set");
	}

	std::string calculatedFamily() const 
	{
		if (fontFamily && !fontFamily->values[0].value.empty())
			return fontFamily->values[0].value;
		
		if (fontParent) 
			return fontParent->calculatedFamily();
		
		throw std::logic_error("internal error: no font parent set");
	}

	explicit operator Gfx::Font() const
	{
		return Gfx::Font(calculatedFamily(),
			fontStyle->get(), *fontWeight, calculatedSize());
	}

	void visit(Style::Visitor &visitor)
	{
		visitor
			(fontFamily, "fontFamily")
			(fontStyle, "fontStyle")
			(fontWeight, "fontWeight")
			(fontSize, "fontSize");
	}
};

struct Text
{
	class Enum(TextAlign)(center, left, right);

	Param<Gfx::Color> color;
	Param<::Anim::Interpolated<TextAlign>> textAlign;
	Param<Gfx::Color> backgroundColor;
	Param<::Anim::Interpolated<Overflow>> overflow;
	Param<::Anim::Interpolated<::Text::NumberFormat>> numberFormat;

	void visit(Style::Visitor &visitor)
	{
		visitor
			(color, "color")
			(textAlign, "textAlign")
			(backgroundColor, "backgroundColor")
			(overflow, "overflow")
			(numberFormat, "numberFormat");
	}
};

struct Box
{
	Param<Gfx::Color> backgroundColor;
	Param<Gfx::Color> borderColor;
	Param<double> borderWidth;

	void visit(Style::Visitor &visitor) 
	{
		visitor
		    (backgroundColor, "backgroundColor")
		    (borderColor, "borderColor")
		    (borderWidth, "borderWidth");
	}
};

struct Label : Padding, Font, Text
{
	void visit(Style::Visitor &visitor) 
	{
		Padding::visit(visitor);
		Font::visit(visitor);
		Text::visit(visitor);
	}
};

struct SimpleLabel : Style::Group, Label
{
	void visit(Style::Visitor &visitor) override 
	{
		Label::visit(visitor);
	}
};

struct Tick : Style::Group {
	//todo> top, bottom, both
	class Enum(Position)(outside, inside, center);

	Param<Gfx::Color> color;
	Param<double> lineWidth;
	Param<Gfx::Length> length;
	Param<::Anim::Interpolated<Position>> position;

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(color, "color")
			(lineWidth, "lineWidth")
			(length, "length")
			(position, "position");
	}
};

struct Guide : Style::Group
{
	Param<Gfx::Color> color;
	Param<double> lineWidth;

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(color, "color")
			(lineWidth, "lineWidth");
	}
};

struct Interlacing : Style::Group
{
	Param<Gfx::Color> color;

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(color, "color");
	}
};

struct OrientedLabel : Label
{
	class Enum(Orientation)(normal, tangential, horizontal, vertical);

	Param<::Anim::Interpolated<Orientation>> orientation;
	Param<double> angle;

	void visit(Style::Visitor &visitor)
	{
		Label::visit(visitor);
		visitor
			(orientation, "orientation")
			(angle, "angle");
	}
};

struct AxisLabel : Style::Group, OrientedLabel
{
	class SpecNameEnum(Position)
		(axis, min_edge, max_edge)
		(axis, min-edge, max-edge);
	
	class Enum(Side)(positive, negative);

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;

	void visit(Style::Visitor &visitor) override 
	{
		OrientedLabel::visit(visitor);
		visitor(position, "position")
		       (side, "side");
	}
};

struct AxisTitle : Style::Group, Label
{
	class SpecNameEnum(Position)
		(axis, min_edge, max_edge)
		(axis, min-edge, max-edge);
	
	class Enum(Side)(positive, upon, negative);
	class Enum(VPosition)(begin, middle, end);
	class Enum(VSide)(positive, upon, negative);
	class Enum(Orientation)(horizontal, vertical);

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;
	Param<::Anim::Interpolated<VPosition>> vposition;
	Param<::Anim::Interpolated<VSide>> vside;
	Param<::Anim::Interpolated<Orientation>> orientation;

	void visit(Style::Visitor &visitor) override 
	{
		Label::visit(visitor);
		visitor(position, "position")
		       (side, "side")
		       (vposition, "vposition")
		       (vside, "vside")
		       (orientation, "orientation");
	}
};

struct Axis : Style::Group
{
	Param<Gfx::Color> color;
	AxisTitle title;
	AxisLabel label;
	Tick ticks;
	Guide guides;
	Interlacing interlacing;

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(color, "color")
			(title, "title")
			(label, "label")
			(ticks, "ticks")
			(guides, "guides")
			(interlacing, "interlacing");
	}
};

struct MarkerLabel : Style::Group, OrientedLabel
{
	class Enum(Position)(center, left, right, top, bottom);
	class Enum(Format)(valueFirst, categoriesFirst);

	Param<::Anim::Interpolated<Position>> position;
	Param<Gfx::ColorTransform> filter;
	Param<::Anim::Interpolated<Format>> format;

	void visit(Style::Visitor &visitor) override 
	{
		OrientedLabel::visit(visitor);
		visitor
			(position, "position")
			(filter, "filter")
			(format, "format");
	}
};

struct Marker : Style::Group
{
	class Enum(BorderOpacityMode)(straight, premultiplied);

	Param<double> borderWidth;
	Param<double> borderOpacity;
	Param<::Anim::Interpolated<BorderOpacityMode>> borderOpacityMode;
	Param<double> fillOpacity;
	Guide guides;
	MarkerLabel label;

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(borderWidth, "borderWidth")
			(borderOpacity, "borderOpacity")
			(borderOpacityMode, "borderOpacityMode")
			(fillOpacity, "fillOpacity")
			(guides, "guides")
			(label, "label");
	}
};

struct Legend : Style::Group, Padding, Box
{
	struct Marker : Style::Group 
	{
		class Enum(Type)(circle, square);

		Param<::Anim::Interpolated<Type>> type;
		Param<Gfx::Length> size;

		void visit(Style::Visitor &visitor) override
		{
			visitor
				(type, "type")
				(size, "size");
		}
	};

	Param<Gfx::Length> width;
	SimpleLabel title;
	SimpleLabel label;
	Marker marker;

	void visit(Style::Visitor &visitor) override 
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		visitor
			(width, "width")
		    (title, "title")
		    (label, "label")
		    (marker, "marker");
	}
};

struct Plot : Style::Group, Padding, Box
{
	Marker marker;
	Axis xAxis;
	Axis yAxis;

	const Axis &getAxis(Diag::Scale::Type id) const {
		return id == Diag::Scale::Type::X ? xAxis : yAxis;
	}

	void visit(Style::Visitor &visitor) override 
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		visitor
			(marker, "marker")
			(xAxis, "xAxis")
			(yAxis, "yAxis");
	}
};

struct Tooltip : Style::Group
{
	Param<double> visible;

	void visit(Style::Visitor &visitor) override 
	{ 
		visitor(visible, "visible"); 
	}
};

struct Data : Style::Group
{
	Param<Gfx::ColorGradient> colorGradient;
	Param<Gfx::ColorPalette> colorPalette;
	Param<double> minLightness;
	Param<double> maxLightness;
	Param<double> lineWidth;
	Param<double> lineMinWidth;
	Param<double> lineMaxWidth;
	Param<double> circleMinRadius;
	Param<double> circleMaxRadius;
	Param<double> barMaxPadding;
	Param<double> barPaddingDecrease;
	Param<double> columnMaxPadding;
	Param<double> columnPaddingDecrease;

	Diag::ColorBuilder::LighnessRange lightnessRange() const
	{
		return { *minLightness, *maxLightness};
	}

	Geom::Point maxPadding() const {
		return { *columnMaxPadding, *barMaxPadding };
	}

	Geom::Point paddingDecrease() const
	{
		return {*columnPaddingDecrease, *barPaddingDecrease };
	}

	void visit(Style::Visitor &visitor) override 
	{
		visitor
			(colorGradient, "colorGradient")
			(colorPalette, "colorPalette")
			(minLightness, "minLightness")
			(maxLightness, "maxLightness")
			(lineWidth, "lineWidth")
			(lineMinWidth, "lineMinWidth")
			(lineMaxWidth, "lineMaxWidth")
			(circleMinRadius, "circleMinRadius")
			(circleMaxRadius, "circleMaxRadius")
			(barMaxPadding, "barMaxPadding")
			(barPaddingDecrease, "barPaddingDecrease")
			(columnMaxPadding, "columnMaxPadding")
			(columnPaddingDecrease, "columnPaddingDecrease");
	}
};

struct Chart : Style::Group, Padding, Box, Font
{
	Plot plot;
	Legend legend;
	SimpleLabel title;
	Tooltip tooltip;
	Data data;

	void visit(Style::Visitor &visitor) override 
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		Font::visit(visitor);
		visitor
			(plot, "plot")
			(legend, "legend")
			(title, "title")
			(tooltip, "tooltip")
			(data, "data");
	}

	static Font defaultFont;
	static Chart def();

	void setup() 
	{
		std::vector<Font*> fonts{
			&title,
			&plot.xAxis.title,
			&plot.xAxis.label,
			&plot.yAxis.title,
			&plot.yAxis.label,
			&plot.marker.label,
			&legend.title,
			&legend.label
		};
		fontParent = &defaultFont;
		for (auto font : fonts) font->fontParent = (Font*)this;
	}
};

}
}

#endif
