#include <gtest/gtest.h>
#include <iostream>
#include "matrix.hpp"

class MATRIX_TEST : public ::testing::Test {
public:
	Matrix<int> *mtrx1;
	Matrix<int> *mtrx2;

	void SetUp() {
		mtrx1 = new Matrix<int>( 3, 3);
		for ( int i = 0; i < 3; i++)
			for ( int j = 0; j < 3; j++)
				mtrx1->setItem( i, j, 1);

		mtrx2 = new Matrix<int>( 3, 3);
		for ( int i = 0; i < 3; i++)
			for ( int j = 0; j < 3; j++)
				mtrx2->setItem( i, j, 2);
	}

	void TearDown() {
		delete mtrx1;
		delete mtrx2;
	}
};

TEST_F( MATRIX_TEST, ADDITION) {
	EXPECT_EQ( *mtrx1 + *mtrx1, *mtrx2);
}


TEST_F( MATRIX_TEST, SUBTRACTION) {
	EXPECT_EQ( *mtrx2 - *mtrx1, *mtrx1);
}

TEST_F( MATRIX_TEST, COMPARISON) {
	EXPECT_EQ( false, *mtrx1 == *mtrx2 );
	EXPECT_EQ( true, *mtrx2 == *mtrx2);
}

TEST_F( MATRIX_TEST, MULTIPLICATION) {
	Matrix<int> result( 3, 3);
	for ( int i = 0; i < 3; i++)
		for ( int j = 0; j < 3; j++)
			result.setItem( i, j, 12);
	EXPECT_EQ( (*mtrx2) * (*mtrx2), result);


	Matrix<int> arg1( 2, 3);
	for ( int i = 0; i < 2; i++)
		for ( int j = 0; j < 3; j++)
			arg1.setItem( i, j, 1);

	Matrix<int> arg2( 3, 4);
	for ( int i = 0; i < 3; i++)
		for ( int j = 0; j < 4; j++)
			arg2.setItem( i, j, 2);

	Matrix<int> comp( 2, 4);
	for ( int i = 0; i < 2; i++)
		for ( int j = 0; j < 4; j++)
			comp.setItem( i, j, 6);

	Matrix<int> mt = arg1 * arg2;
	std::cout << mt;

	try {
		EXPECT_EQ( mt, comp);
	} catch( char *s) {
		std::cerr << s << std::endl;
	}
}

TEST_F( MATRIX_TEST, CONSTRUCTORS) {
	Matrix<int> mt1( *mtrx1);
	EXPECT_EQ( *mtrx1, mt1);

	Matrix<int> mt2( std::move( mt1));
	EXPECT_EQ( *mtrx1, mt2);
}
#if 0
TEST( INPUT, UNIT_1) {
	Matrix<int> mtrx( 3, 3); 
	std::cin >> mtrx;
	std::cout << mtrx;
}
#endif

TEST_F( MATRIX_TEST, OPERATORS) {
	Matrix<int> mt = *mtrx1;
	EXPECT_EQ( mt, *mtrx1);

	Matrix<int> mt1 = std::move(mt);
	EXPECT_EQ( mt1, *mtrx1);
}

TEST_F( MATRIX_TEST, DETERMINANT) {

	Matrix<double> mt( 2, 2);
	mt.setItem( 0, 0, 3);
	mt.setItem( 0, 1, 1);
	mt.setItem( 1, 0, 1);
	mt.setItem( 1, 1, 3);

	EXPECT_DOUBLE_EQ( mt.determinant(), 8);
}

int main( int argc, char** argv) {
	::testing::InitGoogleTest( &argc, argv);
	return RUN_ALL_TESTS();
}