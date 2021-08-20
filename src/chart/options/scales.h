#ifndef SCALES_H
#define SCALES_H

#include <vector>
#include <array>
#include <functional>

#include "scale.h"

#include "base/type/uniquetype.h"
#include "data/datacube/datacubeoptions.h"

namespace Vizzu
{
namespace Diag
{

class Scales
{
public:

	struct Pos
	{
		Scale::Type scaleId;
		int position;
	};

	Scales();

	bool anyAxisSet() const;
	bool oneAxisSet() const;
	bool bothAxisSet() const;
	bool isEmpty() const;

	bool anyScaleOfType(Scale::Type type) const;

	Data::DataCubeOptions::IndexSet getDimensions() const;
	Data::DataCubeOptions::IndexSet getSeries() const;
	Data::DataCubeOptions::IndexSet getDimensions(const std::vector<Scale::Type> &scaleTypes) const;
	Data::DataCubeOptions::IndexSet getRealSeries(const std::vector<Scale::Type> &scaleTypes) const;
	Data::DataCubeOptions getDataCubeOptions() const;

	const Scale &at(const Scale::Type &type) const;
	Scale &at(const Scale::Type &type);
	Scale::Type getEmptyAxisId() const;

	std::pair<bool, Scale::OptionalContinousIndex>
	addSeries(Scale::Type type,
			  const Data::SeriesIndex &index,
			  std::optional<size_t> pos = std::nullopt);
	bool removeSeries(Scale::Type type, const Data::SeriesIndex &index);
	bool clearSeries(Scale::Type type);

	bool isSeriesUsed(const Data::SeriesIndex &index) const;
	bool isSeriesUsed(const std::vector<Scale::Type> &scaleTypes,
					  const Data::SeriesIndex &index) const;
	size_t count(const Data::SeriesIndex &index) const;
	std::list<Scale::Type> find(const Data::SeriesIndex &index) const;
	std::list<Scales::Pos> findPos(const Data::SeriesIndex &index) const;

	void reset();

	bool operator==(const Scales& other) const;

	void visitAll(
		const std::function<
			void(Scale::Type type, const Scale&)
		> &visitor) const;

private:
	std::array<Scale, Scale::Type::id_size> scales;
};

}
}

#endif
