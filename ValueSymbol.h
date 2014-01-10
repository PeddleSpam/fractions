#ifndef VALUE_SYMBOL_H
#define VALUE_SYMBOL_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "ValueSymbolBase.h"
#include "fract.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace fract
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // VALUE SYMBOL +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class ValueType, class Allocator >
    class ValueSymbol : public ValueSymbolBase< ValueType >
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef ValueType value_type;
        typedef Allocator allocator_type;
        typedef Symbol< value_type > symbol_type;
        typedef ValueSymbolBase< value_type > val_symbol_base_type;
        typedef typename symbol_type::size_type size_type;
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        // CONSTRUCTORS -------------------------------------------------------
        
        ValueSymbol( value_type const & value )
        {
            this->value = value;
        }
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        ValueSymbol( ValueSymbol const & other )
        {
            this->value = other.value;
        }
        
        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        ValueSymbol( ValueSymbol && other )
        {
            this->value = other.value;
        }
        
        // DESTRUCTOR ---------------------------------------------------------
        
        virtual ~ValueSymbol( void ){}
        
        // EVALUATE -----------------------------------------------------------
        
        virtual val_symbol_base_type * evaluate( void )
        {
            // Construct value symbol

            ValueSymbol * address =
                symbol_type::template allocate< ValueSymbol >( allocator );

            address = new( address ) ValueSymbol( *this );
            
            // Return pointer to new value symbol
            
            return address;
        }

        virtual val_symbol_base_type * evaluate( void ) const
        {
            // Construct value symbol

            ValueSymbol * address =
                symbol_type::template allocate< ValueSymbol >( allocator );

            address = new( address ) ValueSymbol( *this );
            
            // Return pointer to new value symbol
            
            return address;
        }
        
        // SIZE ---------------------------------------------------------------
        
        virtual size_type const size( void ) const
        {
            return sizeof( ValueSymbol );
        }
        
        // CLONE --------------------------------------------------------------
        
        virtual symbol_type * clone( void ) const
        {
            // Return copy of this object
            
            ValueSymbol * address =
                symbol_type::template allocate< ValueSymbol >( allocator );
            
            address = new( address ) ValueSymbol( *this );
            
            return address;
        }
        
        // GET VALUE ----------------------------------------------------------
        
        virtual value_type const & getValue( void ) const
        {
            return value;
        }
        
        // GET ALLOCATOR ------------------------------------------------------
        
        static allocator_type get_allocator( void )
        {
            return allocator;
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC OPERATORS +++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // COPY ASSIGNMENT ----------------------------------------------------
        
        ValueSymbol const & operator = ( ValueSymbol const & other )
        {
            this->value = other.value;
            
            return *this;
        }
        
        // MOVE ASSIGNMENT ----------------------------------------------------
        
        ValueSymbol const & operator = ( ValueSymbol && other )
        {
            this->value = other.value;
            
            return *this;
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           
        private:
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ALLOCATOR ----------------------------------------------------------
            
        static allocator_type allocator;
            
        // VALUE --------------------------------------------------------------
            
        value_type value;
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STATIC DATA OF VALUE SYMBOL CLASS ++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class ValueType, class Allocator >
    typename ValueSymbol< ValueType, Allocator >::allocator_type
    ValueSymbol< ValueType, Allocator >::allocator;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // VALUE_SYMBOL_H
