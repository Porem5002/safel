#ifndef SAFEL_RESULT_HEADER
#define SAFEL_RESULT_HEADER

#include <cassert>

namespace safel
{
    enum class result_mode
    {
        OK,
        ERR,
    };

    template<typename V>
    struct result_ok
    {
        V value;
    };

    template<typename E>
    struct result_err
    {
        E error;
    };

    template<typename V, typename E>
    class result
    {
        result_mode m_mode;

        union
        {
            V m_value;
            E m_error;
        };

    public:
        constexpr result(result_ok<V> r_ok) : m_mode(result_mode::OK), m_value(r_ok.value) { }

        constexpr result(result_err<E> r_err) : m_mode(result_mode::ERR), m_error(r_err.error) { }

        static constexpr result<V, E> ok(V value)
        {
            return result<V, E>(result_ok { value });
        }

        static constexpr result<V, E> err(E error)
        {
            return result<V, E>(result_err { error });
        } 

        constexpr result_mode mode() const
        {
            return this->m_mode;
        }

        constexpr bool is_ok() const
        {
            return mode() == result_mode::OK;
        }

        constexpr bool is_err() const
        {
            return mode() == result_mode::ERR;
        }

        constexpr bool try_unwrap_ok(V& out) const
        {
            if(is_ok())
            {
                out = this->m_value;
                return true;
            }

            return false;
        }

        constexpr bool try_unwrap_err(E& out) const
        {
            if(is_err())
            {
                out = this->m_error;
                return true;
            }

            return false;
        }

        constexpr V unwrap_ok() const
        {
            assert(is_ok());
            return this->m_value;
        }

        constexpr E unwrap_err() const
        {
            assert(is_err());
            return this->m_error;
        }
    };

    template<typename V>
    constexpr result_ok<V> ok(V value)
    {
        return result_ok<V> { value };
    }

    template<typename E>
    constexpr result_err<E> err(E error)
    {
        return result_err<E> { error };
    }
}

#endif