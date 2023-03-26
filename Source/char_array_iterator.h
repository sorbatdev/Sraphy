#pragma once

namespace sraphy
{
    template<typename T, std::size_t Size>
    class CharArrayConstIterator
    {
    public:
        using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T const*;
        using reference         = T const&;
    public:
        constexpr CharArrayConstIterator() = default;
        constexpr CharArrayConstIterator(pointer p_Ptr, std::size_t p_Offset)
            : m_Ptr{p_Ptr},
              m_Offset{p_Offset}
        {}
    public:
        constexpr reference operator*() const noexcept
        {
            return *m_Ptr;
        }

        constexpr pointer operator->() const noexcept
        {
            return m_Ptr;
        }

        constexpr CharArrayConstIterator& operator++() noexcept
        {
            ++m_Ptr;
            return *this;
        }

        constexpr CharArrayConstIterator operator++(int) noexcept
        {
            CharArrayConstIterator tmp = *this;
            ++m_Ptr;
            return tmp;
        }

        constexpr CharArrayConstIterator& operator--() noexcept
        {
            --m_Ptr;
            return *this;
        }

        constexpr CharArrayConstIterator operator--(int) noexcept
        {
            CharArrayConstIterator tmp = *this;
            --m_Ptr;
            return tmp;
        }

        constexpr CharArrayConstIterator& operator+=(std::ptrdiff_t const p_Offset) noexcept
        {
            m_Ptr += p_Offset;
            return *this;
        }

        constexpr CharArrayConstIterator& operator-=(std::ptrdiff_t const p_Offset) noexcept
        {
            m_Ptr -= p_Offset;
            return *this;
        }

        constexpr std::ptrdiff_t operator-(CharArrayConstIterator const& _Right) const noexcept
        {
            return m_Ptr - _Right.m_Ptr;
        }

        constexpr reference operator[](std::ptrdiff_t const p_Offset) const noexcept
        {
            return m_Ptr[p_Offset];
        }

        constexpr bool operator==(CharArrayConstIterator const& _Right) const noexcept
        {
            return m_Ptr == _Right.m_Ptr;
        }

        constexpr std::strong_ordering operator<=>(CharArrayConstIterator const& _Right) const noexcept
        {
            return m_Ptr <=> _Right.m_Ptr;
        }

        constexpr CharArrayConstIterator operator+(ptrdiff_t const p_Offset) const noexcept
        {
            CharArrayConstIterator tmp = *this;
            tmp += p_Offset;
            return tmp;
        }

        constexpr CharArrayConstIterator operator-(ptrdiff_t const p_Offset) const noexcept
        {
            CharArrayConstIterator tmp = *this;
            tmp -= p_Offset;
            return tmp;
        }

        friend constexpr CharArrayConstIterator operator+(ptrdiff_t const p_Offset, CharArrayConstIterator pp_Next) noexcept
        {
            pp_Next += p_Offset;
            return pp_Next;
        }

        constexpr pointer unwrapped() const noexcept
        {
            return m_Ptr;
        }
    private:
        pointer     m_Ptr{};
        std::size_t m_Offset{};
    };

    template<typename T, std::size_t Size>
    class CharArrayIterator : public CharArrayConstIterator<T, Size>
    {
    public:
        using base              = CharArrayConstIterator<T, Size>;
        using iterator_concept  = std::contiguous_iterator_tag;
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    public:
        constexpr CharArrayIterator() = default;
        constexpr CharArrayIterator(pointer p_Ptr, std::size_t p_Offset)
            : base(p_Ptr, p_Offset)
        {}
    public:
        constexpr reference operator*() const noexcept
        {
            return const_cast<reference>(base::operator*());
        }

        constexpr pointer operator->() const noexcept
        {
            return const_cast<reference>(base::operator->());
        }

        constexpr CharArrayIterator& operator++() noexcept
        {
            base::operator++();
            return *this;
        }

        constexpr CharArrayIterator operator++(int) noexcept
        {
            CharArrayIterator tmp = *this;
            base::operator++(0);
            return tmp;
        }

        constexpr CharArrayIterator& operator--() noexcept
        {
            base::operator--();
            return *this;
        }

        constexpr CharArrayIterator operator--(int) noexcept
        {
            CharArrayIterator tmp = *this;
            base::operator--();
            return tmp;
        }

        constexpr CharArrayIterator& operator+=(const ptrdiff_t p_Offset) noexcept
        {
            base::operator+=(p_Offset);
            return *this;
        }

        constexpr CharArrayIterator operator+(const ptrdiff_t p_Offset) const noexcept
        {
            CharArrayIterator tmp = *this;
            tmp += p_Offset;
            return tmp;
        }

        friend constexpr CharArrayIterator operator+(const ptrdiff_t p_Offset, CharArrayIterator p_Next) noexcept
        {
            p_Next += p_Offset;
            return p_Next;
        }

        constexpr CharArrayIterator& operator-=(const ptrdiff_t p_Offset) noexcept
        {
            base::operator-=(p_Offset);
            return *this;
        }

        using base::operator-;

        constexpr CharArrayIterator operator-(const ptrdiff_t p_Offset) const noexcept
        {
            CharArrayIterator tmp = *this;
            tmp -= p_Offset;
            return tmp;
        }

        constexpr reference operator[](const ptrdiff_t p_Offset) const noexcept
        {
            return const_cast<reference>(base::operator[](p_Offset));
        }


        constexpr pointer unwrapped() const noexcept
        {
            return const_cast<pointer>(base::unwrapped());
        }
    };
}// namespace sraphy

namespace std
{
    template<typename T, size_t Size>
    struct pointer_traits<sraphy::CharArrayConstIterator<T, Size>>
    {
        using pointer         = sraphy::CharArrayConstIterator<T, Size>;
        using element_type    = const char;
        using difference_type = ptrdiff_t;

        static constexpr element_type* to_address(pointer const p_Iter) noexcept
        {
            return p_Iter.unwrapped();
        }
    };

    template<typename T, size_t Size>
    struct pointer_traits<sraphy::CharArrayIterator<T, Size>>
    {
        using pointer         = sraphy::CharArrayIterator<T, Size>;
        using element_type    = T;
        using difference_type = ptrdiff_t;

        static constexpr element_type* to_address(pointer const p_Iter) noexcept
        {
            return p_Iter.unwrapped();
        }
    };
}