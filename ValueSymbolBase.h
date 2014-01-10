#ifndef VALUE_SYMBOL_BASE_H
#define VALUE_SYMBOL_BASE_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "Symbol.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace fract
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // FORWARD DECLARATIONS +++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    template< class ValueType >
    class Symbol;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // SYMBOL +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class ValueType >
    class ValueSymbolBase : public Symbol< ValueType >
    {
        public:

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef ValueType value_type;
        typedef Symbol< ValueType > symbol_type;
        typedef typename symbol_type::val_symbol_base_type
            val_symbol_base_type;
        typedef typename symbol_type::size_type size_type;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // DESTRUCTOR ---------------------------------------------------------

        virtual ~ValueSymbolBase( void ){};

        // EVALUATE -----------------------------------------------------------
        
        virtual val_symbol_base_type * evaluate( void ) = 0;

        virtual val_symbol_base_type * evaluate( void ) const = 0;

        // SIZE ---------------------------------------------------------------
        
        virtual size_type const size( void ) const = 0;
        
        // CLONE --------------------------------------------------------------
        
        virtual symbol_type * clone( void ) const = 0;
        
        // IS VALUE -----------------------------------------------------------
        
        bool const isValue( void ) const
        {
            return true;
        }

        // GET VALUE ----------------------------------------------------------

        virtual value_type const & getValue( void ) const = 0;

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // VALUE_SYMBOL_BASE_H