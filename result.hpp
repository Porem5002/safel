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

    template<typename V, typename E>
    class result
    {
        result_mode m_mode;

        union
        {
            V m_value;
            E m_error;
        } as;

    public:
        static constexpr result<V, E> ok(V value)
        {
            result<V,E> r;
            r.m_mode = result_mode::OK;
            r.as.m_value = value;
            return r;
        }

        static constexpr result<V, E> err(E error)
        {
            result<V,E> r;
            r.m_mode = result_mode::ERR;
            r.as.m_error = error;
            return r;
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
                out = this->as.m_value;
                return true;
            }

            return false;
        }

        constexpr bool try_unwrap_err(E& out) const
        {
            if(is_err())
            {
                out = this->as.m_error;
                return true;
            }

            return false;
        }

        constexpr V unwrap_ok() const
        {
            assert(is_ok());
            return this->as.m_value;
        }

        constexpr E unwrap_err() const
        {
            assert(is_err());
            return this->as.m_error;
        }
    };
}

#endif