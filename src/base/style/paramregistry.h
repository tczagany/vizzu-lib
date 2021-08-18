#ifndef STYLE_PARAMREGISTRY
#define STYLE_PARAMREGISTRY

#include <functional>
#include <map>
#include <string>
#include <type_traits>

#include "base/refl/struct.h"
#include "base/conv/parse.h"
#include "base/conv/tostring.h"

#include "param.h"

namespace Style
{

template <typename Root> class ParamRegistry
{
public:

	struct Accessor
	{
		std::string toString(Root &root)
		{
			auto &value = *reinterpret_cast<IParam *>(
			    reinterpret_cast<std::byte *>(&root) + this->offset);

			return value.toString();
		};

		void fromString(Root &root, const std::string &s) {
			auto &value = *reinterpret_cast<IParam *>(
			    reinterpret_cast<std::byte *>(&root) + this->offset);

			value.fromString(s);
		}

		size_t offset;
	};

	static ParamRegistry &instance()
	{
		static ParamRegistry registry;
		return registry;
	}

	void visit(const auto &visitor)
	{
		for (auto accessor : accessors) visitor(accessor);
	}

	bool hasParam(const std::string &path) const
	{
		return accessors.find(path) != accessors.end();
	}

	void visit(const std::string &path, const auto &visitor)
	{
		visitor(*accessors.at(path));
	}

private:

	struct Proxy : Style::Visitor
	{
		Proxy(ParamRegistry &registry,
		    std::byte *base,
		    const std::string &path = std::string()) :
		    registry(registry),
		    base(base),
		    currentPath(path)
		{}

		Proxy &operator()(IParam &value, const std::string &name)
		{
			auto path = currentPath + (currentPath.empty() ? "" : ".") + name;

			std::byte *ptr = reinterpret_cast<std::byte *>(&value);
			Accessor *accessor = new Accessor();
			accessor->offset = ptr - base;
			registry.accessors.emplace(path, accessor);

			return *this;
		}

		Style::Visitor &operator()(Group &group, const std::string &name)
		{
			auto path = currentPath + (currentPath.empty() ? "" : ".") + name;

			auto proxy = Proxy(registry, base, path);
			group.visit(proxy);

			return *this;
		}

		ParamRegistry &registry;
		std::byte *base;
		std::string currentPath;
	};

	ParamRegistry()
	{
		/* Note: any typesafe solution resulted in at least 50kbyte wasm binary
		   size for 100 param. This implementation takes up to 20kbyte. */
		Root root;
		auto proxy =
		    Proxy(*this, reinterpret_cast<std::byte *>(&root));
		root.visit(proxy);
	}

	std::map<std::string, Accessor*> accessors;
};

}

#endif
