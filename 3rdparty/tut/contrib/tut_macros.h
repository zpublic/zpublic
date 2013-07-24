#ifndef TUT_MACROS_H
#define TUT_MACROS_H

#include <tut/tut.hpp>

#ifdef ensure_THROW
#error ensure_THROW macro is already defined
#endif

/**
 * \brief Helper macro to ensure a call throws exception
 * \author Michal Rzechonek, NSN
 * Example code usage:
 * \code
 *  #include <tut_macros.h>
 *  ensure_THROW( this_function_should_throw_bad_alloc(), std::bad_alloc );
 * \endcode
 */
#define ensure_THROW( x, e ) \
try         \
{           \
    x;      \
    ensure(#x " has not thrown " #e, false); \
}                \
catch(const e &) \
{                \
}                \
catch(...)       \
{                \
    ensure(#x " has thrown unknown exception", false); \
}

#ifdef ensure_NO_THROW
#error ensure_NO_THROW macro is already defined
#endif

/**
 * \brief Helper macro to ensure a call does not throw any exception
 * \author Michal Rzechonek, NSN
 * Example code usage:
 * \code
 *  #include <tut_macros.h>
 *  ensure_NO_THROW( this_function_should_never_throw() );
 * \endcode
 */
#define ensure_NO_THROW( x ) \
try         \
{           \
    x;      \
}           \
catch(...)  \
{           \
    ensure(#x " has thrown unknown exception", false); \
}

#endif
