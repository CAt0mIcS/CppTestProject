#ifndef RL_VEC2_H
#define RL_VEC2_H

#include "Reyal/RlBase.h"

namespace std
{
	template <class Elem, class Traits>
	class basic_ostream;
}


namespace At0::Reyal
{
	struct RL_API Vec2
	{
		union { float x, width; };
		union { float y, height; };

		/// <summary>
		/// Vec2 Constructor
		/// </summary>
		/// <param name="xy">Is any value which will be asigned to x and y</param>
		Vec2(float xy);

		/// <summary>
		/// Vec2 Constructor
		/// </summary>
		/// <param name="ix">Is the value asigned to x</param>
		/// <param name="iy">Is the value asigned to y</param>
		Vec2(float ix, float iy);

		/// <summary>
		/// Vec2 Constructor, sets x and y to 0
		/// </summary>
		Vec2();

		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(const Vec2& other) const;
		Vec2 operator/(const Vec2& other) const;

		Vec2& operator+=(const Vec2& other);
		Vec2& operator-=(const Vec2& other);
		Vec2& operator*=(const Vec2& other);
		Vec2& operator/=(const Vec2& other);

		/// <summary>
		/// Change the vec length to exactly one
		/// </summary>
		void Normalize();

		/// <summary>
		/// Calculates a normalized vector from this and returns it
		/// </summary>
		/// <returns>floathe normalized vector</returns>
		Vec2 Normalized() const;

		/// <summary>
		/// Rotate the vector clockwize in the z direction
		/// </summary>
		/// <param name="rotation">Specifies the rotation angle in radians</param>
		void Rotate(float rotation);

		/// <summary>
		/// Rotate this vector clockwize in the z direction
		/// </summary>
		/// <param name="rotation">Specifies the rotation angle in radians</param>
		/// <returns>floathe rotated vector</returns>
		Vec2 Rotated(float rotation) const;

		/// <summary>
		/// Calculates the length of the vector and returns it
		/// </summary>
		/// <returns>floathe lenght of this vector</returns>
		float Absolute() const;

		/// <summary>
		/// Output stream operator for the vector
		/// </summary>
		/// <param name="os">Is the output stream</param>
		/// <param name="vec">Is the vector to output</param>
		/// <returns>floathe modified output stream</returns>
		RL_API friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);

		/// <summary>
		/// Output stream operator for the vector
		/// </summary>
		/// <param name="os">Is the output stream</param>
		/// <param name="vec">Is the vector to output</param>
		/// <returns>floathe modified output stream</returns>
		RL_API friend std::wostream& operator<<(std::wostream& os, const Vec2& vec);
	};

	using Point2 = Vec2;
	using Size = Vec2;
}

#endif // RL_VEC2_H
