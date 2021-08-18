#ifndef STYLE_PARAMMERGER
#define STYLE_PARAMMERGER

#include <cstddef>
#include <functional>
#include <string>
#include <type_traits>

#include "base/refl/struct.h"
#include "param.h"

namespace Style
{

template <typename Root>
struct ParamMerger : Visitor
{
	std::byte *result;
	std::byte *other;
	Root merged;

	ParamMerger(const Root &base, Root &sub)
	{
		merged = base;
		result = reinterpret_cast<std::byte *>(&merged);
		other = reinterpret_cast<std::byte *>(&sub);
		merged.visit(*this);
	}

	Visitor &operator()(IParam &value, const std::string &) override
	{
		const IParam &otherValue = *reinterpret_cast<const IParam*>
			(other + (reinterpret_cast<std::byte*>(&value) - result));
		if (otherValue) value = otherValue;

		return *this;
	}

	Style::Visitor &operator()(Group &group, const std::string &) override
	{
		group.visit(*this);
		return *this;
	}
};

}

#endif
