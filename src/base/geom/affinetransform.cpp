#include "affinetransform.h"

#include "circle.h"
#include "line.h"
#include "polygon.h"
#include "rect.h"

using namespace Geom;

AffineTransform::AffineTransform() : 
	m{
		{ 1,0,0 },
		{ 0,1,0 },
		{ 0,0,1 }
	}
{}

AffineTransform::AffineTransform(
	double m00, double m01, double m02,
	double m10, double m11, double m12,
	double m22) :
	m {
		{ m00, m01, m02 },
		{ m10, m11, m12 },
		{ 0, 0, m22 }
	}
{}

AffineTransform AffineTransform::Offset(Geom::Point offset)
{
	AffineTransform res;
	res.m[0][2] = offset.x;
	res.m[0][2] = offset.y;
	return res;
}

AffineTransform AffineTransform::Scale(double scale)
{
	AffineTransform res;
	res.m[2][2] = scale;
	return res;
}

AffineTransform AffineTransform::Rotation(double angle)
{
	AffineTransform res;
	res.m[0][0] = cos(angle);
	res.m[0][1] = - sin(angle);
	res.m[1][1] = cos(angle);
	res.m[1][0] = sin(angle);
	return res;
}

void AffineTransform::offset(const Geom::Point &offset)
{
	m[0][2] = offset.x / m[2][2];
	m[0][2] = offset.y / m[2][2];
}

AffineTransform AffineTransform::inverse() const
{
	
	auto det = m[0][0]*m[1][1] - m[1][0]*m[0][1];
	auto div = 1.0/m[2][2];
	return AffineTransform(
		m[1][1] / det, 
		-m[0][1] / det, 
		(m[0][1]*m[1][2] - m[0][2]*m[1][1]) / det / div,
		-m[1][0] / det, 
		m[0][0] / det,
		(m[0][2]*m[1][0] - m[0][0]*m[1][2]) / det / div,
		div);
}

bool AffineTransform::transforms() const
{
	return *this != AffineTransform();
}

AffineTransform &AffineTransform::operator*=(double value)
{
	m[0][0] *= value;
	m[0][1] *= value;
	m[0][2] *= value;
	m[1][0] *= value;
	m[1][1] *= value;
	m[1][2] *= value;
	m[2][2] *= value;
	return *this;
}

AffineTransform &AffineTransform::operator+=(const AffineTransform &other)
{
	const auto &o = other;
	m[0][0] += o.m[0][0];
	m[0][1] += o.m[0][1];
	m[0][2] += o.m[0][2];
	m[1][0] += o.m[1][0];
	m[1][1] += o.m[1][1];
	m[1][2] += o.m[1][2];
	m[2][2] += o.m[2][2];
	return *this;
}

AffineTransform &AffineTransform::operator*=(const AffineTransform &other)
{
	const auto &o = other;
	m[0][0] = m[0][0]*o.m[0][0] + m[0][1]*o.m[1][0];
	m[0][1] = m[0][0]*o.m[0][1] + m[0][1]*o.m[1][1];
	m[0][2] = m[0][0]*o.m[0][2] + m[0][1]*o.m[1][2] + m[0][2]*o.m[2][2];
	m[1][0] = m[1][0]*o.m[0][0] + m[1][1]*o.m[1][0];
	m[1][1] = m[1][0]*o.m[0][1] + m[1][1]*o.m[1][1];
	m[1][2] = m[1][0]*o.m[0][2] + m[1][1]*o.m[1][2] + m[1][2]*o.m[2][2];
	m[2][2] = m[2][2]*o.m[2][2];
	return *this;
}

bool AffineTransform::operator==(const AffineTransform &other) const
{
	return m[0][0] == other.m[0][0]
		&& m[0][1] == other.m[0][1]
		&& m[0][2] == other.m[0][2]
		&& m[1][0] == other.m[1][0]
		&& m[1][1] == other.m[1][1]
		&& m[1][2] == other.m[1][2]
		&& m[2][2] == other.m[2][2];
}

Geom::Point AffineTransform::operator*(const Geom::Point &p) const
{
	return Geom::Point(
		p.x * m[0][0] + p.y * m[0][1] + m[0][2],
		p.x * m[1][0] + p.y * m[1][1] + m[1][2]
	) / m[2][2];
}

Geom::Line AffineTransform::operator*(const Geom::Line &original) const
{
	return Geom::Line(*this * original.begin, *this * original.end);
}

Geom::Circle AffineTransform::operator*(const Geom::Circle &original) const
{
	return Geom::Circle(*this * original.center, m[2][2] * original.radius);
}

Geom::Polygon AffineTransform::operator*(const Geom::Polygon &original) const
{
	Geom::Polygon res;
	for (auto point : original.points)
		res.add(*this * point);
	return res;
}
