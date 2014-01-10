#ifndef POOLED_ALLOCATOR_H
#define POOLED_ALLOCATOR_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// INCLUDES +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <unordered_map>
#include <utility>

#include "StandardAllocatorPolicy.h"
#include "ObjectTraits.h"

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace memory
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // POOLED ALLOCATOR +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template
    <
        class Element,
        class Policy = StandardAllocatorPolicy< Element >,
        class Traits = ObjectTraits< Element >
    >
    class PooledAllocator
    {
        public:
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef Policy AllocationPolicy;
        typedef Traits ElementTraits;
        
        typedef typename AllocationPolicy::value_type value_type;
        typedef typename AllocationPolicy::pointer pointer;
        typedef typename AllocationPolicy::reference reference;
        typedef typename AllocationPolicy::const_pointer const_pointer;
        typedef typename AllocationPolicy::const_reference const_reference;
        typedef typename AllocationPolicy::size_type size_type;
        typedef typename AllocationPolicy::difference_type difference_type;
        /*
        template< class OtherElementType >
        struct rebind
        {
            typedef PooledAllocator
            <
                OtherElementType,
                typename AllocationPolicy::rebind< OtherElementType >::other,
                typename ElementTraits::rebind< OtherElementType >::other
            >
            other;
        };
        */
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // CONSTRUCTORS -------------------------------------------------------
        
        inline explicit PooledAllocator( void ){}
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        inline PooledAllocator( PooledAllocator const & other ):
        policy( other.policy ), traits( other.traits )
        {}

        template
        <
            class OtherElement,
            class OtherPolicy,
            class OtherTraits
        >
        inline PooledAllocator
        (
            PooledAllocator< OtherElement, OtherPolicy, OtherTraits >
            const & other
        ):
        policy( other.policy ), traits( other.traits )
        {}
        
        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        inline PooledAllocator( PooledAllocator && other ):
        policy( other.policy ), traits( other.traits )
        {}

        template
        <
            class OtherElement,
            class OtherPolicy,
            class OtherTraits
        >
        inline PooledAllocator
        (
            PooledAllocator< OtherElement, OtherPolicy, OtherTraits >
            && other
        ):
        policy( other.policy ), traits( other.traits )
        {}
        
        // DESTRUCTOR ---------------------------------------------------------
        
        inline ~PooledAllocator( void )
        {
            clean();
        }
        
        // ADDRESS ------------------------------------------------------------
        
        inline pointer address( reference object ) const
        {
            return traits.address( object );
        }
        
        inline const_pointer address( const_reference object ) const
        {
            return traits.address( object );
        }
        
        // ALLOCATE -----------------------------------------------------------
        
        inline pointer allocate
        (
            size_type count,
            std::allocator< void >::const_pointer hint = 0
        )
        {
            pointer address = nullptr;
            
            // Search for matching size memory block
            
            if( !( buffer.empty() ) )
            {
                buffer_iterator it = buffer.find( count );

                if( it != buffer.end() )
                {
                    address = it->second;
                    buffer.erase( it );
                    
                    return address;
                }
            }
            
            // No matching memory block was found, return new block
            
            address = policy.allocate( count, hint );
            
            return address;
        }
        
        // DEALLOCATE ---------------------------------------------------------
        
        inline void deallocate( pointer address, size_type count )
        {
            // Add memory block to buffer
            
            if( address != nullptr )
            {
                buffer.insert( buffer_entry( count, address ) );
            }
        }
        
        // MAX SIZE -----------------------------------------------------------
        
        inline size_type const max_size( void ) const
        {
            return policy.max_size();
        }
        
        // CONSTRUCT ----------------------------------------------------------
        
        inline void construct( pointer address, const_reference object )
        {
            traits.construct( address, object );
        }
        
        // DESTRUCT -----------------------------------------------------------
        
        inline void destroy( pointer address )
        {
            traits.destroy( address );
        }
        
        // CLEAN --------------------------------------------------------------
        
        void clean( void )
        {
            // Loop through all entries in buffer
            
            buffer_iterator it = buffer.begin();
            
            while( it != buffer.end() )
            {
                // Release memory buffer
                
                policy.deallocate( it->second, it->first );
                
                // Increment iterator
                
                ++it;
            }
            
            // Clear all entries from buffer
            
            buffer.clear();
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC OPERATORS +++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // EQUALITY -----------------------------------------------------------
        
        inline bool const operator == ( PooledAllocator const & other ) const
        {
            return true;
        }
        
        template
        <
            class OtherElement,
            class OtherPolicy,
            class OtherTraits
        >
        inline bool const operator ==
        (
            PooledAllocator
            <
                OtherElement,
                OtherPolicy,
                OtherTraits
            >
            const & other
        )
        const
        {
            return ( this->policy == other.policy );
        }
        
        template< class OtherAllocator >
        inline bool const operator == ( OtherAllocator const & other ) const
        {
            return false;
        }
        
        // INEQUALITY ---------------------------------------------------------
        
        inline bool const operator != ( PooledAllocator const & other ) const
        {
            return false;
        }
        
        template
        <
            class OtherElement,
            class OtherPolicy,
            class OtherTraits
        >
        inline bool const operator !=
        (
            PooledAllocator
            <
                OtherElement,
                OtherPolicy,
                OtherTraits
            >
            const & other
        )
        const
        {
            return ( this->policy == other.policy );
        }
        
        template< class OtherAllocator >
        inline bool const operator != ( OtherAllocator const & other ) const
        {
            true;
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        private:
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE TYPES ++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        typedef std::unordered_multimap< size_type, pointer > buffer_type;
        typedef typename buffer_type::const_iterator buffer_const_iterator;
        typedef typename buffer_type::iterator buffer_iterator;
        typedef typename buffer_type::value_type buffer_entry;
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PRIVATE DATA +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // ALLOCATION POLICY --------------------------------------------------
        
        AllocationPolicy policy;
        
        // ELEMENT TRAITS -----------------------------------------------------
        
        ElementTraits traits;
        
        // FREE BUFFER --------------------------------------------------------
        
        static buffer_type buffer;
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // STATIC DATA OF POOLED ALLOCATOR CLASS ++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class Element, class Policy, class Traits >
    typename PooledAllocator< Element, Policy, Traits >::buffer_type
    PooledAllocator< Element, Policy, Traits >::buffer;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // POOLED_ALLOCATOR_H
