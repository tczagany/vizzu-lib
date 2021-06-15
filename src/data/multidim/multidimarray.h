#ifndef MULTIDIMARRAY_H
#define MULTIDIMARRAY_H

#include <functional>
#include <map>
#include <vector>

#include "multidimindex.h"

namespace Vizzu
{
namespace Data
{
namespace MultiDim
{

template <typename T> class Array;

template <typename T>
class Iterator
{
public:
	Iterator(const Array<T> &parent, bool end = false);
	Iterator &operator++();

	bool operator==(const Iterator &other) const
	{ return ref == other.ref; }

	const T &operator*() { return ref->second; }
	const MultiIndex &getIndex() const { return index; }

private:
	MultiIndex index;
	typename std::map<size_t, T>::const_iterator ref;
	const Array<T> &parent;
};

template <typename T>
class Array
{
	friend class Iterator<T>;
public:
	Array() {}
	Array(const MultiIndex &sizes, const T &def = T());

	T &operator[](const MultiIndex &index);
	const T &at(const MultiIndex &index) const;

	Iterator<T> begin() const { return Iterator<T>(*this, false); }
	Iterator<T> end() const { return Iterator<T>(*this, true); }

	size_t unfoldSubSliceIndex(const SubSliceIndex &) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
					   const std::function<void(const T&)> &visitor) const;

	MultiIndex maxIndex() const;

	bool empty() const;
	size_t unfoldedSize() const;

private:
	MultiIndex sizes;
	std::map<size_t, T> values;
	T def;

	size_t unfoldedIndex(const MultiIndex &index) const;
	MultiIndex foldedIndex(size_t unfoldedIndex) const;

	void visitSubSlice(const SubSliceIndex &subSliceIndex,
					   const std::function<void(const T&)> &visitor,
					   MultiIndex &multiIndex) const;
};

}

}
}

#endif

#include "multidimarray.impl.h"
