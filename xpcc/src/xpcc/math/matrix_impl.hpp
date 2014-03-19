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

#ifndef XPCC__MATRIX_HPP
#	error	"Don't include this file directly, use 'matrix.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::Matrix()
{
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::Matrix(const T *data)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] = data[i];
	}
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS> 
xpcc::Matrix<T, ROWS, COLUMNS>::Matrix(const Matrix &m)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] = m.element[i];
	}
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
template<typename U>
xpcc::Matrix<T, ROWS, COLUMNS>& 
xpcc::Matrix<T, ROWS, COLUMNS>::operator = (const xpcc::Matrix<U, ROWS, COLUMNS> &m)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] = m.element[i];
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
const xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::identityMatrix()
{
	static xpcc::Matrix<T, ROWS, COLUMNS> matrix = zeroMatrix();
	static bool hasIdentityMatrix = false;
	
	if (!hasIdentityMatrix)
	{
		if (ROWS < COLUMNS)
		{
			for (uint_fast8_t i = 0; i < ROWS; ++i) {
				matrix[i][i] = 1;
			}
		}
		else
		{
			for (uint_fast8_t i = 0; i < COLUMNS; ++i) {
				matrix[i][i] = 1;
			}
		}
		
		hasIdentityMatrix = true;
	}
	
	return matrix;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
const xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::zeroMatrix()
{
	static xpcc::Matrix<T, ROWS, COLUMNS> matrix;
	static bool hasZeroMatrix = false;
	
	if (!hasZeroMatrix)
	{
		memset(matrix.ptr(), 0, matrix.getSize());
		hasZeroMatrix = true;
	}
	
	return matrix;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
bool
xpcc::Matrix<T, ROWS, COLUMNS>::operator == (const xpcc::Matrix<T, ROWS, COLUMNS> &m) const
{
	return memcmp(element, m.element, getSize()) == 0;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
bool
xpcc::Matrix<T, ROWS, COLUMNS>::operator != (const xpcc::Matrix<T, ROWS, COLUMNS> &m) const
{
	return memcmp(element, m.element, getSize()) != 0;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, 1, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::getRow(uint8_t index) const
{
	return subMatrix<1, COLUMNS>(index, 0);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, 1>
xpcc::Matrix<T, ROWS, COLUMNS>::getColumn(uint8_t index) const
{
	return subMatrix<ROWS, 1>(0, index);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
T*
xpcc::Matrix<T, ROWS, COLUMNS>::operator [] (uint8_t row)
{
	return &element[row * COLUMNS];
}

template<typename T, uint8_t ROWS, uint8_t COLUMNS>
const T*
xpcc::Matrix<T, ROWS, COLUMNS>::operator [] (uint8_t row) const
{
	return &element[row * COLUMNS];
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
uint8_t
xpcc::Matrix<T, ROWS, COLUMNS>::getNumberOfRows() const	
{
	return ROWS;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
uint8_t
xpcc::Matrix<T, ROWS, COLUMNS>::getNumberOfColumns() const	
{
	return COLUMNS;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
const T*
xpcc::Matrix<T, ROWS, COLUMNS>::ptr() const
{
	return element;
}

template<typename T, uint8_t ROWS, uint8_t COLUMNS>
T*
xpcc::Matrix<T, ROWS, COLUMNS>::ptr()
{
	return element;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator - ()
{
	xpcc::Matrix<T, ROWS, COLUMNS> m;
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		m.element[i] = -this->element[i];
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator - (const xpcc::Matrix<T, ROWS, COLUMNS> &rhs) const
{
	xpcc::Matrix<T, ROWS, COLUMNS> m;
	
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		m.element[i] = element[i] - rhs.element[i];
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator + (const xpcc::Matrix<T, ROWS, COLUMNS> &rhs) const
{
	xpcc::Matrix<T, ROWS, COLUMNS> m;
	
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		m.element[i] = element[i] + rhs.element[i];
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::operator += (const xpcc::Matrix<T, ROWS, COLUMNS> &rhs)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] += rhs.element[i];
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::operator -= (const xpcc::Matrix<T, ROWS, COLUMNS> &rhs)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] -= rhs.element[i];
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
template<uint8_t RHSCOL>
xpcc::Matrix<T, ROWS, ROWS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator * (const Matrix<T, COLUMNS, RHSCOL> &rhs) const
{
	xpcc::Matrix<T, ROWS, ROWS> m;
	
	for (uint_fast8_t i = 0; i < ROWS; ++i)
	{
		for (uint_fast8_t j = 0; j < RHSCOL; ++j)
		{
			m[j][i] = element[j * COLUMNS] * rhs[0][i];
			for (uint_fast8_t x = 1; x < COLUMNS; ++x)
			{
				m[j][i] += element[j * COLUMNS + x] * rhs[x][i];
			}
		}
	}
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator *= (const xpcc::Matrix<T, ROWS, COLUMNS> &rhs)
{
	(*this) = (*this) * rhs;
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator * (const T &rhs) const
{
	xpcc::Matrix<T, ROWS, COLUMNS> m;
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		m.element[i] = element[i] * rhs;
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::operator *= (const T &rhs)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] *= rhs;
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::operator / (const T &rhs) const
{
	xpcc::Matrix<T, ROWS, COLUMNS> m;
	
	float oneOverRhs = 1.0f / rhs;
	
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		m.element[i] = element[i] * oneOverRhs;
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::operator /= (const T &rhs)
{
	float oneOverRhs = 1.0f / rhs;
	
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] *= oneOverRhs;
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, COLUMNS, ROWS>
xpcc::Matrix<T, ROWS, COLUMNS>::asTransposed() const
{
	xpcc::Matrix<T, COLUMNS, ROWS> m;
	
	for (uint_fast8_t i = 0; i < ROWS; ++i) {
		for (uint_fast8_t j = 0; j < COLUMNS; ++j) {
			m.element[j * ROWS + i] = element[i * COLUMNS + j];
		}
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
void
xpcc::Matrix<T, ROWS, COLUMNS>::transpose()
{
	XPCC__STATIC_ASSERT(ROWS == COLUMNS, "transpose() only possible for square matrices");
	
	*this = asTransposed();
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
inline T
xpcc::Matrix<T, ROWS, COLUMNS>::determinant() const
{
	XPCC__STATIC_ASSERT(ROWS == COLUMNS, "determinant() only possible for square matrices");
	
	return xpcc::determinant(*this);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
bool
xpcc::Matrix<T, ROWS, COLUMNS>::hasNan() const
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		if (isnan(element[i])) {
			return true;
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
bool
xpcc::Matrix<T, ROWS, COLUMNS>::hasInf() const
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		if (isinf(element[i])) {
			return true;
		}
	}
	
	return false;
}
 
// ----------------------------------------------------------------------------
template<typename T, uint8_t WIDTH, uint8_t HEIGHT>
xpcc::Matrix<T, WIDTH, HEIGHT>
operator * (int8_t lhs, const xpcc::Matrix<T, WIDTH, HEIGHT> &m)
{
	return m * lhs;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t WIDTH, uint8_t HEIGHT>
xpcc::Matrix<T, WIDTH, HEIGHT>
operator * (float lhs, const xpcc::Matrix<T, WIDTH, HEIGHT> &m)
{
	return m * lhs;
}

// ----------------------------------------------------------------------------
/*template<typename T, uint8_t ROWS, uint8_t COLUMNS>
void
xpcc::Matrix<T, ROWS, COLUMNS>::inverse()
{
	*this = inversed();
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::inversed() const
{
	xpcc::Matrix<T, ROWS, COLUMNS> inverse;
	xpcc::MatrixUtils::luInverse(*this, &inverse, true);
	return inverse;
}*/

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
size_t
xpcc::Matrix<T, ROWS, COLUMNS>::getSize() const
{
	return getNumberOfElements() * sizeof(T);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
uint8_t
xpcc::Matrix<T, ROWS, COLUMNS>::getNumberOfElements() const
{
	return ROWS * COLUMNS;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
template <uint8_t MR, uint8_t MC>
xpcc::Matrix<T, MR, MC>
xpcc::Matrix<T, ROWS, COLUMNS>::subMatrix(uint8_t row, uint8_t column) const
{
	XPCC__STATIC_ASSERT(MR <= ROWS, "sub matrix must be smaller than the original");
	XPCC__STATIC_ASSERT(MC <= COLUMNS, "sub matrix must be smaller than the original");
	
	Matrix<T, MR, MC> sub;
	for (uint_fast8_t i = 0; i < MR; ++i) {
		for (uint_fast8_t j = 0; j < MC; ++j) {
			sub[i][j] = (*this)[i + row][j + column];
		}
	}
	
	return sub;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS> template<typename U>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::replace(const U *data)
{
	for (uint_fast8_t i = 0; i < getNumberOfElements(); ++i) {
		element[i] = data[i];
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
template <uint8_t MR, uint8_t MC>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::replace(uint8_t row, uint8_t column, const xpcc::Matrix<T, MR, MC> &m)
{
	XPCC__STATIC_ASSERT(MR <= ROWS, "replacement matrix can't be larger than the original");
	XPCC__STATIC_ASSERT(MC <= COLUMNS, "replacement matrix can't be larger than the original");
	
	for (uint_fast8_t i = 0; i < MR && (i + row) < ROWS; ++i)
	{
		for (uint_fast8_t j = 0; j < MC && (j + column) < COLUMNS; ++j)
		{
			element[(i + row) * COLUMNS + (j + column)] = m[i][j];
		}
	}
	
	return *this;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::replaceRow(uint8_t index, const xpcc::Matrix<T, 1, COLUMNS> &m)
{
	return replace(index, 0, m);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>&
xpcc::Matrix<T, ROWS, COLUMNS>::replaceColumn(uint8_t index, const xpcc::Matrix<T, ROWS, 1> &m)
{
	return replace(0, index, m);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS+1, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::addRow(uint8_t index, const xpcc::Matrix<T, 1, COLUMNS> &r) const
{
	xpcc::Matrix<T, ROWS+1, COLUMNS> m;
	uint_fast8_t i = 0, ri = 0;
	
	for (; i < index; ++i) {
		m.replaceRow(ri++, getRow(i));
	}
	m.replaceRow(ri++, r);
	for (; i < ROWS+1; ++i) {
		m.replaceRow(ri++, getRow(i));
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS+1>
xpcc::Matrix<T, ROWS, COLUMNS>::addColumn(uint8_t index, const xpcc::Matrix<T, ROWS, 1> &c) const
{
	xpcc::Matrix<T, ROWS, COLUMNS+1> m;
	uint_fast8_t i = 0, ci = 0;
	
	for (; i < index; ++i) {
		m.replaceColumn(ci++, getColumn(i));
	}
	m.replaceColumn(ci++, c);
	for (; i < COLUMNS+1; ++i) {
		m.replaceColumn(ci++, getColumn(i));
	}
	
	return m;
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS-1, COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS>::removeRow(uint8_t index ) const
{
	if (index == 0)
	{
		return subMatrix<ROWS-1, COLUMNS>(1, 0);
	}
	else if (index == (ROWS - 1))
	{
		return subMatrix<ROWS-1, COLUMNS>(0, 0);
	}
	else
	{
		Matrix<T, ROWS-1, COLUMNS> m;
		uint_fast8_t i = 0, ri = 0;
		
		for (; i < index; ++i) {
			m.replaceRow(ri++, getRow(i));
		}
		++i; // skip one row
		for (; i < ROWS; ++i) {
			m.replaceRow(ri++, getRow(i));
		}
		
		return m;
	}
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::Matrix<T, ROWS, COLUMNS-1>
xpcc::Matrix<T, ROWS, COLUMNS>::removeColumn(uint8_t index) const
{
	if (index == 0)
	{
		return subMatrix<ROWS, COLUMNS-1>(0, 1);
	}
	else if (index == (COLUMNS - 1))
	{
		return subMatrix<ROWS, COLUMNS-1>(0, 0);
	}
	else
	{
		Matrix<T, ROWS, COLUMNS-1> m;
		uint_fast8_t i = 0, ci = 0;
		
		for (; i < index; ++i) {
			m.replaceColumn(ci++, getColumn(i));
		}
		++i; // skip one column
		for (; i < COLUMNS; ++i) {
			m.replaceColumn(ci++, getColumn(i));
		}
		
		return m;
	}
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t ROWS, uint8_t COLUMNS>
xpcc::IOStream&
xpcc::operator << (xpcc::IOStream& os, const xpcc::Matrix<T, ROWS, COLUMNS> &m)
{
	os << "{ ";
	
	for (uint_fast8_t i = 0; i < ROWS; ++i)
	{
		os << "{ ";
		for (uint_fast8_t j = 0; j < COLUMNS; ++j)
		{
			os << m.element[i * COLUMNS + j];
			if (j < COLUMNS-1)
			{
				os << ", ";
			}
		}
		os << " }";
		
		if (i < ROWS-1)
		{
			os << ", \n";
		}
	}
	os << " }";
	return os;
}

// ----------------------------------------------------------------------------
template<typename T>
T
xpcc::determinant(const xpcc::Matrix<T, 1, 1> &m)
{
	return m[0][0];
}

// ----------------------------------------------------------------------------
template<typename T>
T
xpcc::determinant(const xpcc::Matrix<T, 2, 2> &m)
{
	return (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
}

// ----------------------------------------------------------------------------
template<typename T, uint8_t N>
T
xpcc::determinant(const xpcc::Matrix<T, N, N> &m)
{
	// not the most efficient way, but should work for now...
	T value = 0;
	int8_t factor = 1;
	for (uint_fast8_t i = 0; i < N; ++i)
	{
		T coeff = m[0][i];
		xpcc::Matrix<T, N-1, N-1> subM;
		
		for (uint_fast8_t x = 0; x < i; ++x) {
			for (uint_fast8_t y = 1; y < N; ++y) {
				subM[y-1][x] = m[y][x];
			}
		}
		
		for (uint_fast8_t x = i+1; x < N; ++x) {
			for (uint_fast8_t y = 1; y < N; ++y) {
				subM[y-1][x-1] = m[y][x];
			}
		}
		
		value += coeff * factor * determinant(subM);
		factor *= -1;
	}
	
	return value;
}

