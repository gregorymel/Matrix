#pragma once

#define MAX_LINES   10;
#define MAX_COLUMNS 10;

template <typename T>
class Matrix {
	T** matrix;
	unsigned int columns;
	unsigned int lines;

	bool findNonZero( unsigned int line, 
					  unsigned int &non_zero_line) const;
public:
	void setItem( int i, int j, T value) { matrix[ i][ j] = value;}
	T getItem( int i, int j) { return matrix[ i][ j]; }

	Matrix( int lines = 0, int columns = 0);
	Matrix( const Matrix<T>& other);
	Matrix( Matrix<T>&& other);
	Matrix<T>& operator=( const Matrix<T>& other);
	Matrix<T>& operator=( Matrix<T> copied);
	Matrix<T>& operator=( Matrix<T>&& move);
	~Matrix();

	double determinant();

	Matrix<T> operator+( const Matrix<T>& right);
	Matrix<T> operator-( const Matrix<T>& right);
	Matrix<T> operator*( const Matrix<T>& right);
	template <typename U>
	friend bool operator==( const Matrix<U>& left, const Matrix<U>& right);
	template <typename U>
	friend std::ostream& operator<<( std::ostream& os, const Matrix<U>& mtrx);
	template <typename U>
	friend void swap( Matrix<U>& first, Matrix<U>& second);
	template <typename U>
	friend std::istream& operator>>( std::istream& is, Matrix<U>& mtrx);
};
 
template <typename T>
Matrix<T> Matrix<T>::operator+( const Matrix<T>& right) {
	if ( columns != right.columns || lines != right.lines) {
		throw "NotTheSameSize";	
	}

	Matrix<T> result( right.lines, right.columns);
	for ( int i = 0; i < columns; i++)
		for ( int j = 0; j < lines; j++)
			result.matrix[ i][ j] = this->matrix[ i][ j] + right.matrix[ i][ j];

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-( const Matrix<T>& right) {
	if ( columns != right.columns || lines != right.lines) {
		throw "NotTheSameSize";	
	}

	Matrix<T> result( right.lines, right.columns);
	for ( int i = 0; i < columns; i++)
		for ( int j = 0; j < lines; j++)
			result.matrix[ i][ j] = this->matrix[ i][ j] - right.matrix[ i][ j];

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*( const Matrix<T>& right) {
	if ( columns != right.lines) {
		throw "NotTheSameSize";
	}

	Matrix<T> result( lines, right.columns);
	for ( int i = 0; i < lines; i++)
		for ( int j = 0; j < right.columns; j++) {
			result.matrix[ i][ j] = 0;
			for ( int k = 0; k < right.lines; k++) {
				result.matrix[ i][ j] += matrix[ i][ k] * right.matrix[ k][ j];
			}
		}

	return result;
}

template <typename U>
bool operator==( const Matrix<U>& left, const Matrix<U>& right) {
	if ( left.columns != right.columns || left.lines != right.lines)
		return false;

	for ( int i = 0; i < left.lines; i++)
		for ( int j = 0; j < left.columns; j++)
			if ( right.matrix[ i][ j] != left.matrix[ i][ j])
				return false;

	return true;
}

template <typename T>
std::ostream& operator<<( std::ostream& os, const Matrix<T>& mtrx) {
	for ( int i = 0; i < mtrx.lines; i++) {
		for ( int j = 0; j < mtrx.columns; j++)
			os << mtrx.matrix[ i][ j] << " ";

		os << std::endl;
	}

	return os;
}

template <typename T>
Matrix<T>::Matrix( int lines, int columns) : lines( lines), columns( columns) {
	matrix = lines ? new T*[lines] : NULL;
	for ( int i = 0; i < lines; i++) {
		matrix[ i] = new T[columns]();
	}
}

template <typename T>
Matrix<T>::~Matrix() {
	for ( int i = 0; i < lines; i++) {
		delete [] matrix[ i];
	}
	
	delete [] matrix;
}

template <typename T>
Matrix<T>::Matrix( const Matrix<T>& other) : 
	lines( other.lines),
	columns( other.columns)
{
	matrix = lines ? new T*[ lines] : NULL;
	for ( int i = 0; i < lines; i++) {
		matrix[ i] = columns ? new T[columns] : NULL;
	}

	for ( int i = 0; i < lines; i++) {
		std::copy( other.matrix[ i], other.matrix[ i] + columns, matrix[ i]);
	}
}

template <typename T>
void swap( Matrix<T>& first, Matrix<T>& second) {
	std::swap( first.lines, second.lines);
	std::swap( first.columns, second.columns);
	std::swap( first.matrix, second.matrix);
}

template <typename T>
Matrix<T>& Matrix<T>::operator=( const Matrix<T>& other) {
	Matrix<T> copied( other);
	swap<T>( *this, copied);
	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=( Matrix<T> copied) {
	swap<T>( *this, copied);
	return *this;
}

template <typename T>
Matrix<T>::Matrix( Matrix<T>&& other) :
	lines( other.lines),
	columns( other.columns),
	matrix( other.matrix) 
{
	other.lines = 0;
	other.columns = 0;
	other.matrix = nullptr;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=( Matrix<T>&& move) {
	swap<T>( *this, move);
	return *this;
}

template <typename T>
std::istream& operator>>( std::istream& is, Matrix<T>& mtrx) {

	for ( int i = 0; i < mtrx.lines; i++)
		for ( int j = 0; j < mtrx.columns; j++)
			is >> mtrx.matrix[ i][ j];

	return is;
}

template <>
double Matrix<double>::determinant() {
	if ( this->lines != this->columns)
		throw "IsNotSquareMatrix";

	unsigned int size = this->lines; // size of matrix

	if ( size == 0)
		return 0;

	if ( size == 1)
		return this->matrix[ 0][ 0];

	Matrix<double> copied( *this);
	bool sign;
	double det = 1;
	double tmp;
	unsigned int non_zero_line;
	// cycle along the main diagonal
	for ( int i = 0; i < size; i++) {
		if ( copied.matrix[ i][ i] == 0) {
			if ( !copied.findNonZero( i, non_zero_line))
				return 0;

			std::swap( copied.matrix[ non_zero_line], copied.matrix[ i]);
			// now copy.matrix[i][i] != 0
			sign = !sign;
		}

		det *= copied.matrix[ i][ i];
		tmp = copied.matrix[ i][ i];

		for ( int j = i; j < size; j++) {
			copied.matrix[ i][ j] = copied.matrix[ i][ j] / tmp;
			std::cerr << copied.matrix[ i][ j] << " ";
		}
		std::cout << std::endl;
		// now copy.matrix[ i][ i] == 1


		for ( int j = i + 1; j < size; j++) {
			tmp = copied.matrix[ j][ i];
			for ( int k = i; k < size; k++)
				copied.matrix[ j][ k] -= copied.matrix[ i][ k] * tmp; 
		}

	}
	if ( sign)
		return det * -1;

	return det;
}

template <typename T>
bool Matrix<T>::findNonZero( unsigned int line,
							 unsigned int &non_zero_line) const
{
	unsigned int column = line;
	line++;
	for ( ; line < lines; line++) {
		if 	( matrix[ line][ column] != 0) {
			non_zero_line = line;
			return true;
		}
	}

	return false;		
}
