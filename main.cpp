// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include <cstdint>
#include <iomanip>
#include <chrono>
#include <limits>
#include <string>
#include <ctime>

#include "CheckedSafeFractCalculator.h"
#include "UnsafeFractCalculator.h"
#include "SafeFractCalculator.h"
#include "matrix.h"
#include "sfract.h"
#include "fract.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// TYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef long double float_type;
typedef std::uint64_t int_type;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// SECONDS --------------------------------------------------------------------

float_type seconds( void )
{
	// C++11 ------------------------------------------------------------------
	
	// Initialise start time - first time [seconds] is called.
	
	static std::chrono::high_resolution_clock::time_point start =
		std::chrono::high_resolution_clock::now();
	
	// Get current time

	std::chrono::high_resolution_clock::time_point current =
		std::chrono::high_resolution_clock::now();

	// Find duration between current and start time
	
	std::chrono::duration< float_type > duration =
		std::chrono::duration_cast< std::chrono::duration< float_type > >
			( current - start );
	
	return duration.count();

	// C++98 ------------------------------------------------------------------

	/*
	return ( ( float_type )std::clock() ) / ( ( float_type )CLOCKS_PER_SEC );
	*/

	// NIX --------------------------------------------------------------------

	/*
	timespec ts;
	
	clock_gettime( CLOCK_MONOTONIC, &ts );
	
	return ( ( float_type )ts.tv_sec ) +
			 ( ( ( float_type )ts.tv_nsec ) /
			 1000000000.0l );
	*/
}

// SINE -----------------------------------------------------------------------

float_type const fsin( float_type const x )
{
	static float_type const PI = 3.141592653589793238462643383279502884197169l;
	static float_type const HALF_PI = PI / 2.0l;
	
	// Ensure x <= ( pi / 2 )

	float_type nx = x;
	
	if( nx > HALF_PI )
	{
		nx = std::fmod( nx, HALF_PI );
	}
	
	// Calculate cosine of x using Taylor Expansion

	float_type const one = 1.0l;
	float_type sign = -1.0l;
	float_type fact = one;
	float_type fnum = one;
	float_type px = nx;

	float_type sum = nx;

	for( unsigned i = 0; i < 9; i += 1 )
	{
		// Calculate factorial

		fnum += one;
		fact *= fnum;
		fnum += one;
		fact *= fnum;

		// Calculate power
			
		px *= nx;
		px *= nx;

		// Add term to sum
			
		sum += sign * ( px / fact );
			
		// Flip sign

		sign *= -1.0l;
	}

	// Ensure sign is positive

	if( sum < 0.0l )
	{
		sum *= -1.0l;
	}

	// Return result
		
	return sum;
}

// SINE TEST ------------------------------------------------------------------

bool const sineTest( int_type const divisionCount, 
					 int_type const loopCount,
					 float_type & runTime,
					 float_type & sum )
{
	static float_type const PI = 3.141592653589793238463l;
	static float_type const HALF_PI = PI / 2.0l;
	
	// Check inputs
	
	if( divisionCount < 1 || loopCount < 1 )
	{
		return false;
	}
	
	runTime = 0.0l;
	sum = 0.0l;
	
	// Construct input table
	
	float_type * table = new float_type[ ( unsigned int )divisionCount ];
	float_type const delta = HALF_PI / ( ( float_type )divisionCount );
	
	for( int_type i = 0; i < divisionCount; i += 1 )
	{
		table[ i ] = delta * ( ( float_type )( i + 1 ) );
	}
	
	// Perform test
	
	float_type startTime = seconds();
	
	for( int_type i = 0; i < loopCount; i += 1 )
	{
		for( int_type j = 0; j < divisionCount; j += 1 )
		{
			sum += fsin( table[ j ] );
		}
	}
	
	float_type endTime = seconds();
	
	// Output timing
	
	runTime = endTime - startTime;

	// Clean up table

	delete[] table;
	
	return true;
}

template< class Calc >
bool const sineTest( int_type const divisionCount, 
                     int_type const loopCount,
                     float_type & runTime,
                     fract::fract< Calc > & sum )
{
	typedef fract::fract< Calc > fract_type;
	
	// Check inputs
	
	if( divisionCount < 1 || loopCount < 1 )
	{
		return false;
	}
	
	runTime = 0.0l;
	sum = fract_type( 0, true );
	
	// Construct input table
	
	fract_type * table = new fract_type[ ( unsigned int )divisionCount ];
	fract_type const delta = fract_type::HALF_PI /
							 fract_type( divisionCount, true );
	
	for( int_type i = 0; i < divisionCount; i += 1 )
	{
		table[ i ] = delta * fract_type( i + 1, true );
	}
	
	// Perform test
	
	float_type startTime = seconds();
	
	for( int_type i = 0; i < loopCount; i += 1 )
	{
		for( int_type j = 0; j < divisionCount; j += 1 )
		{
			sum += fract::sin( table[ j ] );
		}
	}
	
	float_type endTime = seconds();
	
	// Output timing
	
	runTime = endTime - startTime;

	// Clean up table

	delete[] table;
	
	return true;
}

template< class FractType, class Allocator >
bool const sineTest( int_type const divisionCount,
					 int_type const loopCount,
					 float_type & runTime,
					 fract::sfract< FractType, Allocator > & sum )
{
	typedef FractType fract_type;
	typedef fract::sfract< fract_type, Allocator > sfract_type;

	// Check inputs
	
	if( divisionCount < 1 || loopCount < 1 )
	{
		return false;
	}

	runTime = 0.0l;
	sum = sfract_type::ZERO;

	// Construct input table
	
	sfract_type * table = new sfract_type[ ( unsigned int )divisionCount ];
	sfract_type const delta = sfract_type::HALF_PI /
							 sfract_type( fract_type( divisionCount, true ) );
	delta.evaluate();
	
	for( int_type i = 0; i < divisionCount; i += 1 )
	{
		table[ i ] = delta * sfract_type( fract_type( i + 1, true ) );
		table[ i ].evaluate();
	}

	// Perform test
	
	float_type startTime = seconds();
	
	for( int_type i = 0; i < loopCount; i += 1 )
	{
		for( int_type j = 0; j < divisionCount; j += 1 )
		{
			sum += fract::sin( table[ j ] );
			sum.evaluate();
		}
	}
	
	float_type endTime = seconds();
	
	// Output timing
	
	runTime = endTime - startTime;

	// Clean up table

	delete[] table;
	
	return true;
}

// RUN SINE TESTS -------------------------------------------------------------

bool const runSineTests( int_type divisionCount, int_type loopCount )
{
	typedef fract::fract< fract::UnsafeFractCalculator > unsafe_fract;
	typedef fract::fract< fract::SafeFractCalculator > safe_fract;
	typedef fract::fract< fract::CheckedSafeFractCalculator > checked_fract;
	typedef fract::sfract< checked_fract, std::allocator< char > >
		std_sfract_type;
	typedef fract::sfract< checked_fract >
		pooled_sfract_type;
	
	// Check inputs

	if( divisionCount < 1 )
	{
		divisionCount = 1;
	}

	if( loopCount < 1 )
	{
		loopCount = 1;
	}
	
	safe_fract safeFractSum( 0, true );
	unsafe_fract unsafeFractSum( 0, true );
	checked_fract checkedSafeFractSum( 0, true );
	std_sfract_type std_sfractSum( checkedSafeFractSum );
	pooled_sfract_type pooled_sfractSum( checkedSafeFractSum );

	float_type floatSum = 0.0l;
	float_type runTime = 0.0l;
	
	std::cout << std::setprecision( 16 ) << std::fixed;

	// Long double sine test
	
	if( sineTest( divisionCount, loopCount, runTime, floatSum ) )
	{
		std::cout << "sine test using long double:\n";
		std::cout << "sum      = " << floatSum << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using long double\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}
	
	// Unsafe fract test
	
	if( sineTest( divisionCount, loopCount, runTime, unsafeFractSum ) )
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - UnsafeFractCalculator\n";
		std::cout << "sum      = " << unsafeFractSum.toLongDouble() << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - UnsafeFractCalculator\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}
	
	// Safe fract Test
	
	if( sineTest( divisionCount, loopCount, runTime, safeFractSum ) )
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - SafeFractCalculator\n";
		std::cout << "sum      = " << safeFractSum.toLongDouble() << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - SafeFractCalculator\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}
	
	// Checked safe fract test
	
	if( sineTest( divisionCount, loopCount, runTime, checkedSafeFractSum ) )
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - CheckedSafeFractCalculator\n";
		std::cout << "sum      = " << checkedSafeFractSum.toLongDouble();
		std::cout << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using fract:\n";
		std::cout << "    - CheckedSafeFractCalculator\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}

	// Standard allocating symbolic fract test

	if( sineTest( divisionCount, loopCount, runTime, std_sfractSum ) )
	{
		std::cout << "sine test using sfract\n";
		std::cout << "    - fract\n";
		std::cout << "        - CheckedSafeFractCalculator\n";
		std::cout << "    - std::allocator\n";
		std::cout << "sum      = ";
		std::cout << ( std_sfractSum.evaluate() ).toLongDouble();
		std::cout << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using sfract\n";
		std::cout << "    - fract\n";
		std::cout << "        - CheckedSafeFractCalculator\n";
		std::cout << "    - std::allocator\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}

	// Pooled allocating symbolic fract test

	if( sineTest( divisionCount, loopCount, runTime, pooled_sfractSum ) )
	{
		std::cout << "sine test using sfract\n";
		std::cout << "    - fract\n";
		std::cout << "        - CheckedSafeFractCalculator\n";
		std::cout << "    - PooledAllocator\n";
		std::cout << "sum      = ";
		std::cout << ( pooled_sfractSum.evaluate() ).toLongDouble();
		std::cout << "\n";
		std::cout << "run time = " << runTime << " seconds\n\n\n";
	}
	else
	{
		std::cout << "sine test using sfract\n";
		std::cout << "    - fract\n";
		std::cout << "        - CheckedSafeFractCalculator\n";
		std::cout << "    - PooledAllocator\n";
		std::cout << "FAILED\n\n\n";
		return false;
	}
	
	return true;
}

// TEST MATRIX ----------------------------------------------------------------

bool const testMatrix( void )
{
	typedef fract::fract< fract::CheckedSafeFractCalculator > fract_type;
	typedef fract::sfract< fract_type, std::allocator< char > > sfract_type;
	typedef matrix< sfract_type > matrix_type;

	// Create matrix elements

	sfract_type e12( fract_type( 1, 2, true ) );
	sfract_type e13( fract_type( 1, 3, true ) );
	sfract_type e14( fract_type( 1, 4, true ) );
	sfract_type e15( fract_type( 1, 5, true ) );
	sfract_type e16( fract_type( 1, 6, true ) );
	sfract_type e17( fract_type( 1, 7, true ) );
	sfract_type e18( fract_type( 1, 8, true ) );
	sfract_type e19( fract_type( 1, 9, true ) );
	sfract_type e110( fract_type( 1, 10, true ) );

	matrix_type mat = matrix_type( e12, e13, e14,
								   e15, e16, e17,
								   e18, e19, e110 );
	
	// Create rotation matrix

	matrix_type rot;
	rot.rotate( sfract_type::HALF_PI );

	std::cout << std::setprecision( 3 ) << std::fixed;
	std::cout << "rotation matrix for 90 degrees ( Pi / 2 ) is\n" << rot;
	std::cout << "\n";

	rot();

	std::cout << "after simplification the rotation matrix for 90 degrees";
    std::cout << " is\n" << rot << "\n";

	return true;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MAIN +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int main()
{
	typedef fract::fract< fract::SafeFractCalculator > fract_type;
	typedef fract::sfract< fract_type > sfract_type;
	
	std::cout << "Fract testing...\n\n\n";
	
	// Run tests

	runSineTests( 8, 100 );
	testMatrix();
	
	// Wait to exit
        /*
	std::cout << "Enter any key to exit...";

	std::string input;
	std::cin >> input;
        */
	return 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
