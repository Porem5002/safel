#ifndef SAFEL_OPTION_HEADER
#define SAFEL_OPTION_HEADER

#include <cassert>

namespace safel
{
    enum class option_mode
    {
        SOME,
        NONE,
    };

    template<typename V>
    class option
    {
        option_mode m_mode;
        V m_value;

    public:
        static constexpr option<V> some(V value)
        {
            option<V> o;
            o.m_mode = option_mode::SOME;
            o.m_value = value;
            return o;
        }

        static constexpr option<V> none()
        {
            option<V> o;
            o.m_mode = option_mode::NONE;
            return o;
        } 

        constexpr option_mode mode() const
        {
            return this->m_mode;
        }

        constexpr bool is_some() const
        {
            return mode() == option_mode::SOME;
        }

        constexpr bool is_none() const
        {
            return mode() == option_mode::NONE;
        }

        constexpr bool try_unwrap(V& out) const
        {
            if(is_some())
            {
                out = this->m_value;
                return true;
            }

            return false;
        }

        constexpr V unwrap() const
        {
            assert(is_some());
            return this->m_value;
        }
    };
}

#endif