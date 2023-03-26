#pragma once

#include "char_array_iterator.h"

namespace sraphy
{
    template<std::size_t Size>
        requires (Size <= 1024)
    class CharArray
    {
    public:
        using value_type      = char;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer         = char*;
        using const_pointer   = char const*;
        using reference       = char&;
        using const_reference = char const&;

        using iterator       = CharArrayIterator<char, Size>;
        using const_iterator = CharArrayConstIterator<char, Size>;

        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        constexpr CharArray() = default;
        constexpr CharArray(std::string_view p_Str)
        {
            if (p_Str.size() <= Size - 1)
            {
                std::copy(p_Str.begin(), p_Str.end(), std::begin(m_Elems));
                m_Elems[p_Str.size()] = '\0';
                m_RealSize = p_Str.size();
            }
        }

        constexpr CharArray(CharArray const&) = default;
        constexpr CharArray(CharArray&&) = default;

    public:
        constexpr void fill(char const& p_Value)
        {
            std::fill_n(m_Elems, Size, p_Value);
        }

        constexpr void swap(CharArray& p_Other)
        {
            std::ranges::swap(m_Elems, p_Other.m_Elems);
        }
        
        constexpr auto begin() noexcept -> iterator
        {
            return iterator(m_Elems, 0);
        }

        constexpr auto begin() const noexcept -> const_iterator
        {
            return const_iterator(m_Elems, 0);
        }

        constexpr auto end() noexcept -> iterator
        {
            return iterator(m_Elems, m_RealSize + 1);
        }

        constexpr auto end() const noexcept -> const_iterator
        {
            return const_iterator(m_Elems, m_RealSize + 1);
        }

        constexpr auto cbegin() const noexcept -> const_iterator
        {
            return begin();
        }

        constexpr auto cend() const noexcept -> const_iterator
        {
            return end();
        }

        constexpr auto rbegin() noexcept -> iterator
        {
            return reverse_iterator(end());
        }

        constexpr auto rbegin() const noexcept -> const_reverse_iterator
        {
            return const_reverse_iterator(end());
        }

        constexpr auto rend() noexcept -> iterator
        {
            return reverse_iterator(begin());
        }

        constexpr auto rend() const noexcept -> const_reverse_iterator
        {
            return const_reverse_iterator(begin());
        }

        constexpr auto crbegin() const noexcept -> const_reverse_iterator
        {
            return rbegin();
        }

        constexpr auto crend() const noexcept -> const_reverse_iterator
        {
            return rend();
        }

        constexpr auto size() const noexcept -> size_type
        {
            return m_RealSize;
        }

        constexpr auto max_size() const noexcept -> size_type
        {
            return Size;
        }

        constexpr bool empty() const noexcept
        {
            return false;
        }

        constexpr auto at(size_type p_Index) noexcept -> reference
        {
            return m_Elems[p_Index];
        }

        constexpr auto at(size_type p_Index) const noexcept -> const_reference
        {
            return m_Elems[p_Index];
        }

        constexpr auto front() noexcept -> reference
        {
            return m_Elems[0];
        }

        constexpr auto front() const noexcept -> const_reference
        {
            return m_Elems[0];
        }

        constexpr auto back() noexcept -> reference
        {
            return m_Elems[Size - 1];
        }

        constexpr auto back() const noexcept -> const_reference
        {
            return m_Elems[Size - 1];
        }

        constexpr auto data() const noexcept -> const_pointer
        {
            return m_Elems;
        }

        constexpr auto c_str() const noexcept -> const_pointer
        {
            return m_Elems;
        }

        constexpr auto operator[](size_type p_Index) noexcept -> reference
        {
            return m_Elems[p_Index];
        }

        constexpr auto operator[](size_type p_Index) const noexcept -> reference
        {
            return m_Elems[p_Index];
        }

        constexpr CharArray& operator=(CharArray const& p_Rhs) = default;
        constexpr CharArray& operator=(CharArray&& p_Rhs) noexcept = default;

        constexpr CharArray& operator=(std::string_view p_Str)
        {
            if (p_Str.size() <= Size - 1)
            {
                std::copy(p_Str.begin(), p_Str.end(), std::begin(m_Elems));
                m_Elems[p_Str.size()] = '\0';
                m_RealSize = p_Str.size();
            }

            return *this;
        }

        constexpr operator std::string() const
        {
            std::string s;
            s = m_Elems;
            return s;
        }

    private:
        char m_Elems[Size];
        std::size_t m_RealSize{};
    };
}// namespace sraphy
