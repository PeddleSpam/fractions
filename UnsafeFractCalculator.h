#ifndef UNSAFE_FRACT_CALCULATOR_H
#define UNSAFE_FRACT_CALCULATOR_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <cstdint>

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
    // UNSAFE FRACT CALCULATOR CLASS ++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    class UnsafeFractCalculator
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        typedef std::uint64_t int_type;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        // ADD ----------------------------------------------------------------
            
        template< class Calc1, class Calc2 >
        static fract< Calc1 > const add( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            // Copy fractions and equalise them

            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            equalise( clhs, crhs );
            
            // Get numerators

            int_type lNumer = clhs.getNumerator();
            int_type rNumer = crhs.getNumerator();

            // Use signs to determine process

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

            // Return result of component multiplication

            return fract< Calc1 >( lhs.getNumerator() * rhs.getNumerator(),
                                   lhs.getDenominator() * rhs.getDenominator(),
                                   positive );
        }
        
        // ARE EQUAL ----------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const areEqual( fract< Calc1 > const & lhs,
                                    fract< Calc2 > const & rhs )
        {
            // Compare signs
            
            if( lhs.isPositive() != rhs.isPositive() )
            {
                return false;
            }
            
            // Copy fractions and equalise
            
            fract< Calc1 > clhs( lhs );
            fract< Calc2 > crhs( rhs );
            
            equalise( clhs, crhs );
            
            // Compare numerators
            
            return clhs.getNumerator() == crhs.getNumerator();
        }
        
        // IS LESS ------------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isLess( fract< Calc1 > const & lhs,
                                  fract< Calc2 > const & rhs )
        {
            // Compare signs
            
            if( lhs.isPositive() )
            {
                if( rhs.isNegative() )
                {
                    return false;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() < crhs.getNumerator();
            }
            else
            {
                if( rhs.isPositive() )
                {
                    return true;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() > crhs.getNumerator();
            }
        }
        
        // IS LESS OR EQUAL ---------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isLessOrEqual( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            // Compare signs
            
            if( lhs.isPositive() )
            {
                if( rhs.isNegative() )
                {
                    return false;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() <= crhs.getNumerator();
            }
            else
            {
                if( rhs.isPositive() )
                {
                    return true;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() >= crhs.getNumerator();
            }
        }
        
        // IS GREATER ---------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreater( fract< Calc1 > const & lhs,
                                     fract< Calc2 > const & rhs )
        {
            // Compare signs
            
            if( lhs.isPositive() )
            {
                if( rhs.isNegative() )
                {
                    return true;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() > crhs.getNumerator();
            }
            else
            {
                if( rhs.isPositive() )
                {
                    return false;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() < crhs.getNumerator();
            }
        }
        
        // IS GREATER OR EQUAL ------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreaterOrEqual( fract< Calc1 > const & lhs,
                                            fract< Calc2 > const & rhs )
        {
            // Compare signs
            
            if( lhs.isPositive() )
            {
                if( rhs.isNegative() )
                {
                    return true;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() >= crhs.getNumerator();
            }
            else
            {
                if( rhs.isPositive() )
                {
                    return false;
                }
                
                // Copy fractions, equalise, and compare
            
                fract< Calc1 > clhs( lhs );
                fract< Calc2 > crhs( rhs );

                equalise( clhs, crhs );
                
                return clhs.getNumerator() <= crhs.getNumerator();
            }
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // PI -----------------------------------------------------------------
        
        /*  The constant [PI] is initialised to 311/99, which is accurate to 3
         *  decimal places. This and similar fractions can be found at:
         *  http://qin.laya.com/tech_projects_approxpi.html
         */
        
        static fract< UnsafeFractCalculator > const PI;
        static fract< UnsafeFractCalculator > const TWO_PI;
        static fract< UnsafeFractCalculator > const HALF_PI;
        
        // EULER'S CONSTANT ---------------------------------------------------
        
        /*  The constant [E] is initialised to 685/252, which is accurate to 4
         *  decimal places. This fraction was calculated as a sum of the
         *  infinite series:
         *  e = sum( 1 / n! ) [0 <= n < infinity]
         */
        
        static fract< UnsafeFractCalculator > const E;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        private:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // EQUALISE -----------------------------------------------------------
            
        template< class Calc1, class Calc2 >
        static void equalise( fract< Calc1 > & lhs, fract< Calc2 > & rhs )
        {
            DEBUG_CODE( assert( lhs.getDenominator() > 0 &&
                                rhs.getDenominator() > 0 ) );

            // Get fraction components

            int_type lNumer = lhs.getNumerator();
            int_type lDenom = lhs.getDenominator();
            int_type rNumer = rhs.getNumerator();
            int_type rDenom = rhs.getDenominator();
            
            // Compute least common multiple of denominators

            int_type lcm = ( lDenom / gcd( lDenom, rDenom ) ) * rDenom;
            
            // Equalise LHS

            int_type mul = lcm / lDenom;

            lNumer *= mul;
            lDenom *= mul;

            // Equalise RHS

            mul = lcm / rDenom;

            rNumer *= mul;
            
            // Set fraction components
            
            lhs.setNumerator( lNumer );
            lhs.setDenominator( lDenom );
            
            rhs.setNumerator( rNumer );
            rhs.setDenominator( lDenom );
        }
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // PUBLIC STATIC DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // PI ---------------------------------------------------------------------
    
    fract< UnsafeFractCalculator > const
    UnsafeFractCalculator::PI =
    fract< UnsafeFractCalculator >( 311, 99, true );
    
    fract< UnsafeFractCalculator > const
    UnsafeFractCalculator::TWO_PI =
    UnsafeFractCalculator::PI *
    fract< UnsafeFractCalculator >( 2, true );
    
    fract< UnsafeFractCalculator > const
    UnsafeFractCalculator::HALF_PI = 
    UnsafeFractCalculator::PI /
    fract< UnsafeFractCalculator >( 2, true );
    
    // EULER'S CONSTANT -------------------------------------------------------
    
    fract< UnsafeFractCalculator > const
    UnsafeFractCalculator::E = 
    fract< UnsafeFractCalculator >( 685, 252, true );
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // UNSAFE_FRACT_CALCULATOR_H
