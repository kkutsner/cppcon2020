// Author: Konstantin Kutsner
//

#pragma once

#include <type_traits>

template <class type_t, std::size_t size>
class basic_placeholder
{
public:
    using opaque_type = type_t *;

    template <typename... args_t>
    explicit basic_placeholder(args_t &&... args)
        : m_ptr{new (m_bytes) type_t{std::forward<args_t>(args)...}}
    {
        static_assert(size > 0);

        // it will print the required size of the underying type in case of error
        actual_size<sizeof(type_t)>();
    }

    basic_placeholder(const basic_placeholder &other)
        : m_ptr{new (m_bytes) type_t{*other.m_ptr}}
    {
    }
    basic_placeholder(basic_placeholder &&other) noexcept
        : m_ptr{new (m_bytes) type_t{std::move(*other.m_ptr)}}
    {
    }
    basic_placeholder &operator=(const basic_placeholder &other)
    {
        if (&other != this)
        {
            (*m_ptr) = *other.m_ptr;
            // m_ptr itself should not change
        }
        return *this;
    }

    basic_placeholder &operator=(basic_placeholder &&other) noexcept
    {
        if (&other != this)
        {
            (*m_ptr) = std::move(*other.m_ptr);
            // m_ptr itself should not change
        }
        return *this;
    }

    ~basic_placeholder() noexcept
    {
        (*m_ptr).~type_t();
    }

    // Ideally the operators below must be explicit, but does it make sense in this particular case
    operator type_t &() noexcept
    {
        return *m_ptr;
    }
    operator const type_t &() const noexcept
    {
        return *m_ptr;
    }
    // I really miss overload dot operator beause the operators below would not be neede
    opaque_type operator->() noexcept
    {
        return m_ptr;
    }

private:
    template <std::size_t actual_size>
    constexpr void actual_size()
    {
        static_assert(size == actual_size, "The size of underlying type does \
        not match the expected size.");
    }
    std::uint8_t m_bytes[size];
    opaque_type m_ptr; // could we avoid having this member?
};

using x86_windows_size = std::size_t;
using x64_windows_size = std::size_t;
template <typename type_t, x86_windows_size s1, x64_windows_size s2>
struct placeholder_size_selector final
{
    static constexpr std::size_t select_size()
    {
        if (sizeof(std::size_t) == 4)
            return x86_windows_size{s1};
        if (sizeof(std::size_t) == 8)
            return x64_windows_size{s2};
        return 0;
    }

    using placeholder_type = basic_placeholder<type_t, select_size()>;
};

template <typename type_t, x86_windows_size s1, x64_windows_size s2>
using placeholder = typename placeholder_size_selector<type_t, s1, s2>::placeholder_type;