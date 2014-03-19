// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC__VECTOR3_HPP
	#error	"Don't include this file directly, use 'vector3.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector() :
	x(),
	y(),
	z()
{
}

// ----------------------------------------------------------------------------
template<typename T>
template<typename U>
xpcc::Vector<T, 3>::Vector(const U *array) :
	x(array[0]),
	y(array[1]),
	z(array[2])
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(T inX, T inY, T inZ) :
	x(inX),
	y(inY),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 1> &inX,
		const xpcc::Vector<T, 1> &inY,
		const xpcc::Vector<T, 1> &inZ) :
	x(inX.x),
	y(inY.x),
	z(inZ.x)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const T &inX,
		const xpcc::Vector<T, 1> &inY,
		const xpcc::Vector<T, 1> &inZ) :
	x(inX),
	y(inY.x),
	z(inZ.x)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 1> &inX, const T &inY, const xpcc::Vector<T, 1> &inZ) :
	x(inX.x),
	y(inY),
	z(inZ.x)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 1> &inX, const xpcc::Vector<T, 1> &inY, const T &inZ)
:
	x(inX.x),
	y(inY.x),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 1> &inX, const T &inY, const T &inZ) :
	x(inX.x),
	y(inY),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const T &inX, const xpcc::Vector<T, 1> &inY, const T &inZ) :
	x(inX),
	y(inY.x),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const T &inX, const T &inY, const xpcc::Vector<T, 1> &inZ) :
	x(inX),
	y(inY),
	z(inZ.x)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 2> &inXY, const T &inZ) :
	x(inXY.x),
	y(inXY.y),
	z(inZ)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 2> &inXY, const xpcc::Vector<T, 1> &inZ) :
	x(inXY.x),
	y(inXY.y),
	z(inZ.x)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const T &inX, const xpcc::Vector<T, 2> &inYZ) :
	x(inX),
	y(inYZ.x),
	z(inYZ.y)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<T, 1> &inX, const xpcc::Vector<T, 2> &inYZ) :
	x(inX.x),
	y(inYZ.x),
	z(inYZ.y)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(T inVal) :
	x(inVal),
	y(inVal),
	z(inVal)
{
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>::Vector(const xpcc::Matrix<T, 3, 1> &rhs) :
	x(reinterpret_cast<const T*>(&rhs)[0]),
	y(reinterpret_cast<const T*>(&rhs)[1]),
	z(reinterpret_cast<const T*>(&rhs)[2])
{
}

// ----------------------------------------------------------------------------
template<typename T>
template<typename U>
xpcc::Vector<T, 3>::Vector(const xpcc::Vector<U, 3> &rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z)
{
}

// ----------------------------------------------------------------------------
template<typename T>
void
xpcc::Vector<T, 3>::set(const T& x, const T& y, const T& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// ----------------------------------------------------------------------------
template<typename T>
void
xpcc::Vector<T, 3>::setX(const T& value)
{
	this->x = value;
}

template<typename T>
void
xpcc::Vector<T, 3>::setY(const T& value)
{
	this->y = value;
}

template<typename T>
void
xpcc::Vector<T, 3>::setZ(const T& value)
{
	this->z = value;
}

// ----------------------------------------------------------------------------
template<typename T>
const T&
xpcc::Vector<T, 3>::getX() const
{
	return this->x;
}

template<typename T>
const T&
xpcc::Vector<T, 3>::getY() const
{
	return this->y;
}

template<typename T>
const T&
xpcc::Vector<T, 3>::getZ() const
{
	return this->z;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator = (const xpcc::Matrix<T, 3, 1> &rhs)
{
	x = reinterpret_cast<const T*>(&rhs)[0];
	y = reinterpret_cast<const T*>(&rhs)[1];
	z = reinterpret_cast<const T*>(&rhs)[2];
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator = (const Vector &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator == (const Vector &rhs) const
{
	return (rhs.x == x) && (rhs.y == y) && (rhs.z == z);
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator != (const Vector &rhs) const
{
	return (rhs.x != x) || (rhs.y != y) || (rhs.z != z);
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator < (const Vector &rhs) const
{
	return (x < rhs.x) || ((x == rhs.x) && ((y < rhs.y) || ((y == rhs.y) && (z < rhs.z))));
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator <= (const Vector &rhs) const
{
	return (x < rhs.x) || ((x == rhs.x) && ((y < rhs.y) || ((y == rhs.y) && (z <= rhs.z))));
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator > (const Vector &rhs) const
{
	return (x > rhs.x) || ((x == rhs.x) && ((y > rhs.y) || ((y == rhs.y) && (z > rhs.z))));
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::operator >= (const Vector &rhs) const
{
	return (x > rhs.x) || ((x == rhs.x) && ((y > rhs.y) || ((y == rhs.y) && (z >= rhs.z))));
}

// ----------------------------------------------------------------------------
template<typename T>
const T&
xpcc::Vector<T, 3>::operator [] (uint8_t index) const
{
	return reinterpret_cast<const T*>(this)[index];
}

// ----------------------------------------------------------------------------
template<typename T>
T&
xpcc::Vector<T, 3>::operator [] (uint8_t index)
{
	return reinterpret_cast<T*>(this)[index];
}


// ----------------------------------------------------------------------------
template<typename T>
T*
xpcc::Vector<T, 3>::ptr()
{
	return reinterpret_cast<T*>(this);
}

// ----------------------------------------------------------------------------
template<typename T>
const T*
xpcc::Vector<T, 3>::ptr() const
{
	return reinterpret_cast<const T*>(this);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator + (const xpcc::Vector<T, 3> &rhs) const
{
	return xpcc::Vector<T, 3>(x+rhs.x, y+rhs.y, z+rhs.z);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator - (const xpcc::Vector<T, 3> &rhs) const
{
	return xpcc::Vector<T, 3>(x-rhs.x, y-rhs.y, z-rhs.z);
}

// ----------------------------------------------------------------------------
template<typename T>
T
xpcc::Vector<T, 3>::operator * (const xpcc::Vector<T, 3> &rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator ^ (const xpcc::Vector<T, 3> &rhs) const
{
	return xpcc::Vector<T, 3>(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator * (const T &rhs) const
{
	return xpcc::Vector<T, 3>(x*rhs, y*rhs, z*rhs);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator / (const T &rhs) const
{
	return xpcc::Vector<T, 3>(x/rhs, y/rhs, z/rhs);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator += (const xpcc::Vector<T, 3> &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator -= (const xpcc::Vector<T, 3> &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator *= (const T &rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>&
xpcc::Vector<T, 3>::operator /= (const T &rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::operator - () const
{
	return xpcc::Vector<T, 3>(-x, -y, -z);
}

// ----------------------------------------------------------------------------
template<typename T>
float
xpcc::Vector<T, 3>::getLength() const
{
	return std::sqrt(getLengthSquared());
}

// ----------------------------------------------------------------------------
template<typename T>
float
xpcc::Vector<T, 3>::getLengthSquared() const
{
	return x*x+y*y+z*z;
}

// ----------------------------------------------------------------------------
template<typename T>
void
xpcc::Vector<T, 3>::scale(float newLength)
{
	*this = scaled(newLength);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::scaled(float newLength) const
{
	float scale = newLength/getLength();
	return *this * scale;
}

// ----------------------------------------------------------------------------
template<typename T>
void
xpcc::Vector<T, 3>::normalize()
{
	scale(1.0);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Vector<T, 3>
xpcc::Vector<T, 3>::normalized() const
{
	return scaled(1.0f);
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Matrix<T, 3, 1>&
xpcc::Vector<T, 3>::asMatrix()
{
	return *(xpcc::Matrix<T, 3, 1>*)this;
}

// ----------------------------------------------------------------------------
template<typename T>
const xpcc::Matrix<T, 3, 1>&
xpcc::Vector<T, 3>::asMatrix() const
{
	return *(xpcc::Matrix<T, 3, 1>*)this;
}

// ----------------------------------------------------------------------------
template<typename T>
xpcc::Matrix<T, 1, 3>&
xpcc::Vector<T, 3>::asTransposedMatrix()
{
	return *(xpcc::Matrix<T, 1, 3>*)this;
}

// ----------------------------------------------------------------------------
template<typename T>
const xpcc::Matrix<T, 1, 3>&
xpcc::Vector<T, 3>::asTransposedMatrix() const
{
	return *(xpcc::Matrix<T, 1, 3>*)this;
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::hasNan() const
{
	return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

// ----------------------------------------------------------------------------
template<typename T>
bool
xpcc::Vector<T, 3>::hasInf() const
{
	return std::isinf(x) || std::isinf(y) || std::isinf(z);
}

// ----------------------------------------------------------------------------
//template<typename T>
//static inline xpcc::Vector<T,3> operator * (const T &lhs, const xpcc::Vector<T,3> &rhs)
//{
//	return rhs * lhs;
//}
//
//
//// ----------------------------------------------------------------------------
//template<typename T>
//static inline xpcc::Vector<T,3> operator * (const xpcc::Matrix<T, 3, 3> &lhs, const xpcc::Vector<T,3> &rhs)
//{
//	return lhs * rhs.asTMatrix();
//}
