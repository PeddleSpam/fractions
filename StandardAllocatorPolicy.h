#ifndef STANDARD_ALLOCATOR_POLICY_H
#define STANDARD_ALLOCATOR_POLICY_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <limits>

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace memory
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STANDARD ALLOCATOR POLICY CLASS ++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class Type >
    class StandardAllocatorPolicy
    {
        public:
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        typedef Type value_type;
        typedef value_type * pointer;
        typedef value_type & reference;
        typedef value_type const * const_pointer;
        typedef value_type const & const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        
        template< class OtherType >
        struct rebind
        {
            typedef StandardAllocatorPolicy< OtherType > other;
        };
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // CONSTRUCTORS -------------------------------------------------------
        
        inline explicit StandardAllocatorPolicy( void ){}
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        inline explicit StandardAllocatorPolicy
        (
            StandardAllocatorPolicy const & other
        ){}

        template< class OtherType >
        inline explicit StandardAllocatorPolicy
        (
            StandardAllocatorPolicy< OtherType > const & other
        ){}
        
        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        inline explicit StandardAllocatorPolicy
        (
            StandardAllocatorPolicy && other
        ){}

        template< class OtherType >
        inline explicit StandardAllocatorPolicy
        (
            StandardAllocatorPolicy< OtherType > && other
        ){}
        
        // DESTRUCTOR ---------------------------------------------------------
        
        inline ~StandardAllocatorPolicy( void ){}
        
        // ALLOCATE -----------------------------------------------------------
        
        inline pointer allocate
        (
            size_type count,
            typename std::allocator< void >::const_pointer hint = 0
        )
        {
            return reinterpret_cast< pointer >
            ( 
                ::operator new( count * sizeof( size_type ) ) 
            );
        }
        
        // DEALLOCATE ---------------------------------------------------------
        
        inline void deallocate( pointer address, size_type count )
        {
            ::operator delete( address );
        }
        
        // MAX SIZE -----------------------------------------------------------
        
        inline size_type const max_size( void ) const
        {
            return std::numeric_limits< size_type >::max() /
                   sizeof( size_type );
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC OPERATORS +++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // EQUALITY -----------------------------------------------------------
        
        template< class OtherType >
        inline bool const
        operator == ( StandardAllocatorPolicy< OtherType > const & other )
        {
            return true;
        }
        
        template< class OtherAllocatorType >
        inline bool const
        operator == ( OtherAllocatorType const & other )
        {
            return false;
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // STANDARD_ALLOCATOR_POLICY_H
