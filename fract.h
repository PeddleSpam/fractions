#ifndef FRACT_H
#define FRACT_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>
#include <cassert>
#include <cstdint>
#include <limits>
#include <cmath>

#include "debug.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace fract
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FORWARD DECLARATIONS +++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class Calculator >
    class fract;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // GREATEST COMMON DIVISOR ------------------------------------------------
    
    template< class IntType >
    IntType const gcd( IntType a, IntType b )
    {
        for( ;; )
        {
            if( a == 0 )
            {
                return b;
            }

            b %= a;

            if( b == 0 )
            {
                return a;
            }

            a %= b;
        }
    }
    
    // MODULO -----------------------------------------------------------------
    
    template< class Calc1, class Calc2 >
    fract< Calc1 > const mod( fract< Calc1 > const & x,
                              fract< Calc2 > const & y )
    {
        return fract< Calc1 >( std::fmod( x.toLongDouble(),
                                          y.toLongDouble() ) );
    }
    
    // SINE -------------------------------------------------------------------
    
    template< class Calculator >
    fract< Calculator > const sin( fract< Calculator > const & x )
    {
        // Ensure x <= HALF_PI ( 90 degrees )
        
        fract< Calculator > nx( x );
        nx.makePositive();
        
        if( nx > fract< Calculator >::HALF_PI )
        {
            nx = mod( nx, fract< Calculator >::HALF_PI );
        }
        
        // Calculate sine of x using Taylor Expansion

        fract< Calculator > const one( 1, true );
        fract< Calculator > sign( 1, false );
        fract< Calculator > fact = one;
        fract< Calculator > fnum = one;
        fract< Calculator > px = nx;

        fract< Calculator > sum = nx;
        
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

            sign.negate();
        }

        // Ensure sign is positive

        sum.makePositive();
        
        // Return result
        
        return sum;
    }

    // COSINE -----------------------------------------------------------------
    
    template< class Calculator >
    fract< Calculator > const cos( fract< Calculator > const & x )
    {
        // Ensure x <= HALF_PI ( 90 degrees )

        fract< Calculator > nx( x );
        nx.makePositive();

        if( nx > fract< Calculator >::HALF_PI )
        {
            nx = mod( nx, fract< Calculator >::HALF_PI );
        }
        
        // Calculate cosine of x using Taylor Expansion

        fract< Calculator > const one( 1, true );
        fract< Calculator > sign( 1, false );
        fract< Calculator > fact = one;
        fract< Calculator > fnum( 0, true );
        fract< Calculator > px = one;

        fract< Calculator > sum = one;

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

            sign = -( sign );
        }

        // Ensure sign is positive

        sum.makePositive();

        // Return result
        
        return sum;
    }

    // TANGENT ----------------------------------------------------------------

    template< class Calculator >
    fract< Calculator > const tan( fract< Calculator > const & x )
    {
        return ( sin( x ) / cos( x ) );
    }

    // POWER ------------------------------------------------------------------

    template< class Calculator >
    fract< Calculator > const pow( fract< Calculator > const & x,
                                   int const exponent )
    {
        // Check for zero exponent

        if( exponent == 0 )
        {
            return fract< Calculator >( 1, 1, true ); // x^0 == 1
        }

        fract< Calculator > temp( x );

        // Check sign of exponent

        if( exponent > 0 )
        {
            for( int i = 1; i < exponent; i += 1 )
            {
                temp *= temp;
            }
        }
        else
        {
            for( int i = -1; i > exponent; i -= 1 )
            {
                temp /= temp;
            }
        }

        return temp;
    }
    
    // OUTPUT STREAM OPERATOR -------------------------------------------------

    template< class Calculator >
    std::ostream & operator << ( std::ostream & out,
                                 fract< Calculator > const & rhs )
    {
        if( rhs.isZero() )
        {
            out << "0";
            return out;
        }

        // Calculate whole and numerator

        typename fract< Calculator >::int_type numer = rhs.getNumerator();
        typename fract< Calculator >::int_type denom = rhs.getDenominator();
        typename fract< Calculator >::int_type whole = numer / denom;

        numer -= ( whole * denom );

        // Output components

        if( rhs.isNegative() )
        {
            out << "-(";

            if( whole > 0 )
            {
                out << whole;

                if( numer > 0 )
                {
                    out << "+" << numer << "/" << denom;
                }
            }
            else
            {
                out << numer << "/" << denom;
            }

            out << ")";
        }
        else
        {
            if( whole > 0 )
            {
                out << whole;

                if( numer > 0 )
                {
                    out << "+" << numer << "/" << denom;
                }
            }
            else
            {
                out << numer << "/" << denom;
            }
        }

        return out;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FRACTION CLASS +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class Calculator >
    class fract
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        typedef Calculator CalcType;
        typedef typename Calculator::int_type int_type;
        typedef long double float_type;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // CONSTRUCTORS -------------------------------------------------------

        fract( void )
        {
            this->positive = true;
            this->numer = 0;
            this->denom = 1;
        }

        fract( int_type whole, bool positive )
        {
            this->positive = positive;
            this->numer = whole;
            this->denom = 1;
        }

        fract( int_type numerator, int_type denominator, bool positive )
        {
            this->positive = positive;
            this->numer = numerator;
            this->denom = denominator;

            if( this->denom == 0 )
            {
                this->denom = 1;
            }

            this->simplify();
        }

        fract( int_type whole,
               int_type numerator,
               int_type denominator,
               bool positive )
        {
            this->positive = positive;
            this->numer = numerator + ( whole * denominator );
            this->denom = denominator;

            if( this->denom == 0 )
            {
                this->denom = 1;
            }

            this->simplify();
        }
        
        /*  fract( float_type const number );
         * 
         *  Calculates the approximate fractional representation of a floating-
         *  point number using continued fractions. The code for this method
         *  is adapted from David Eppstein's program [frap.c] available at:
         *  http://www.ics.uci.edu/~eppstein/numth/frap.c
         */
        
        fract( float_type const number )
        {
            // Calculate constants.
            
            static unsigned intBits =
                std::numeric_limits< int_type >::digits;
            
            static float_type maxInt =
                ( float_type )std::numeric_limits< int_type >::max();
            
            // Setup matrices to store fractions
            
            float_type m1[ 2 ] = { 1, 0 }; // [0][0], [1][0]
            float_type m2[ 2 ] = { 0, 1 }; // [0][1], [1][1]
            float_type m3[ 2 ];
            
            // Setup variables
            
            float_type ai, x = std::abs( number );
            
            // Calculate terms until maximum integer value is reached
            
            for( unsigned i = 0; i < intBits; i += 1 )
            {
                // Get integer component
                
                ai = std::floor( x );
                
                // Calculate term for numerator
                
                m3[ 0 ] = m1[ 0 ] * ai + m2[ 0 ];
                m2[ 0 ] = m1[ 0 ];
                
                // Calculate term for denominator
                
                m3[ 1 ] = m1[ 1 ] * ai + m2[ 1 ];
                m2[ 1 ] = m1[ 1 ];
                
                // Check for maximum integer value
                
                if( m3[ 0 ] > maxInt || m3[ 1 ] > maxInt )
                {
                    break;
                }
                        
                // Update values for next term
                
                m1[ 0 ] = m3[ 0 ];
                m1[ 1 ] = m3[ 1 ];
                
                if( x == std::floor( x ) )
                {
                    break;
                }
                
                x = 1 / ( x - std::floor( ai ) );
            }
            
            // Assign results to class members
            
            this->numer = ( int_type )m1[ 0 ];
            this->denom = ( int_type )m1[ 1 ];
            
            // Check for edge cases

            if( this->numer < 1 )
            {
                this->denom = 1;
                this->positive = true;
            }
            else
            {
                if( this->denom < 1 )
                {
                    this->denom = 1;
                }

                if( number >= 0 )
                {
                    this->positive = true;
                }
                else
                {
                    this->positive = false;
                }
            }
        }
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        fract( fract const & other )
        {
            this->positive = other.positive;
            this->numer = other.numer;
            this->denom = other.denom;
        }

        template< class Calc >
        fract( fract< Calc > const & other )
        {
            this->positive = other.isPositive();
            this->numer = other.getNumerator();
            this->denom = other.getDenominator();
        }

        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        fract( fract && other )
        {
            this->positive = other.positive;
            this->numer = other.numer;
            this->denom = other.denom;
        }

        template< class Calc >
        fract( fract< Calc > && other )
        {
            this->positive = other.isPositive();
            this->numer = other.getNumerator();
            this->denom = other.getDenominator();
        }

        // DESTRUCTOR ---------------------------------------------------------

        ~fract( void )
        {
            // empty
        }

        // RECIPROCAL ---------------------------------------------------------

        fract const reciprocal( void ) const
        {
            fract temp;

            temp.positive = this->positive;
            temp.numer = this->denom;
            temp.denom = this->numer;

            if( temp.denom < 1 )
            {
                temp.numer = 0;
                temp.denom = 1;
            }

            return temp;
        }

        // GET NUMERATOR ------------------------------------------------------

        int_type const getNumerator( void ) const
        {
            return this->numer;
        }
        
        // SET NUMERATOR ------------------------------------------------------
        
        void setNumerator( int_type numerator )
        {
            this->numer = numerator;
        }
        
        // GET DENOMINATOR ----------------------------------------------------

        int_type const getDenominator( void ) const
        {
            return this->denom;
        }
        
        // SET DENOMINATOR ----------------------------------------------------
        
        void setDenominator( int_type denominator )
        {
            this->denom = denominator;
            
            if( this->denom < 1 )
            {
                this->denom = 1;
            }
        }
        
        // IS POSITIVE --------------------------------------------------------

        bool const isPositive( void ) const
        {
            return this->positive;
        }

        // IS NEGATIVE --------------------------------------------------------

        bool const isNegative( void ) const
        {
            return !( this->positive );
        }
        
        // MAKE POSITIVE ------------------------------------------------------
        
        void makePositive( void )
        {
            this->positive = true;
        }
        
        // MAKE NEGATIVE ------------------------------------------------------
        
        void makeNegative( void )
        {
            this->positive = false;
        }
        
        // NEGATION -----------------------------------------------------------
        
        fract const negation( void ) const
        {
            fract temp( *this );
            
            temp.negate();
            
            return temp;
        }
        
        // NEGATE -------------------------------------------------------------

        void negate( void )
        {
            this->positive = !( this->positive );
        }
        
        // IS ZERO ------------------------------------------------------------

        bool const isZero( void ) const
        {
            return ( this->numer == 0 );
        }
        
        // TO SHORT -----------------------------------------------------------

        short const toShort( void ) const
        {
            if( this->positive )
            {
                return ( this->numer / this->denom );
            }
            else
            {
                return -( this->numer / this->denom );
            }
        }

        // TO INT -------------------------------------------------------------

        int const toInt( void ) const
        {
            if( this->positive )
            {
                return ( this->numer / this->denom );
            }
            else
            {
                return -( this->numer / this->denom );
            }
        }

        // TO LONG INT --------------------------------------------------------

        long const toLongInt( void ) const
        {
            if( this->positive )
            {
                return ( this->numer / this->denom );
            }
            else
            {
                return -( this->numer / this->denom );
            }
        }

        // TO FLOAT -----------------------------------------------------------

        float const toFloat( void ) const
        {
            if( this->positive )
            {
                return ( ( ( float )this->numer ) /
                         ( ( float )this->denom ) );
            }
            else
            {
                return -( ( ( float )this->numer ) /
                          ( ( float )this->denom ) );
            }
        }

        // TO DOUBLE ----------------------------------------------------------

        double const toDouble( void ) const
        {
            if( this->positive )
            {
                return ( ( ( double )this->numer ) /
                         ( ( double )this->denom ) );
            }
            else
            {
                return -( ( ( double )this->numer ) /
                          ( ( double )this->denom ) );
            }
        }

        // TO LONG DOUBLE -----------------------------------------------------

        long double const toLongDouble( void ) const
        {
            if( this->positive )
            {
                return ( ( ( long double )this->numer ) /
                         ( ( long double ) this->denom ) );
            }
            else
            {
                return -( ( ( long double )this->numer ) /
                          ( ( long double ) this->denom ) );
            }
        }

        // SINE ---------------------------------------------------------------

        fract const sin( void ) const
        {
            return sin( *this );
        }
        
        // COSINE -------------------------------------------------------------

        fract const cos( void ) const
        {
            return cos( *this );
        }

        // TANGENT ------------------------------------------------------------

        fract const tan( void ) const
        {
            return tan( *this );
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC OPERATORS +++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // COPY ASSIGNMENT ----------------------------------------------------
        
        fract const & operator = ( fract const & other )
        {
            fract temp( other );

            this->positive = temp.positive;
            this->numer = temp.numer;
            this->denom = temp.denom;

            return *this;
        }
        
        template< class Calc >
        fract const & operator = ( fract< Calc > const & other )
        {
            fract< Calc > temp( other );

            this->positive = temp.positive;
            this->numer = temp.numer;
            this->denom = temp.denom;

            return *this;
        }

        // MOVE ASSIGNMENT ----------------------------------------------------
        
        fract const & operator = ( fract && other )
        {
            this->positive = other.positive;
            this->numer = other.numer;
            this->denom = other.denom;

            return *this;
        }
        
        template< class Calc >
        fract const & operator = ( fract< Calc > && other )
        {
            this->positive = other.positive;
            this->numer = other.numer;
            this->denom = other.denom;

            return *this;
        }

        // ADDITION -----------------------------------------------------------

        template< class Calc >
        fract const operator + ( fract< Calc > const & right ) const
        {
            // Add operands
            
            fract result = CalcType::add( *this, right );
            
            // Simplify and return result

            result.simplify();

            return result;
        }

        // SUBTRACTION --------------------------------------------------------

        template< class Calc >
        fract const operator - ( fract< Calc > const & right ) const
        {
            // Add left to negative of right
            
            fract result = CalcType::add( *this, right.negation() );
            
            // Simplify and return result

            result.simplify();

            return result;
        }

        // MULTIPLICATION -----------------------------------------------------

        template< class Calc >
        fract const operator * ( fract< Calc > const & right ) const
        {
            // Multiply operands
            
            fract result = CalcType::mul( *this, right );
            
            // Simplify and return result

            result.simplify();

            return result;
        }

        // DIVISION -----------------------------------------------------------

        template< class Calc >
        fract const operator / ( fract< Calc > const & right ) const
        {
            // Multiply by reciprocal of RHS and return result

            return ( ( *this ) * ( right.reciprocal() ) );
        }

        // UNARY MINUS --------------------------------------------------------

        fract const operator - ( void ) const
        {
            return this->negation();
        }

        // COMPOUND ASSIGNMENT ------------------------------------------------

        template< class Calc >
        fract const & operator += ( fract< Calc > const & right )
        {
            return ( *this ) = ( *this ) + right;
        }

        template< class Calc >
        fract const & operator -= ( fract< Calc > const & right )
        {
            return ( *this ) = ( *this ) - right;
        }

        template< class Calc >
        fract const & operator *= ( fract< Calc > const & right )
        {
            return ( *this ) = ( *this ) * right;
        }

        template< class Calc >
        fract const & operator /= ( fract< Calc > const & right )
        {
            return ( *this ) = ( *this ) / right;
        }

        // EQUALITY -----------------------------------------------------------

        template< class Calc >
        bool const operator == ( fract< Calc > const & right ) const
        {
            return CalcType::areEqual( *this, right );
        }

        // INEQUALITY ---------------------------------------------------------

        template< class Calc >
        bool const operator != ( fract< Calc > const & right ) const
        {
            return !( CalcType::areEqual( *this, right ) );
        }

        // GREATER THAN -------------------------------------------------------

        template< class Calc >
        bool const operator > ( fract< Calc > const & right ) const
        {
            return CalcType::isGreater( *this, right );
        }

        // LESS THAN ----------------------------------------------------------

        template< class Calc >
        bool const operator < ( fract< Calc > const & right ) const
        {
            return CalcType::isLess( *this, right );
        }

        // GREATER THAN OR EQUAL ----------------------------------------------

        template< class Calc >
        bool const operator >= ( fract< Calc > const & right ) const
        {
            return CalcType::isGreaterOrEqual( *this, right );
        }

        // LESS THAN OR EQUAL -------------------------------------------------

        template< class Calc >
        bool const operator <= ( fract< Calc > const & right ) const
        {
            return CalcType::isLessOrEqual( *this, right );
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // PI -----------------------------------------------------------------
        
        static fract const & PI;
        static fract const & TWO_PI;
        static fract const & HALF_PI;

        // EULER'S CONSTANT ---------------------------------------------------
        
        static fract const & E;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        private:

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // SIMPLIFY -----------------------------------------------------------

        void simplify( void )
        {
            DEBUG_CODE( assert( this->denom > 0 ) );

            // Check for zero numerator

            if( this->numer == 0 )
            {
                this->denom = 1;
                this->positive = true;
                return;
            }

            // Compute greatest common divisor of numerator and denominator

            int_type const divisor = gcd( this->numer, this->denom );

            // Divide both components by GCD

            this->numer /= divisor;
            this->denom /= divisor;
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        int_type numer;
        int_type denom;

        bool positive;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STATIC DATA MEMBERS OF FRACT +++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // PI ---------------------------------------------------------------------
    
    template< class Calc > fract< Calc > const &
    fract< Calc >::PI = Calc::PI;
    
    template< class Calc > fract< Calc > const &
    fract< Calc >::TWO_PI = Calc::TWO_PI;
    
    template< class Calc > fract< Calc > const &
    fract< Calc >::HALF_PI = Calc::HALF_PI;
    
    // EULER'S CONSTANT -------------------------------------------------------
    
    template< class Calc > fract< Calc > const &
    fract< Calc >::E = Calc::E;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // FRACT_H
