#ifndef CHECKED_SAFE_FRACT_CALCULATOR_H
#define CHECKED_SAFE_FRACT_CALCULATOR_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <exception>
#include <cstdint>

#include "SafeFractCalculator.h"
#include "debug.h"
#include "fract.h"

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
    // UNSAFE OPERATION CLASS +++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    class UnsafeOperationException : public std::exception
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // WHAT ---------------------------------------------------------------
            
        virtual char const * what( void ) const throw()
        {
            return "Unsafe operation detected";
        }
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
    
    UnsafeOperation;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // SAFE FRACT CALCULATOR CLASS ++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    class CheckedSafeFractCalculator
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        typedef std::uint64_t int_type;
        typedef long double float_type;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ADD ----------------------------------------------------------------
            
        template< class Calc1, class Calc2 >
        static fract< Calc1 > const add( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise fractions and check addition safety
            
            if( equalise( clhs, crhs ) )
            {
                // Get numerators
                
                int_type lNumer = clhs.getNumerator();
                int_type rNumer = crhs.getNumerator();
                
                // Check addition is safe

                if( isAdditionSafe( lNumer, rNumer ) )
                {
                    // Perform addition based on sign
                    
                    if( clhs.isPositive() == crhs.isPositive() )
                    {
                        lNumer += rNumer;
                    }
                    else
                    {
                        if( lNumer > rNumer )
                        {
                            lNumer -= rNumer;
                        }
                        else
                        {
                            lNumer = rNumer - lNumer;
                            clhs.negate();
                        }
                    }

                    // Return result

                    return fract< Calc1 >( lNumer,
                                           clhs.getDenominator(),
                                           clhs.isPositive() );
                }
            }
            
            // Convert fractions to floating-point types and add
            
            return fract< Calc1 >( toFloatingPoint( lhs ) +
                                   toFloatingPoint( rhs ) );
        }
        
        // MUL ----------------------------------------------------------------
            
        template< class Calc1, class Calc2 >
        static fract< Calc1 > const mul( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            DEBUG_CODE( assert( lhs.getDenominator() > 0 &&
                                rhs.getDenominator() > 0 ) );

            // Determine sign of result

            bool positive = true;

            if( lhs.isPositive() == rhs.isPositive() )
            {
                positive = true;
            }
            else
            {
                positive = false;
            }
            
            // Get fraction components
                
            int_type lNumer = lhs.getNumerator();
            int_type lDenom = lhs.getDenominator();
            int_type rNumer = rhs.getNumerator();
            int_type rDenom = rhs.getDenominator();
            
            // Check to see if multiplication is safe
            
            if( isMultiplicationSafe( lNumer, rNumer ) &&
                isMultiplicationSafe( lDenom, rDenom ) )
            {
                // Multiply LHS with RHS
                
                lNumer *= rNumer;
                lDenom *= rDenom;
                
                // Return result
                
                return fract< Calc1 >( lNumer, lDenom, positive );
            }
            
            // Convert fractions to floating-point types and multiply
            
            return fract< Calc1 >( toFloatingPoint( lhs ) *
                                   toFloatingPoint( rhs ) );
        }
        
        // ARE EQUAL ----------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const areEqual( fract< Calc1 > const & lhs,
                                    fract< Calc2 > const & rhs )
        {
            // Check signs
            
            if( lhs.isPositive() != rhs.isPositive() )
            {
                return false;
            }
            
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise
            
            if( equalise( clhs, crhs ) )
            {
                // Return result of numerator comparison
                
                return ( clhs.getNumerator() == crhs.getNumerator() );
            }
            
            // Equalise has failed, compare floating-point conversions
            
            float_type flhs = toFloatingPoint( lhs );
            float_type frhs = toFloatingPoint( rhs );
            
            flhs = std::abs( flhs - frhs );
            
            return flhs < std::numeric_limits< float_type >::epsilon();
        }
        
        // IS LESS ------------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isLess( fract< Calc1 > const & lhs,
                                  fract< Calc2 > const & rhs )
        {
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise
            
            if( equalise( clhs, crhs ) )
            {
                // Compare signs
            
                if( clhs.isPositive() )
                {
                    if( crhs.isNegative() )
                    {
                        return false;
                    }

                    return clhs.getNumerator() < crhs.getNumerator();
                }
                else
                {
                    if( rhs.isPositive() )
                    {
                        return true;
                    }

                    return clhs.getNumerator() > crhs.getNumerator();
                }
            }
            
            // Equalise has failed, compare floating-point conversions
            
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );
            
            return ( diff + EPSILON ) > 0;
        }
        
        // IS LESS OR EQUAL ---------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isLessOrEqual( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise
            
            if( equalise( clhs, crhs ) )
            {
                // Compare signs
            
                if( clhs.isPositive() )
                {
                    if( crhs.isNegative() )
                    {
                        return false;
                    }

                    return clhs.getNumerator() <= crhs.getNumerator();
                }
                else
                {
                    if( rhs.isPositive() )
                    {
                        return true;
                    }

                    return clhs.getNumerator() >= crhs.getNumerator();
                }
            }
            
            // Equalise has failed, compare floating-point conversions
            
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );

            return ( std::abs( diff ) < EPSILON ) || ( diff + EPSILON > 0 );
        }
        
        // IS GREATER ---------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreater( fract< Calc1 > const & lhs,
                                     fract< Calc2 > const & rhs )
        {
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise
            
            if( equalise( clhs, crhs ) )
            {
                // Compare signs
            
                if( lhs.isPositive() )
                {
                    if( rhs.isNegative() )
                    {
                        return true;
                    }

                    return clhs.getNumerator() > crhs.getNumerator();
                }
                else
                {
                    if( rhs.isPositive() )
                    {
                        return false;
                    }

                    return clhs.getNumerator() < crhs.getNumerator();
                }
            }
            
            // Equalise has failed, compare floating-point conversions
            
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );
            
            return ( diff + EPSILON ) < 0;
        }
        
        // IS GREATER OR EQUAL ------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreaterOrEqual( fract< Calc1 > const & lhs,
                                            fract< Calc2 > const & rhs )
        {
            // Copy fractions
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            // Attempt to equalise
            
            if( equalise( clhs, crhs ) )
            {
                // Compare signs
            
                if( lhs.isPositive() )
                {
                    if( rhs.isNegative() )
                    {
                        return true;
                    }

                    return clhs.getNumerator() >= crhs.getNumerator();
                }
                else
                {
                    if( rhs.isPositive() )
                    {
                        return false;
                    }

                    return clhs.getNumerator() <= crhs.getNumerator();
                }
            }
            
            // Equalise has failed, compare floating-point conversions
            
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );

            return ( std::abs( diff ) < EPSILON ) || ( diff + EPSILON < 0 );
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // PI -----------------------------------------------------------------
        
        /*  The constant [PI] is initialised to
         *  2646693125139304345/842468587426513207, which is accurate to 37
         *  decimal places. This and similar fractions can be found at:
         *  http://qin.laya.com/tech_projects_approxpi.html
         */
        
        static fract< CheckedSafeFractCalculator > const PI;
        static fract< CheckedSafeFractCalculator > const TWO_PI;
        static fract< CheckedSafeFractCalculator > const HALF_PI;
        
        // EULER'S CONSTANT ---------------------------------------------------
        
        /*  The constant [E] is initialised to 685/252, which is accurate to 4
         *  decimal places. This fraction was calculated as a sum of the
         *  infinite series:
         *  e = sum( 1 / n! ) [0 <= n < infinity]
         */
        
        static fract< CheckedSafeFractCalculator > const E;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        private:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
         
        // EQUALISE -----------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const equalise( fract< Calc1 > & lhs,
                                    fract< Calc2 > & rhs )
        {
            DEBUG_CODE( assert( lhs.getDenominator() > 0 &&
                                rhs.getDenominator() > 0 ) );
            
            // Get denominators
            
            int_type lDenom = lhs.getDenominator();
            int_type rDenom = rhs.getDenominator();
            
            // Check to see if denominators can be multiplied
            
            if( !( isMultiplicationSafe( lDenom, rDenom ) ) )
            {
                return false;
            }
            
            // Calculate least common multiple of denominators
                
            int_type lcm = ( lDenom * rDenom ) / gcd( lDenom, rDenom );
            
            // Check LHS components can be multiplied
            
            int_type lNumer = lhs.getNumerator();
            
            int_type mul = lcm / lDenom;
            
            if( !( isMultiplicationSafe( mul, lNumer ) ) ||
                !( isMultiplicationSafe( mul, lDenom ) ) )
            {
                return false;
            }
            
            lNumer *= mul;
            lDenom *= mul;
            
            // Check RHS numerator can be multiplied
            
            int_type rNumer = rhs.getNumerator();
            
            mul = lcm / rDenom;
            
            if( !( isMultiplicationSafe( mul, rNumer ) ) )
            {
                return false;
            }
            
            rNumer *= mul;
            
            // Set fraction components and return true to indicate success
            
            lhs.setNumerator( lNumer );
            lhs.setDenominator( lDenom );

            rhs.setNumerator( rNumer );
            rhs.setDenominator( lDenom );
            
            return true;
        }
            
        // TO FLOATING POINT --------------------------------------------------
        
        template< class Calc >
        static float_type const toFloatingPoint( fract< Calc > const & x )
        {
            if( x.isPositive() )
            {
                return ( ( float_type )x.getNumerator() ) /
                       ( ( float_type )x.getDenominator() );
            }
            else
            {
                return -( ( ( float_type )x.getNumerator() ) /
                          ( ( float_type )x.getDenominator() ) );
            }
        }
        
        // IS ADDITION SAFE ---------------------------------------------------
        
        static bool const isAdditionSafe( int_type const lhs,
                                          int_type const rhs )
        {
            return ( rhs < ( intMax - lhs ) );
        }
        
        // IS MULTIPLICATION SAFE ---------------------------------------------
        
        static bool const isMultiplicationSafe( int_type const lhs,
                                                int_type const rhs )
        {
            if( lhs != 0 )
            {
                return ( rhs < ( intMax / lhs ) );
            }
            
            return true;
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // MAXIMUM INTEGER VALUE ----------------------------------------------
        
        static int_type const intMax;

        // EPSILON ------------------------------------------------------------

        static float_type const EPSILON;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // PUBLIC STATIC DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // PI ---------------------------------------------------------------------
    
    fract< CheckedSafeFractCalculator > const
    CheckedSafeFractCalculator::PI = 
    fract< CheckedSafeFractCalculator >( 2646693125139304345,
                                         842468587426513207,
                                         true );
    
    fract< CheckedSafeFractCalculator > const
    CheckedSafeFractCalculator::TWO_PI =
    CheckedSafeFractCalculator::PI *
    fract< CheckedSafeFractCalculator >( 2, true );
    
    fract< CheckedSafeFractCalculator > const
    CheckedSafeFractCalculator::HALF_PI = 
    CheckedSafeFractCalculator::PI /
    fract< CheckedSafeFractCalculator >( 2, true );
    
    // EULER'S CONSTANT -------------------------------------------------------
    
    fract< CheckedSafeFractCalculator > const
    CheckedSafeFractCalculator::E = 
    fract< CheckedSafeFractCalculator >( 685, 252, true );

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // PRIVATE STATIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // MAXIMUM INTEGER VALUE --------------------------------------------------

    CheckedSafeFractCalculator::int_type const
    CheckedSafeFractCalculator::intMax =
    std::numeric_limits< int_type >::max();

    // EPSILON ----------------------------------------------------------------

    CheckedSafeFractCalculator::float_type const
    CheckedSafeFractCalculator::EPSILON =
    std::numeric_limits< float_type >::epsilon();
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // CHECKED_SAFE_FRACT_CALCULATOR_H
