#ifndef OBJECT_TRAITS_H
#define    OBJECT_TRAITS_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// NAMESPACE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

namespace memory
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // OBJECT TRAITS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    template< class Type >
    class ObjectTraits
    {
        public:
            
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC TYPES +++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
        template< class OtherType >
        struct rebind
        {
            typedef ObjectTraits< OtherType > other;
        };
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // PUBLIC METHODS +++++++++++++++++++++++++++++++++++++++++++++++++++++
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
        // CONSTRUCTORS -------------------------------------------------------
        
        inline explicit ObjectTraits( void ){}
        
        // COPY CONSTRUCTOR ---------------------------------------------------
        
        inline explicit ObjectTraits( ObjectTraits const & other ){}

        template< class OtherType >
        inline explicit
        ObjectTraits( ObjectTraits< OtherType > const & other ){}
        
        // MOVE CONSTRUCTOR ---------------------------------------------------
        
        inline explicit ObjectTraits( ObjectTraits && other ){}

        template< class OtherType >
        inline explicit ObjectTraits( ObjectTraits< OtherType > && other ){}
        
        // DESTRUCTOR ---------------------------------------------------------
        
        inline ~ObjectTraits( void ){}
        
        // ADDRESS ------------------------------------------------------------
        
        inline Type * address( Type & object ) const
        {
            return &object;
        }
        
        inline Type const * address( Type const & object ) const
        {
            return &object;
        }
        
        // CONSTRUCT ----------------------------------------------------------
        
        inline void construct( Type * address, Type const & object )
        {
            new( address ) Type( object ); // placement new
        }
        
        // DESTROY ------------------------------------------------------------
        
        inline void destroy( Type * address )
        {
            address->~Type();
        }
        
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // OBJECT_TRAITS_H
