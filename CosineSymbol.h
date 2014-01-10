#ifndef COSINE_SYMBOL_H
#define COSINE_SYMBOL_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "ValueSymbol.h"
#include "Symbol.h"
#include "fract.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace fract
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // COSINE SYMBOL ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template< class ValueType, class Allocator >
    class CosineSymbol : public Symbol< ValueType >
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef ValueType value_type;
        typedef Allocator allocator_type;
        typedef Symbol< value_type > symbol_type;
        typedef typename symbol_type::size_type size_type;
        typedef typename allocator_type::value_type alloc_val_type;
        typedef ValueSymbol< value_type, allocator_type > val_symbol_type;
        typedef ValueSymbolBase< value_type > val_symbol_base_type;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // CONSTRUCTORS -------------------------------------------------------

        CosineSymbol( symbol_type & child )
        {
            this->child = &child;
        }

        // COPY CONSTRUCTOR ---------------------------------------------------

        CosineSymbol( CosineSymbol const & other )
        {
            this->child = other.child->clone();
        }

        // MOVE CONSTRUCTOR ---------------------------------------------------

        CosineSymbol( CosineSymbol && other )
        {
            this->child = other.child;

            other.child = nullptr;
        }

        // DESTRUCTOR ---------------------------------------------------------

        virtual ~CosineSymbol( void )
        {
            symbol_type::deallocate( allocator, child );
        }

        // EVALUATE -----------------------------------------------------------

        virtual val_symbol_base_type * evaluate( void )
        {
            // Evaluate child symbol

            val_symbol_type * evalChild =
                reinterpret_cast< val_symbol_type * >( child->evaluate() );

            // Construct new value symbol for result

            val_symbol_type * result =
                symbol_type::template allocate< val_symbol_type >( allocator );

            result = new( result )
                val_symbol_type( cos( evalChild->getValue() ) );

            // Replace old child with evaluation

            symbol_type::deallocate( allocator, child );
            child = evalChild;

            return result;
        }

        virtual val_symbol_base_type * evaluate( void ) const
        {
            // Evaluate child symbol

            val_symbol_type * evalChild =
                reinterpret_cast< val_symbol_type * >( child->evaluate() );

            // Construct new value symbol for result

            val_symbol_type * result =
                symbol_type::template allocate< val_symbol_type >( allocator );

            result = new( result )
                val_symbol_type( cos( evalChild->getValue() ) );

            // Deconstruct and deallocate evaluation

            symbol_type::deallocate( allocator, evalChild );

            return result;
        }

        // SIZE ---------------------------------------------------------------

        virtual size_type const size( void ) const
        {
            return sizeof( CosineSymbol );
        }

        // CLONE --------------------------------------------------------------

        virtual symbol_type * clone( void ) const
        {
            // Return copy of this object

            CosineSymbol * address = 
                symbol_type::template allocate< CosineSymbol >( allocator );

            address = new( address ) CosineSymbol( *this );

            return address;
        }

        // IS VALUE -----------------------------------------------------------
        
        virtual bool const isValue( void ) const
        {
            return false;
        }
        
        // GET ALLOCATOR ------------------------------------------------------
        
        static allocator_type get_allocator( void )
        {
            return allocator;
        }

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           
        private:
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ALLOCATOR ----------------------------------------------------------
            
        static allocator_type allocator;
            
        // OPERAND ------------------------------------------------------------
        
        symbol_type * child;
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STATIC DATA OF COSINE SYMBOL +++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template< class ValueType, class Allocator >
    typename CosineSymbol< ValueType, Allocator >::allocator_type
    CosineSymbol< ValueType, Allocator >::allocator;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // COSINE_SYMBOL_H
