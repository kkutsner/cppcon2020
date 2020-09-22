// Author: Konstantin Kutsner
//

#pragma once

#include <type_traits>

namespace details {

// Helper traits that let examine current build configuration. Whould be cool if there are some
// standard heplers can be used instead of guessing using old fashioned preprocessor. Also, there
// should be potentially more helpers to distinguish different platforms.
#if defined(DEBUG) || defined(_DEBUG)
static constexpr auto debug = true;
#else
static constexpr auto debug = false;
#endif

static constexpr auto release = !debug;
static constexpr auto x86 = sizeof(std::size_t) == 4;
static constexpr auto x64 = sizeof(std::size_t) == 8;

template <typename type_t> constexpr auto is_valid_type()
{
  const auto is_reference = std::is_reference_v<type_t>;
  const auto is_pointer = std::is_pointer_v<type_t>;
  return !is_reference && !is_pointer;
}

template <class type_t, std::size_t size> class basic_placeholder final
{
public:
  static_assert(details::is_valid_type<type_t>(), "References and Pointers are not supported.");

  using opaque_type = type_t*;
  using const_opaque_type = type_t* const;
  using value_type = std::decay_t<type_t>;

  template <typename... args_t>
  explicit basic_placeholder(args_t&&... args) noexcept(
    std::is_nothrow_constructible_v<value_type, args_t...>)
    : m_ptr{new (m_bytes) type_t{std::forward<args_t>(args)...}}
  {
    static_assert(size > 0);

    // it will print the required size of the underying type in case of error
    print_actual_size<sizeof(value_type)>();
  }

  basic_placeholder(const basic_placeholder& other) noexcept(
    std::is_nothrow_copy_constructible_v<value_type>)
    : m_ptr{new (m_bytes) value_type{*other.m_ptr}}
  {
  }
  basic_placeholder(basic_placeholder&& other) noexcept
    : m_ptr{new (m_bytes) value_type{std::move(*other.m_ptr)}}
  {
  }
  basic_placeholder& operator=(const basic_placeholder& other) noexcept(
    std::is_nothrow_assignable_v<value_type, value_type>)
  {
    if (&other != this)
    {
      (*this->m_ptr) = *other.m_ptr;
      // m_ptr itself should not change
    }
    return *this;
  }

  basic_placeholder&
  operator=(basic_placeholder&& other) noexcept(std::is_nothrow_move_assignable_v<value_type>)
  {
    if (&other != this)
    {
      (*this->m_ptr) = std::move(*other.m_ptr);
      // m_ptr itself should not change
    }
    return *this;
  }

  ~basic_placeholder() noexcept
  {
    (*this->m_ptr).~value_type();
  }

  // Ideally the operators below must be explicit, but does it make sense in this particular case
  operator value_type&() noexcept
  {
    return *this->m_ptr;
  }
  operator const value_type&() const noexcept
  {
    return *this->m_ptr;
  }
  // I really miss overload dot operator beause the operators below would not be neede
  opaque_type operator->() noexcept
  {
    return this->m_ptr;
  }
  const_opaque_type operator->() const noexcept
  {
    return m_ptr;
  }

private:
  template <std::size_t actual_size> constexpr void print_actual_size()
  {
    static_assert(size == actual_size, "The size of underlying type does \
        not match the expected size.");
  }
  std::uint8_t m_bytes[size];
  type_t* m_ptr; // could we avoid having this member?
};

// As an example, we just handle couple configurations, but in real life,
// the debug and release build configurations may have different sizes too
using x86_windows_size = std::size_t;
using x64_windows_size = std::size_t;

// This helper trait just to provide a way to select the appropriate size for the
// placeholder based on different criterias
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

} // namespace details

// The following alias is the main alias that should be used to define aliased for private value
// types
template <typename type_t, details::x86_windows_size s1, details::x64_windows_size s2>
using placeholder = typename details::placeholder_size_selector<type_t, s1, s2>::placeholder_type;