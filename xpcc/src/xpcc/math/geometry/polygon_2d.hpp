// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
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

#ifndef XPCC__POLYGON_2D_HPP
#define XPCC__POLYGON_2D_HPP

#include "point_set_2d.hpp"
#include "vector2.hpp"

namespace xpcc
{
	// forward declaration
	template <typename T>
	class Circle2D;
	
	template <typename T>
	class LineSegment2D;
	
	template <typename T>
	class Ray2D;
	
	/**
	 * \brief	Polygon
	 *
	 * The Polygon class provides a vector of points. The polygon is
	 * implicit closed, which means the first and the last point are connected.
	 * 
	 * \author	Fabian Greif
	 * \ingroup	geometry
	 */
	template <typename T>
	class Polygon2D : public PointSet2D<T>
	{
		typedef std::size_t SizeType;
	public:
		/**
		 * \brief	Constructs a polygon capable of holding n points
		 */
		Polygon2D(SizeType n);
		
		Polygon2D(const Polygon2D& other);
		
		Polygon2D&
		operator = (const Polygon2D& other);
		
		/// append a point to the polygon
		Polygon2D&
		operator << (const Vector<T, 2>& point);
		
		/**
		 * \brief	Check if a intersection exists
		 * 
		 * \todo	Currently a brute force approach is used here,
		 * 			needs to be optimized
		 */
		bool
		intersects(const Polygon2D& other) const;
		
		/// Check if a intersection exists
		bool
		intersects(const Circle2D<T>& circle) const;
		
		/// Check if a intersection exists
		bool
		intersects(const LineSegment2D<T>& segment) const;
		
		/// Check if a intersection exists
		bool
		intersects(const Ray2D<T>& segment) const;
		
		/**
		 * \brief	Calculate the intersection point(s)
		 */
		bool
		getIntersections(const LineSegment2D<T>& segment,
				PointSet2D<T>& intersectionPoints) const;
		
		/**
		 * Check if the point is contained inside the area of the polygon.
		 * 
		 * The borders of the polygon are included in the area of the polygon.
		 * 
		 * @warning Only works for convex polygons! 
		 * 
		 * @return  `true` if point is contained in the area, `false` if
		 *          it is outside.
		 */
		bool
		isInside(const Vector<T, 2>& point);
	};
}

#include "circle_2d.hpp"
#include "line_segment_2d.hpp"
#include "ray_2d.hpp"

#include "polygon_2d_impl.hpp"

#endif // XPCC__POLYGON_2D_HPP
