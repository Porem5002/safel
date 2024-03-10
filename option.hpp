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

    struct option_none { };

    constexpr option_none none = {};

    template<typename V>
    class option
    {
        option_mode m_mode;
        V m_value;

        constexpr option(V value) : m_mode(option_mode::SOME), m_value(value) { }

    public:
        constexpr option(option_none) : m_mode(option_mode::NONE), m_value{} { }

        static constexpr option<V> some(V value)
        {
            return option(value);
        }

        static constexpr option<V> none()
        {
            return option<V>(safel::none);
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

    template<typename V>
    constexpr option<V> some(V value)
    {
        return option<V>::some(value);
    }
}

#endif