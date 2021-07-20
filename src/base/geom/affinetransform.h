#ifndef GEOM_AFFINETRANSFORM
#define GEOM_AFFINETRANSFORM

#include "point.h"
#include <array>

namespace Geom
{

struct Size;
struct Rect;
class Circle;
struct Line;
class Polygon;

class AffineTransform
{
public:
	double m[3][3];

	AffineTransform();
	AffineTransform(
		double m00, double m01, double m02,
		double m10, double m11, double m12,
		double m22 = 1);
	static AffineTransform Offset(Geom::Point offset);
	static AffineTransform Scale(double scale);
	static AffineTransform Rotation(double angle);

	AffineTransform(Geom::Rect from, Geom::Rect to);

	AffineTransform inverse() const;
	bool transforms() const;

	void offset(const Geom::Point &offset);

	AffineTransform &operator+=(const AffineTransform &other);
	AffineTransform &operator*=(const AffineTransform &other);
	AffineTransform &operator*=(double value);
	bool operator==(const AffineTransform &other) const;

	Geom::Point operator*(const Geom::Point &original) const;
	Geom::Line operator*(const Geom::Line &original) const;
	Geom::Circle operator*(const Geom::Circle &original) const;
	Geom::Polygon operator*(const Geom::Polygon &original) const;
};

}

#endif
