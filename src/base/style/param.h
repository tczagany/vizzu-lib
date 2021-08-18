#ifndef STYLE_PARAM_H
#define STYLE_PARAM_H

#include <string>

#include "base/conv/parse.h"
#include "base/conv/tostring.h"

namespace Style
{

class IParam;
struct Group;

struct Visitor
{
	virtual ~Visitor() {}

	virtual Visitor &operator()(IParam &param, const std::string &name) = 0;
	virtual Visitor &operator()(Group &group, const std::string &name) = 0;
};

struct Group
{
	virtual ~Group() {}
	virtual void visit(Visitor &visitor) = 0;
};

class IParam 
{
public:
	IParam(bool avail = false) : avail(avail) {}
	virtual ~IParam() {}

	virtual IParam &operator*=(double value) = 0;
	virtual IParam &operator+=(const IParam &other) = 0;

	IParam &operator=(const IParam &other) {
		if (other) setFrom(other);
		else reset();
		return *this;
	}

	bool operator==(const IParam &other) const {
		if (avail && other) return isEq(other);
		if (!avail && !other.avail) return true;
		return false;
	}

	std::string toString() const 
	{
		return avail ? convert() : "null";
	}
	
	void fromString(const std::string &s) 
	{
		if (s == "null") reset(); 
		else {
			parse(s);
			avail = true;
		} 
	}

	explicit operator bool() const {
		return avail;
	}

	void reset() {
		avail = false;
		doReset();
	}

protected:
	bool avail;

	virtual void doReset() = 0;
	virtual std::string convert() const = 0;
	virtual void parse(const std::string &) = 0;
	virtual void setFrom(const IParam &other) = 0;
	virtual bool isEq(const IParam &other) const = 0;

	[[noreturn]] void invalidAccess() const {
		throw std::logic_error("value not set");
	}
};

template<typename T> 
class Param : public IParam
{
public:
	typedef T value_type;

	Param() {}

	template<class U> 
	explicit(!std::is_convertible_v<U, T>) 
	Param(U &&value) : 
		IParam(true), 
		value(value)
	{}

	template<class... Args>
	explicit Param(const Args&... args) : 
		IParam(true), 
		value(args...)
	{}

	template<typename Arg>
	Param &operator=(const Arg &value) {
		*this = Param<value_type>(value);
		return *this;
	}

	const value_type &operator*() const {
		if (!avail) invalidAccess();
		return value;
	}

	value_type &operator*() {
		if (!avail) invalidAccess();
		return value;
	}

	value_type* operator->() {
		if (!avail) invalidAccess();
		return &value;
	}

	const value_type* operator->() const {
		if (!avail) invalidAccess();
		return &value;
	}

protected:
	std::string convert() const override
	{
		return Conv::toString(value);
	}

	void parse(const std::string &s) override
	{
		value = Conv::parse<value_type>(s);
	}

	void doReset() override {
		value = value_type();
	}

	void setFrom(const IParam &other) override {
		value = *dynamic_cast<const Param<T>&>(other);
	}

	bool isEq(const IParam &other) const override {
		const auto &otherValue = *dynamic_cast<const Param<T>&>(other);
		return value == otherValue;
	}

	IParam &operator*=(double value) override
	{
		this->value = this->value * value;
		return *this;
	}

	IParam &operator+=(const IParam &other) override
	{
		const auto &otherValue = *dynamic_cast<const Param<T>&>(other);
		value = value + otherValue;
		return *this;
	}

	value_type value;
};

}

#endif
