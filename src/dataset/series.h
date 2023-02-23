#ifndef DATASET_SERIES_H
#define DATASET_SERIES_H

#include "types.h"

namespace Vizzu
{
namespace DataSet
{

/**
 * @brief The AbstractSeries intrface
 * @details This class is the base class for all Series
 * implementations. It grants the ability to iterate over the values
 * of a Series and to get the value at a given index.
 */
class AbstractSeries
{
public:
	virtual ~AbstractSeries() {}
	virtual int size() const = 0;
	virtual SeriesId id() const = 0;
	virtual const std::string &name() const = 0;
	virtual ValueType type() const = 0;
	virtual const Value &at(int index) const = 0;
	virtual ValueIterator begin() const = 0;
	virtual ValueIterator end() const = 0;
};

class Series : public AbstractSeries
{
public:
	Series();

	ValueType type() const override;
	int size() const override;
	const Value &at(int index) const override;
	ValueIterator begin() const override;
	ValueIterator end() const override;

protected:
	SeriesId id;
	ValueType vtype;
	SeriesIndexPtr index;
	SeriesPtr data;
	SeriesInfo info;
};

class SeriesIndex
{
public:
	SeriesIndex();

	int size() const;
	void resize(int newSize);
	int &at(int arg);
	ValueIterator begin() const;
	ValueIterator end() const;

protected:
	IndexVector forwardIndices;
	IndexVector backwardIndices;
};

class SeriesContainer
{
friend class DataSet;
public:
	static const SeriesId nullId = 0;

	int size() const;
	SeriesIterator begin() const;
	SeriesIterator end() const;
	SeriesPtr getSeries(SeriesId id) const;
	SeriesPtr getSeries(const char *name) const;

protected:
	static uint32_t nextSeriesId;
	DataSet &dataset;
	SeriesMap series;

	SeriesContainer(DataSet &dataset);
	SeriesId insert(SeriesPtr ptr);
};

}
}

#endif // DATASET_SERIES_H