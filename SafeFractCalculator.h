#ifndef SAFE_FRACT_CALCULATOR_H
#define SAFE_FRACT_CALCULATOR_H

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
    // SAFE FRACT CALCULATOR CLASS ++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    class SafeFractCalculator
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
            return fract< Calc1 >( toFloatingPoint( lhs ) +
                                   toFloatingPoint( rhs ) );
        }
        
        // MUL ----------------------------------------------------------------
            
        template< class Calc1, class Calc2 >
        static fract< Calc1 > const mul( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            return fract< Calc1 >( toFloatingPoint( lhs ) *
                                   toFloatingPoint( rhs ) );
        }
        
        // ARE EQUAL ----------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const areEqual( fract< Calc1 > const & lhs,
                                    fract< Calc2 > const & rhs )
        {
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
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );
            
            return ( diff + EPSILON ) > 0;
        }
        
        // IS LESS OR EQUAL ---------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isLessOrEqual( fract< Calc1 > const & lhs,
                                         fract< Calc2 > const & rhs )
        {
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );

            return ( std::abs( diff ) < EPSILON ) || ( diff + EPSILON > 0 );
        }
        
        // IS GREATER ---------------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreater( fract< Calc1 > const & lhs,
                                     fract< Calc2 > const & rhs )
        {
            float_type diff = toFloatingPoint( lhs ) - toFloatingPoint( rhs );
            
            return ( diff + EPSILON ) < 0;
        }
        
        // IS GREATER OR EQUAL ------------------------------------------------
        
        template< class Calc1, class Calc2 >
        static bool const isGreaterOrEqual( fract< Calc1 > const & lhs,
                                            fract< Calc2 > const & rhs )
        {
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
        
        static fract< SafeFractCalculator > const PI;
        static fract< SafeFractCalculator > const TWO_PI;
        static fract< SafeFractCalculator > const HALF_PI;
        
        // EULER'S CONSTANT ---------------------------------------------------
        
        /*  The constant [E] is initialised to 685/252, which is accurate to 4
         *  decimal places. This fraction was calculated as a sum of the
         *  infinite series:
         *  e = sum( 1 / n! ) [0 <= n < infinity]
         */
        
        static fract< SafeFractCalculator > const E;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        private:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
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

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // EPSILON ------------------------------------------------------------

        static float_type const EPSILON;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // PUBLIC STATIC DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // PI ---------------------------------------------------------------------
    
    fract< SafeFractCalculator > const
    SafeFractCalculator::PI =
    fract< SafeFractCalculator >( 2646693125139304345,
                                  842468587426513207,
                                  true );
    
    fract< SafeFractCalculator > const
    SafeFractCalculator::TWO_PI =
    SafeFractCalculator::PI *
    fract< SafeFractCalculator >( 2, true );
    
    fract< SafeFractCalculator > const
    SafeFractCalculator::HALF_PI = 
    SafeFractCalculator::PI /
    fract< SafeFractCalculator >( 2, true );
    
    // EULER'S CONSTANT -------------------------------------------------------
    
    fract< SafeFractCalculator > const
    SafeFractCalculator::E = 
    fract< SafeFractCalculator >( 685, 252, true );

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // PRIVATE STATIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // EPSILON ----------------------------------------------------------------

    SafeFractCalculator::float_type const
    SafeFractCalculator::EPSILON =
    std::numeric_limits< float_type >::epsilon();
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // SAFE_FRACT_CALCULATOR_H
