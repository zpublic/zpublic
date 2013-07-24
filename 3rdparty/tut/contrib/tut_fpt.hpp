/**
 * @brief  Additional ensures for scientific/engineering applications.
 * @author Joerg <yogi2005@users.sourceforge.net>
 * @date   07/04/2008
 */
#ifndef TUT_FPT_H_GUARD
#define TUT_FPT_H_GUARD

#include <limits>

namespace tut
{
    namespace detail
    {
        template< typename FPT >
        inline FPT fpt_abs( FPT arg )
        {
            return arg < static_cast< FPT >( 0 ) ? - arg : arg;
        }

        template< typename FPT >
        struct fpt_limits
        {
            static FPT min_value()
            {
                return std::numeric_limits< FPT >::is_specialized
                    ? std::numeric_limits< FPT >::min()
                    : 0;
            }

            static FPT max_value()
            {
                return std::numeric_limits< FPT >::is_specialized
                    ? std::numeric_limits< FPT >::max()
                    : static_cast< FPT >( 1000000 );
            }
        };

        template< typename FPT >
        inline FPT safe_fpt_division( FPT f1, FPT f2 )
        {
            // Avoid overflow
            if (    f2 < static_cast< FPT >( 1 )
                 && f1 > f2 * fpt_limits< FPT >::max_value() )
            {
                return fpt_limits< FPT >::max_value();
            }

            // Avoid underflow
            if (    f1 == static_cast< FPT >( 0 )
                 || (    f2 > static_cast< FPT >( 1 )
                      && f1 < f2 * fpt_limits< FPT >::min_value() ) )
            {
                return static_cast< FPT >( 0 );
            }

            return f1 / f2;
        }

        template< typename FPT >
        struct percent_tolerance_t
        {
            explicit percent_tolerance_t( FPT v ) : m_value( v ) {}
            FPT m_value;
        };

        template< typename FPT >
        inline percent_tolerance_t< FPT > percent_tolerance( FPT v )
        {
            return percent_tolerance_t< FPT >( v );
        }

        template< typename FPT >
        struct fraction_tolerance_t
        {
            explicit fraction_tolerance_t( FPT v ) : m_value( v ) {}
            FPT m_value;
        };

        template< typename FPT >
        inline fraction_tolerance_t< FPT > fraction_tolerance( FPT v )
        {
            return fraction_tolerance_t< FPT >( v );
        }

        template< typename FPT >
        struct close_at_tolerance
        {
            template< typename ToleranceBaseType >
            explicit close_at_tolerance( percent_tolerance_t< ToleranceBaseType > tolerance )
            : _fraction_tolerance( fpt_abs( static_cast< FPT >( 0.01 ) * tolerance.m_value ) )
            {}

            template< typename ToleranceBaseType >
            explicit close_at_tolerance( fraction_tolerance_t< ToleranceBaseType > tolerance )
            : _fraction_tolerance( fpt_abs( tolerance.m_value ) )
            {}

            bool operator()( FPT left, FPT right ) const
            {
                FPT diff = fpt_abs( left - right );
                FPT d1 = safe_fpt_division( diff, fpt_abs( right ) );
                FPT d2 = safe_fpt_division( diff, fpt_abs( left ) );
                return d1 <= _fraction_tolerance && d2 <= _fraction_tolerance;
            }

            const FPT _fraction_tolerance;
        };

        struct check_is_close
        {
            template< typename FPT, typename ToleranceBaseType >
            bool operator()( FPT left, FPT right, percent_tolerance_t< ToleranceBaseType > tolerance )
            {
                close_at_tolerance< FPT > pred( tolerance );
                return pred( left, right );
            }

            template< typename FPT, typename ToleranceBaseType >
            bool operator()( FPT left, FPT right, fraction_tolerance_t< ToleranceBaseType > tolerance )
            {
                close_at_tolerance< FPT > pred( tolerance );
                return pred( left, right );
            }
        };
    } // namespace detail

    template< typename T, typename ToleranceBaseType >
    void ensure_close( const char* msg, const T& actual, const T& expected, const ToleranceBaseType& tolerance )
    {
        if ( ! detail::check_is_close()( actual, expected, detail::percent_tolerance( tolerance ) ) )
        {
            std::stringstream ss;
            ss << ( msg ? msg : "" )
            << ( msg ? ": " : "" )
            << "expected ("
            << expected
            << ") and actual ("
            << actual
            << ") differ more than "
            << tolerance
            << "%";
             throw failure( ss.str().c_str() );
        }
    }

    template< typename T, typename ToleranceBaseType >
    void ensure_close( const T& actual, const T& expected, const ToleranceBaseType& tolerance )
    {
        ensure_close<>( 0, actual, expected, tolerance );
    }

    template< typename T, typename ToleranceBaseType >
    void ensure_close_fraction( const char* msg, const T& actual, const T& expected, const ToleranceBaseType& tolerance )
    {
        if ( ! detail::check_is_close()( actual, expected, detail::fraction_tolerance( tolerance ) ) )
        {
            std::stringstream ss;
            ss << ( msg ? msg : "" )
            << ( msg ? ": " : "" )
            << "expected ("
            << expected
            << ") and actual ("
            << actual
            << ") differ more than fraction ("
            << tolerance
            << ")";
            throw failure( ss.str().c_str() );
        }
    }

    template< typename T, typename ToleranceBaseType >
    void ensure_close_fraction( const T& actual, const T& expected, const ToleranceBaseType& tolerance )
    {
        ensure_close_fraction<>( 0, actual, expected, tolerance );
    }

} // namespace tut

#endif

