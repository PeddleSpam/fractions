#ifndef SFRACT_H
#define    SFRACT_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <iostream>

#include "StandardAllocatorPolicy.h"
#include "MultiplicationSymbol.h"
#include "ReciprocalSymbol.h"
#include "PooledAllocator.h"
#include "AdditionSymbol.h"
#include "TangentSymbol.h"
#include "NegateSymbol.h"
#include "ObjectTraits.h"
#include "CosineSymbol.h"
#include "ValueSymbol.h"
#include "SineSymbol.h"
#include "Symbol.h"
#include "fract.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace fract
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FORWARD DECLARATIONS +++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template< class FractType, class Allocator >
    class sfract;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // SINE -------------------------------------------------------------------

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    sin( sfract< FractType, Allocator > const & x )
    {
        return x.sin();
    }

    // COSINE -----------------------------------------------------------------

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    cos( sfract< FractType, Allocator > const & x )
    {
        return x.cos();
    }

    // TANGENT ----------------------------------------------------------------

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    tan( sfract< FractType, Allocator > const & x )
    {
        return x.tan();
    }

    // OUTPUT OPERATOR --------------------------------------------------------

    template< class FractType, class Allocator >
    std::ostream & operator << ( std::ostream & out,
                                 sfract< FractType, Allocator > const & rhs )
    {
        out << ( rhs.evaluate() );
        return out;
    }
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // SYMBOLIC FRACTION CLASS ++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template
    <
        class FractType,
        class Allocator = memory::PooledAllocator
        <
            char,
            memory::StandardAllocatorPolicy< char >,
            memory::ObjectTraits< char >
        >
    >
    class sfract
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef unsigned long int_type;
        typedef FractType fract_type;
        typedef Allocator allocator_type;   
        typedef typename allocator_type::value_type alloc_val_type;

        typedef Symbol< fract_type > symbol_type;
        typedef ValueSymbolBase< fract_type > val_symbol_base_type;
        typedef ValueSymbol< fract_type, allocator_type > val_symbol_type;
        typedef AdditionSymbol< fract_type, allocator_type > add_symbol_type;
        typedef NegateSymbol< fract_type, allocator_type > neg_symbol_type;
        typedef MultiplicationSymbol< fract_type, allocator_type >
            mul_symbol_type;
        typedef ReciprocalSymbol< fract_type, allocator_type > rec_symbol_type;
        typedef SineSymbol< fract_type, allocator_type > sin_symbol_type;
        typedef CosineSymbol< fract_type, allocator_type > cos_symbol_type;
        typedef TangentSymbol< fract_type, allocator_type > tan_symbol_type;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // CONSTRUCTORS -------------------------------------------------------
        
        sfract( void )
        {
            // Allocate and construct value symbol
            
            root =
                symbol_type::template allocate< val_symbol_type >( allocator );
            root = new( root ) val_symbol_type( fract_type() );
        }
        
        sfract( int_type const whole )
        {
            root =
                symbol_type::template allocate< val_symbol_type >( allocator );
            root = new( root ) val_symbol_type( fract_type( whole, true ) );
        }

        template< class Calc >
        sfract( fract< Calc > const & value )
        {
            // Allocate and construct value symbol
            
            root =
                symbol_type::template allocate< val_symbol_type >( allocator );
            root = new( root ) val_symbol_type( value );
        }
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        sfract( sfract const & other )
        {
            // Clone other's root symbol and assign to this object
            
            root = other.root->clone();
        }

        template< class FType, class Alloc >
        sfract( sfract< FType, Alloc > const & other )
        {
            // Clone other's root symbol and assign to this object
            
            root = other.root->clone();
        }
        
        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        sfract( sfract && other )
        {
            // Take ownership of other's root symbol
            
            root = other.root;
            other.root = nullptr;
        }

        template< class FType, class Alloc >
        sfract( sfract< FType, Alloc > && other )
        {
            // Take ownership of other's root symbol
            
            root = other.root;
            other.root = nullptr;
        }
        
        // DESTRUCTOR ---------------------------------------------------------
        
        ~sfract( void )
        {
            symbol_type::deallocate( allocator, root );
        }

        // EVALUATE -----------------------------------------------------------

        fract_type const evaluate( void )
        {
            // Check to see if root is evaluated

            if( !( root->isValue() ) )
            {
                // Evaluate root symbol and store result

                symbol_type * eval = root->evaluate();

                // Deconstruct and deallocate root symbol

                symbol_type::deallocate( allocator, root );

                // Set root to evaluated symbol

                root = eval;
            }

            return static_cast< val_symbol_base_type * >( root )->getValue();
        }

        fract_type const evaluate( void ) const
        {
            // Evaluate root node

            val_symbol_base_type * eval = root->evaluate();
            
            // Get value from evaluation node

            fract_type value = eval->getValue();

            // Deallocate evaluation node

            symbol_type::deallocate( allocator, eval );

            return value;
        }

        // NEGATE -------------------------------------------------------------

        void negate( void )
        {
            // Create new negation symbol
            
            neg_symbol_type * negSymbol =
                symbol_type::template allocate< neg_symbol_type >( allocator );

            negSymbol = new( negSymbol ) neg_symbol_type( *root );

            // Make negation symbol the new root

            root = negSymbol;
        }

        // NEGATION -----------------------------------------------------------

        sfract const negation( void ) const
        {
            sfract temp( *this );

            temp.negate();

            return temp;
        }

        // INVERT -------------------------------------------------------------

        void invert( void )
        {
            // Create new reciprocal symbol

            rec_symbol_type * recSymbol =
                symbol_type::template allocate< rec_symbol_type >( allocator );

            recSymbol = new( recSymbol ) rec_symbol_type( *root );

            // Make reciprocal symbol the new root

            root = recSymbol;
        }

        // RECIPROCAL ---------------------------------------------------------

        sfract const reciprocal( void ) const
        {
            sfract temp( *this );

            temp.invert();

            return temp;
        }

        // SINE ---------------------------------------------------------------

        sfract const sin( void ) const
        {
            // Clone root

            symbol_type * rootClone = root->clone();
            
            // Create new sine symbol

            sin_symbol_type * sinSymbol =
                symbol_type::template allocate< sin_symbol_type >( allocator );

            sinSymbol = new( sinSymbol ) sin_symbol_type( *( rootClone ) );

            // Return new sfract with symbol as root

            return sfract( *sinSymbol );
        }

        // COSINE -------------------------------------------------------------

        sfract const cos( void ) const
        {
            // Clone root

            symbol_type * rootClone = root->clone();
            
            // Create new cosine symbol

            cos_symbol_type * cosSymbol =
                symbol_type::template allocate< cos_symbol_type >( allocator );

            cosSymbol = new( cosSymbol ) cos_symbol_type( *( rootClone ) );

            // Return new sfract with symbol as root

            return sfract( *cosSymbol );
        }

        // TANGENT ------------------------------------------------------------

        sfract const tan( void ) const
        {
            // Create new tangent symbol

            tan_symbol_type * tanSymbol =
                symbol_type::template allocate< tan_symbol_type >( allocator );

            tanSymbol = new( tanSymbol ) tan_symbol_type( *( root->clone() ) );

            // Return new sfract with symbol as root

            return sfract( *tanSymbol );
        }
        
        // GET ALLOCATOR ------------------------------------------------------
        
        static allocator_type get_allocator( void )
        {
            return allocator;
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC OPERATORS +++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // COPY ASSIGNMENT OPERATOR -------------------------------------------
        
        sfract const & operator = ( sfract const & other )
        {
            // Deconstruct and deallocate current root symbol
            
            symbol_type::deallocate( allocator, root );
            
            // Copy root symbol of other object and assign to root
            
            root = other.root->clone();
            
            return *this;
        }

        template< class FType, class Alloc >
        sfract const & operator = ( sfract< FType, Alloc > const & other )
        {
            // Deconstruct and deallocate current root symbol
            
            symbol_type::deallocate( allocator, root );
            
            // Copy root symbol of other object and assign to root
            
            root = other.root->clone();
            
            return *this;
        }
        
        // MOVE ASSIGNMENT OPERATOR -------------------------------------------
        
        sfract const & operator = ( sfract && other )
        {
            // Deconstruct and deallocate current root symbol
            
            symbol_type::deallocate( allocator, root );
            
            // Take ownership of other's root symbol
            
            root = other.root;
            other.root = nullptr;
            
            return *this;
        }

        template< class FType, class Alloc >
        sfract const & operator = ( sfract< FType, Alloc > && other )
        {
            // Deconstruct and deallocate current root symbol
            
            symbol_type::deallocate( allocator, root );
            
            // Take ownership of other's root symbol
            
            root = other.root;
            other.root = nullptr;
            
            return *this;
        }
        
        // ADDITION -----------------------------------------------------------
        
        template< class FType, class Alloc >
        sfract const operator + ( sfract< FType, Alloc > const & other ) const
        {
            // Perform rule reductions

            if( this->root->isValue() )
            {
                val_symbol_base_type * thisVal =
                    reinterpret_cast< val_symbol_base_type * >( this->root );
                
                if( thisVal->getValue() == ZERO ) // 0 + [other]
                {
                    return other;
                }
            }
            
            if( other.root->isValue() )
            {
                val_symbol_base_type * otherVal =
                    reinterpret_cast< val_symbol_base_type * >( other.root );
                
                if( otherVal->getValue() == ZERO ) // [this] + 0
                {
                    return *this;
                }
            }
            
            // Construct new tree with an addition symbol as root and return

            add_symbol_type * addSymbol =
                symbol_type::template allocate< add_symbol_type >( allocator );
            
            addSymbol = new( addSymbol )
                add_symbol_type( *( root->clone() ),
                                 *( other.root->clone() ) );
            
            return sfract( *addSymbol );
        }

        // SUBTRACTION --------------------------------------------------------

        template< class FType, class Alloc >
        sfract const operator - ( sfract< FType, Alloc > const & other ) const
        {
            // Perform rule reductions

            if( this->root->isValue() )
            {
                val_symbol_base_type * thisVal =
                    reinterpret_cast< val_symbol_base_type * >( this->root );
                
                if( thisVal->getValue() == ZERO ) // 0 - [other]
                {
                    return other.negation();
                }
            }
            
            if( other.root->isValue() )
            {
                val_symbol_base_type * otherVal =
                    reinterpret_cast< val_symbol_base_type * >( other.root );
                
                if( otherVal->getValue() == ZERO ) // [this] - 0
                {
                    return *this;
                }
            }
            
            // Create new negation symbol

            neg_symbol_type * negSymbol =
                symbol_type::template allocate< neg_symbol_type >( allocator );

            negSymbol =
                new( negSymbol ) neg_symbol_type( *( other.root->clone() ) );

            // Create new addition symbol and return

            add_symbol_type * addSymbol =
                symbol_type::template allocate< add_symbol_type >( allocator );

            addSymbol = new( addSymbol )
                add_symbol_type( *( this->root->clone() ), *negSymbol );

            return sfract( *addSymbol );
        }

        // MULTIPLICATION -----------------------------------------------------

        template< class FType, class Alloc >
        sfract const operator * ( sfract< FType, Alloc > const & other ) const
        {
            // Perform rule reductions

            if( this->root->isValue() )
            {
                val_symbol_base_type * thisVal =
                    reinterpret_cast< val_symbol_base_type * >( this->root );

                fract_type value = thisVal->getValue();

                if( value == ZERO ) // 0 * [other]
                {
                    return ZERO;
                }

                if( value == ONE ) // (+-)1 * [other]
                {
                    if( value.isPositive() ) // 1 * [other]
                    {
                        return other;
                    }
                    else // (-1) * [other]
                    {
                        return other.negation();
                    }
                }
            }

            if( other.root->isValue() )
            {
                val_symbol_base_type * otherVal =
                    reinterpret_cast< val_symbol_base_type * >( other.root );

                fract_type value = otherVal->getValue();

                if( value == ZERO ) // [this] * 0
                {
                    return ZERO;
                }

                if( value == ONE ) // [this] * (+-)1
                {
                    if( value.isPositive() ) // [this] * 1
                    {
                        return *this;
                    }
                    else // [this] * (-1)
                    {
                        return this->negation();
                    }
                }
            }

            // Construct new tree with a multiplication symbol as root

            mul_symbol_type * mulSymbol =
                symbol_type::template allocate< mul_symbol_type >( allocator );

            mulSymbol = new( mulSymbol )
                mul_symbol_type( *( this->root->clone() ),
                                 *( other.root->clone() ) );

            return sfract( *mulSymbol );
        }

        // DIVISION -----------------------------------------------------------
        
        template< class FType, class Alloc >
        sfract const operator / ( sfract< FType, Alloc > const & other ) const
        {
            // Perform rule reductions

            if( this->root->isValue() )
            {
                val_symbol_base_type * thisVal =
                    reinterpret_cast< val_symbol_base_type * >( this->root );

                fract_type value = thisVal->getValue();

                if( value == ONE ) // (+-)1 / [other]
                {
                    if( value.isPositive() ) // 1 / [other]
                    {
                        return other.reciprocal();
                    }
                    else // (-1) / [other]
                    {
                        return ( other.reciprocal() ).negation();
                    }
                }
            }

            if( other.root->isValue() )
            {
                val_symbol_base_type * otherVal =
                    reinterpret_cast< val_symbol_base_type * >( other.root );

                fract_type value = otherVal->getValue();

                if( value == ONE ) // [this] / (+-)1
                {
                    if( value.isPositive() ) // [this] / 1
                    {
                        return *this;
                    }
                    else // [this] / (-1)
                    {
                        return this->negation();
                    }
                }
            }

            // Create new reciprocal symbol

            rec_symbol_type * recSymbol =
                symbol_type::template allocate< rec_symbol_type >( allocator );

            recSymbol =
                new( recSymbol ) rec_symbol_type( *( other.root->clone() ) );

            // Create new multiplication symbol and return

            mul_symbol_type * mulSymbol =
                symbol_type::template allocate< mul_symbol_type >( allocator );

            mulSymbol = new( mulSymbol )
                mul_symbol_type( *( this->root->clone() ), *recSymbol );

            return sfract( *mulSymbol );
        }

        // UNARY MINUS --------------------------------------------------------

        sfract const operator - ( void ) const
        {
            return this->negation();
        }

        // COMPOUND ASSIGNMENT ------------------------------------------------

        template< class FType, class Alloc >
        sfract const & operator += ( sfract< FType, Alloc > const & right )
        {
            return ( *this ) = ( *this ) + right;
        }

        template< class FType, class Alloc >
        sfract const & operator -= ( sfract< FType, Alloc > const & right )
        {
            return ( *this ) = ( *this ) - right;
        }

        template< class FType, class Alloc >
        sfract const & operator *= ( sfract< FType, Alloc > const & right )
        {
            return ( *this ) = ( *this ) * right;
        }

        template< class FType, class Alloc >
        sfract const & operator /= ( sfract< FType, Alloc > const & right )
        {
            return ( *this ) = ( *this ) / right;
        }

        // SIMPLIFY -----------------------------------------------------------

        sfract const operator () ( void ) const
        {
            return sfract( evaluate() );
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC DATA ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // SYMBOLIC FRACTION CONSTANTS ----------------------------------------

        static sfract const PI;
        static sfract const HALF_PI;
        static sfract const TWO_PI;
        static sfract const E;

        // FRACTION CONSTANTS -------------------------------------------------

        static fract_type const ZERO;
        static fract_type const ONE;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           
        private:

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE METHODS ++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // CONSTRUCTORS -------------------------------------------------------

        sfract( symbol_type & root )
        {
            this->root = &root;
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ALLOCATORS ---------------------------------------------------------
        
        static allocator_type allocator;
            
        // ROOT SYMBOL --------------------------------------------------------
            
        symbol_type * root;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STATIC DATA OF SYMBOLIC FRACTION CLASS +++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    sfract< FractType, Allocator >::PI = 
    sfract< FractType, Allocator >
    ( sfract< FractType, Allocator >::fract_type::PI );

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    sfract< FractType, Allocator >::HALF_PI = 
    sfract< FractType, Allocator >
    ( sfract< FractType, Allocator >::fract_type::HALF_PI );

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    sfract< FractType, Allocator >::TWO_PI = 
    sfract< FractType, Allocator >
    ( sfract< FractType, Allocator >::fract_type::TWO_PI );

    template< class FractType, class Allocator >
    sfract< FractType, Allocator > const
    sfract< FractType, Allocator >::E = 
    sfract< FractType, Allocator >
    ( sfract< FractType, Allocator >::fract_type::E );

    template< class FractType, class Allocator >
    typename sfract< FractType, Allocator >::fract_type const
    sfract< FractType, Allocator >::ZERO =
    typename sfract< FractType, Allocator >::fract_type( 0, true );

    template< class FractType, class Allocator >
    typename sfract< FractType, Allocator >::fract_type const
    sfract< FractType, Allocator >::ONE = 
    typename sfract< FractType, Allocator >::fract_type( 1, true );

    template< class FractType, class Allocator >
    typename sfract< FractType, Allocator >::allocator_type
    sfract< FractType, Allocator >::allocator;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // SFRACT_H
