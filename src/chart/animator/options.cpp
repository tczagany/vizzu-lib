#include "options.h"

#include "base/conv/parse.h"
#include "base/conv/tostring.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

using namespace std::literals::chrono_literals;

Options::Options()
{
	playState = ::Anim::Control::PlayState::running;
}

void Options::Section::set(const std::string &param, const std::string &value)
{
	if (param == "easing") easing = ::Anim::Easing(value);
	else if (param == "delay") delay = ::Anim::Duration(value);
	else if (param == "duration") duration = ::Anim::Duration(value);
	else throw std::logic_error("invalid animation parameter: " + param);
}

std::string Options::Section::get(const std::string &param)
{
	//todo: easing 2 string conversion missing
	if (param == "easing") return "null";
	else if (param == "delay") return Conv::toString(delay);
	else if (param == "duration") return Conv::toString(duration);
	else throw std::logic_error("invalid animation parameter: " + param);
}

void Options::set(const std::string &path,
	const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');

	if (parts.size() == 1) 
	{
		if (path == "playState") {
			playState = ::Anim::Control::PlayState(value);
		}
		else all.set(path, value);
	}
	else if (parts.size() == 2)
	{
		auto sectionId = SectionId(parts[0]);
		sections.at((int)sectionId).set(parts[1], value);
	}
	else throw std::logic_error("invalid animation option: " + path);
}

const Options::Section &Options::get(SectionId sectionId) const
{
	return sections.at((int)sectionId);
}

std::string Options::get(const std::string &path)
{
	auto parts = Text::SmartString::split(path, '.');

	if (parts.size() == 1) 
	{
		if (path == "playState") {
			return Conv::toString(playState);
		}
		else return all.get(path);
	}
	else if (parts.size() == 2)
	{
		auto sectionId = SectionId(parts[0]);
		return sections.at((int)sectionId).get(parts[1]);
	}
	else throw std::logic_error("invalid animation option: " + path);
}

std::list<std::string> Options::listParams()
{
	std::list<std::string> res;
	return res;
}
