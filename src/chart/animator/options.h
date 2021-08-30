#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <array>
#include <list>
#include <optional>

#include "base/anim/options.h"
#include "base/anim/control.h"
#include "base/refl/enum.h"

namespace Vizzu
{
namespace Anim
{

class Enum(SectionId)
	(style,title,legend,show,hide,color,coordSystem,geometry,y,x);

class Options
{
public:
	Options();

	struct Section {
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
		void set(const std::string &param, const std::string &value);
		std::string get(const std::string &param);
	};

	Section all;
	::Anim::Control::PlayState playState;
	std::array<Section, SectionId::EnumInfo::count()> sections;

	static std::list<std::string> listParams();

	void set(const std::string &path, const std::string &value);
	std::string get(const std::string &path);

	const Section &get(SectionId sectionId) const;
};

}
}

#endif
