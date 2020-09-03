#pragma once

#include "pch.h"


	struct NPoint
	{
		int x, y;

		//NPoint
		NPoint operator+(const NPoint& other)
		{
			return { x + other.x, y + other.y };
		}

		NPoint& operator+=(const NPoint& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		NPoint operator-(const NPoint& other)
		{
			return { x - other.x, y - other.y };

		}

		NPoint& operator-=(const NPoint& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		NPoint& operator*=(const NPoint& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		NPoint operator*(const NPoint& other)
		{
			return { x * other.x, y * other.y };

		}

		NPoint operator/(const NPoint& other)
		{
			return { x / other.x, y / other.y };

		}

		NPoint& operator/=(const NPoint& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		//int
		NPoint operator+(const int val)
		{
			return { x + val, y + val };

		}

		NPoint& operator+=(const int val)
		{
			x += val;
			y += val;
			return *this;
		}

		NPoint operator-(const int val)
		{
			return { x - val, y - val };
		}

		NPoint& operator-=(const int val)
		{
			x += val;
			y += val;
			return *this;
		}

		NPoint& operator*=(const int val)
		{
			x *= val;
			y *= val;
			return *this;
		}

		NPoint operator*(const int val)
		{
			return { x * val, y * val };
		}

		NPoint operator/(const int val)
		{
			return { x / val, y / val };
		}

		NPoint& operator/=(const int val)
		{
			x /= val;
			y /= val;
			return *this;
		}

		NPoint& operator=(const int val)
		{
			x = val;
			y = val;
			return *this;
		}

		//POINTS
		NPoint operator+(const POINTS val)
		{
			return { x + val.x, y + val.y };
		}

		NPoint& operator+=(const POINTS val)
		{
			x += val.x;
			y += val.y;
			return *this;
		}

		NPoint operator-(const POINTS val)
		{
			return { x - val.x, y - val.y };
		}

		NPoint& operator-=(const POINTS val)
		{
			x += val.x;
			y += val.y;
			return *this;
		}

		NPoint& operator*=(const POINTS val)
		{
			x *= val.x;
			y *= val.y;
			return *this;
		}

		NPoint operator*(const POINTS val)
		{
			return { x * val.x, y * val.y };
		}

		NPoint operator/(const POINTS val)
		{
			return { x / val.x, y / val.y };
		}

		NPoint& operator/=(const POINTS val)
		{
			x /= val.x;
			y /= val.y;
			return *this;
		}

		NPoint& operator=(const POINTS val)
		{
			x = val.x;
			y = val.y;
			return *this;
		}
	};


	struct NSize
	{
		int width, height;

		//NSize
		NSize operator+(const NSize& other)
		{
			return { width + other.width, height + other.height };
		}

		NSize& operator+=(const NSize& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		NSize operator-(const NSize& other)
		{
			return { width - other.width, height - other.height };
		}

		NSize& operator-=(const NSize& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		NSize& operator*=(const NSize& other)
		{
			width *= other.width;
			height *= other.height;
			return *this;
		}

		NSize operator*(const NSize& other)
		{
			return { width * other.width, height * other.height };
		}

		NSize operator/(const NSize& other)
		{
			return { width / other.width, height / other.height };
		}

		NSize& operator/=(const NSize& other)
		{
			width /= other.width;
			height /= other.height;
			return *this;
		}

		//int
		NSize operator+(const int val)
		{
			return { width + val, height + val };
		}

		NSize& operator+=(const int val)
		{
			width += val;
			height += val;
			return *this;
		}

		NSize operator-(const int val)
		{
			return { width - val, height - val };
		}

		NSize& operator-=(const int val)
		{
			width -= val;
			height -= val;
			return *this;
		}

		NSize& operator*=(const int val)
		{
			width *= val;
			height *= val;
			return *this;
		}

		NSize operator*(const int val)
		{
			return { width * val, height * val };
		}

		NSize operator/(const int val)
		{
			return { width / val, height / val };
		}

		NSize& operator/=(const int val)
		{
			width /= val;
			height /= val;
			return *this;
		}
	};

