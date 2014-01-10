#ifndef TANGENT_SYMBOL_H
#define TANGENT_SYMBOL_H

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
    class TangentSymbol : public Symbol< ValueType >
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

        TangentSymbol( symbol_type & child )
        {
            this->child = &child;
        }

        // MOVE CONSTRUCTOR ---------------------------------------------------

        TangentSymbol( TangentSymbol && other )
        {
            this->child = other.child;

            other.child = nullptr;
        }

        // DESTRUCTOR ---------------------------------------------------------

        virtual ~TangentSymbol( void )
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
                val_symbol_type( tan( evalChild->getValue() ) );

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
                val_symbol_type( tan( evalChild->getValue() ) );

            // Deconstruct and deallocate evaluation

            symbol_type::deallocate( allocator, evalChild );

            return result;
        }

        // SIZE ---------------------------------------------------------------

        virtual size_type const size( void ) const
        {
            return sizeof( TangentSymbol );
        }

        // CLONE --------------------------------------------------------------

        virtual symbol_type * clone( void ) const
        {
            // Return copy of this object

            TangentSymbol * address = 
                symbol_type::template allocate< TangentSymbol >( allocator );

            address = new( address ) TangentSymbol( *this );

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
    // STATIC DATA OF TANGENT SYMBOL ++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template< class ValueType, class Allocator >
    typename TangentSymbol< ValueType, Allocator >::allocator_type
    TangentSymbol< ValueType, Allocator >::allocator;

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // TANGENT_SYMBOL_H
