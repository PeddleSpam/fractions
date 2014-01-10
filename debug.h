#ifndef DEBUG_H
#define DEBUG_H

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// DEBUGGING MACRO ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if defined( DEBUG ) | defined( _DEBUG )
    #ifndef DEBUG_CODE
        #define DEBUG_CODE( x ) ( x )
    #endif
#else
    #ifndef DEBUG_CODE
        #define DEBUG_CODE( x )
    #endif
#endif

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // DEBUG_H
