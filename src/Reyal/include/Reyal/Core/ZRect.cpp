#include "rlpch.h"
#include "Reyal/Core/ZRect.h"

#include <iostream>

namespace At0::Reyal
{
	ZRect::ZRect(float ltrb)
		: left(ltrb), top(ltrb), right(ltrb), bottom(ltrb)
	{

	}

	ZRect::ZRect(float ileft, float itop, float iright, float ibottom)
		: left(ileft), top(itop), right(iright), bottom(ibottom)
	{

	}

	ZRect::ZRect()
		: left(0), top(0), right(0), bottom(0)
	{

	}

	ZRect ZRect::operator+(const ZRect& other) const
	{
		ZRect temp(*this);
		temp += other;
		return temp;
	}

	ZRect ZRect::operator-(const ZRect& other) const
	{
		ZRect temp(*this);
		temp -= other;
		return temp;
	}

	ZRect ZRect::operator*(const ZRect& other) const
	{
		ZRect temp(*this);
		temp *= other;
		return temp;
	}

	ZRect ZRect::operator/(const ZRect& other) const
	{
		ZRect temp(*this);
		temp /= other;
		return temp;
	}


	ZRect& ZRect::operator+=(const ZRect& other)
	{
		left += other.left;
		top += other.top;
		right += other.right;
		bottom += other.bottom;
		return *this;
	}

	ZRect& ZRect::operator-=(const ZRect& other)
	{
		left -= other.left;
		top -= other.top;
		right -= other.right;
		bottom -= other.bottom;
		return *this;
	}

	ZRect& ZRect::operator*=(const ZRect& other)
	{
		left *= other.left;
		top *= other.top;
		right *= other.right;
		bottom *= other.bottom;
		return *this;
	}

	ZRect& ZRect::operator/=(const ZRect& other)
	{
		left /= other.left;
		top /= other.top;
		right /= other.right;
		bottom /= other.bottom;
		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const ZRect& rect)
	{
		os << "{left=" << rect.left << ", top=" << rect.top << ", right=" << rect.right << ", bottom=" << rect.bottom << "}";
		return os;
	}

}
