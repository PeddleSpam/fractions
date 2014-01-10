#ifndef ADDITION_SYMBOL_H
#define	ADDITION_SYMBOL_H

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
	// ADDITION SYMBOL CLASS ++++++++++++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	template< class ValueType, class Allocator >
	class AdditionSymbol : public Symbol< ValueType >
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
		
		AdditionSymbol( symbol_type & left, symbol_type & right )
		{
			this->left = &left;
			this->right = &right;
		}
		
		// COPY CONSTRUCTOR ---------------------------------------------------
		
		AdditionSymbol( AdditionSymbol const & other )
		{
			// Copy operands of other
			
			this->left = other.left->clone();
			this->right = other.right->clone();
		}
		
		// MOVE CONSTRUCTOR ---------------------------------------------------
		
		AdditionSymbol( AdditionSymbol && other )
		{
			// Acquire operands of other
			
			this->left = other.left;
			this->right = other.right;
			
			other.left = nullptr;
			other.right = nullptr;
		}
		
		// DESTRUCTOR ---------------------------------------------------------
		
		virtual ~AdditionSymbol( void )
		{
			symbol_type::deallocate( allocator, left );
			symbol_type::deallocate( allocator, right );
		}
		
		// EVALUATE -----------------------------------------------------------
		
		virtual val_symbol_base_type * evaluate( void )
		{
			// Evaluate operands
			
			val_symbol_type * evalLeft =
				reinterpret_cast< val_symbol_type * >( left->evaluate() );
			val_symbol_type * evalRight =
				reinterpret_cast< val_symbol_type * >( right->evaluate() );
			
			// Construct new value symbol using evaluated operands
			
			val_symbol_type * result =
				symbol_type::template allocate< val_symbol_type >( allocator );

			result = new( result ) val_symbol_type( evalLeft->getValue() + 
													evalRight->getValue() );
			
			// Deconstruct and deallocate operands
			
			symbol_type::deallocate( allocator, left );
			symbol_type::deallocate( allocator, right );

			// Replace operands with evaluations

			left = evalLeft;
			right = evalRight;
			
			// Return addition result
			
			return result;
		}

		virtual val_symbol_base_type * evaluate( void ) const
		{
			// Evaluate operands
			
			val_symbol_type * evalLeft =
				reinterpret_cast< val_symbol_type * >( left->evaluate() );
			val_symbol_type * evalRight =
				reinterpret_cast< val_symbol_type * >( right->evaluate() );
			
			// Construct new value symbol using evaluated operands
			
			val_symbol_type * result =
				symbol_type::template allocate< val_symbol_type >( allocator );

			result = new( result ) val_symbol_type( evalLeft->getValue() + 
													evalRight->getValue() );
			
			// Deconstruct and deallocate operands
			
			symbol_type::deallocate( allocator, evalLeft );
			symbol_type::deallocate( allocator, evalRight );
			
			// Return addition result
			
			return result;
		}
		
		// SIZE ---------------------------------------------------------------
		
		virtual size_type const size( void ) const
		{
			return sizeof( AdditionSymbol );
		}
		
		// CLONE --------------------------------------------------------------
		
		virtual symbol_type * clone( void ) const
		{
			// Return copy of this object
			
			AdditionSymbol * address =
				symbol_type::template allocate< AdditionSymbol >( allocator );
			
			address = new( address ) AdditionSymbol( *this );
			
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
			
		// OPERANDS -----------------------------------------------------------
		
		symbol_type * left;
		symbol_type * right;
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	};
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// STATIC DATA OF ADDITION SYMBOL +++++++++++++++++++++++++++++++++++++++++
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	template< class ValueType, class Allocator >
	typename AdditionSymbol< ValueType, Allocator >::allocator_type
	AdditionSymbol< ValueType, Allocator >::allocator;
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // ADDITION_SYMBOL_H
