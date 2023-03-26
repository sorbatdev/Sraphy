#pragma once

namespace sraphy
{
    template<typename T, typename... Args>
    concept Functor = requires {
        std::invocable<T, Args&&...>;
        std::is_class_v<T>;
    };

    template<typename Fn> class Delegate {};

    template<typename Return, typename... Args>
    class Delegate<Return(Args...)>
    {
    private:
        using ReturnType = Return;
        using StubType = ReturnType(*)(void* const, Args&&...);
        
        using FreeFuncPtr = ReturnType(* const)(Args...);

        template<typename T>
        using MethodPtr = ReturnType(T::* const)(Args...);

        template<typename T>
        using ConstMethodPtr = ReturnType(T::* const)(Args...) const;
        
        using FunctorDeleter  = void(*)(void* const);

    public:
        Delegate() = default;

        Delegate(Delegate const&)            = delete;
        Delegate& operator=(Delegate const&) = delete;
        
        Delegate(Delegate&& other) noexcept
        {
            m_ObjectPointer  = std::exchange(other.m_ObjectPointer, nullptr);
            m_StubPointer    = std::exchange(other.m_StubPointer, nullptr); // Not a problem since they are static
            m_FunctorDeleter = std::exchange(other.m_FunctorDeleter, nullptr);
            m_FunctorSize    = std::exchange(other.m_FunctorSize, 0);
            m_IsMoved        = std::exchange(other.m_IsMoved, true);
        }

        Delegate& operator=(Delegate&& rhs) noexcept
        {
            if (&rhs == this) return *this;

            m_ObjectPointer  = std::exchange(rhs.m_ObjectPointer, nullptr);
            m_StubPointer    = std::exchange(rhs.m_StubPointer, nullptr);// Not a problem since they are static
            m_FunctorDeleter = std::exchange(rhs.m_FunctorDeleter, nullptr);
            m_FunctorSize    = std::exchange(rhs.m_FunctorSize, 0);
            m_IsMoved        = std::exchange(rhs.m_IsMoved, true);

            return *this;
        }

        Delegate(std::nullptr_t) noexcept : Delegate() {}

        template<Functor<Args...> F>
        Delegate(F&& functor)
            : m_FunctorSize{sizeof(decltype(functor))}
        {
            m_ObjectPointer = std::malloc(m_FunctorSize);
            new (m_ObjectPointer) F(std::move(functor));
            m_FunctorDeleter = &functor_deleter<F>;
            m_StubPointer    = &functor_stub<F>;
        }

        ~Delegate()
        {
            if (m_FunctorDeleter != nullptr && !m_IsMoved)
            {
                m_FunctorDeleter(m_ObjectPointer);
                std::free(m_ObjectPointer);
            }
        }
    public: 
        // Factory methods
        
        template<FreeFuncPtr ffunc_ptr>
        static Delegate From() noexcept
        {
            return { nullptr, function_stub<ffunc_ptr> };
        }
        
        template<class ObjType, MethodPtr<ObjType> method_ptr>
        static Delegate From(ObjType* const p_ObjPtr) noexcept
        {
            return { p_ObjPtr, method_stub<ObjType, method_ptr> };
        }
        
        template <typename ObjType, MethodPtr<ObjType> method_ptr>
        static Delegate From(ObjType& p_Object) noexcept
        {
            return { &p_Object, method_stub<ObjType, method_ptr> };
        }

        template<class ObjType, ConstMethodPtr<ObjType> method_ptr>
        static Delegate From(ObjType const* const p_ObjPtr) noexcept
        {
            return {const_cast<ObjType*>(&p_ObjPtr), const_method_stub<ObjType, method_ptr> };
        }

        template <typename ObjType, ConstMethodPtr<ObjType> method_ptr>
        static Delegate From(ObjType const& p_Object) noexcept
        {
            return { const_cast<ObjType*>(&p_Object), const_method_stub<ObjType, method_ptr> };
        }

        static Delegate From(Functor<Args...> auto&& f)
        {
            using functor_type = typename std::decay_t<decltype(f)>;
            return Delegate(std::forward<functor_type>(f));
        }

        bool operator==(Delegate const& p_Other) const noexcept
        {
            return (m_ObjectPointer == p_Other.m_ObjectPointer) && (m_StubPointer == p_Other.m_StubPointer);
        }

        bool operator!=(Delegate const& p_Other) const noexcept
        {
            return *this != p_Other;
        }

        bool operator<(Delegate const& p_Other) const noexcept
        {
            return (m_ObjectPointer < p_Other.m_ObjectPointer)
                || ((m_ObjectPointer == p_Other.m_ObjectPointer) && (m_StubPointer < p_Other.m_StubPointer));
        }

        decltype(auto) operator()(Args&&... p_Args) const noexcept
        {
            if constexpr (std::is_same_v<ReturnType, void>)
            {
                if (m_StubPointer) m_StubPointer(m_ObjectPointer, std::forward<Args>(p_Args)...);
            }
            else
            {
                return m_StubPointer(m_ObjectPointer, std::forward<Args>(p_Args)...);
            }
        }
    private:
        Delegate(void* const p_ObjectPtr, StubType const p_StubPtr) noexcept
            : m_ObjectPointer(p_ObjectPtr), m_StubPointer(p_StubPtr)
        {}

        template <FreeFuncPtr ffunc_ptr>
        static ReturnType function_stub(void* const, Args&&... p_Args)
        {
            return ffunc_ptr(std::forward<Args>(p_Args)...);
        }

        template <typename ObjType, MethodPtr<ObjType> method_ptr>
        static ReturnType method_stub(void* const p_ObjectPtr, Args&&... p_Args)
        {
            return (static_cast<ObjType*>(p_ObjectPtr)->*method_ptr)(std::forward<Args>(p_Args)...);
        }

        template <typename ObjType, ConstMethodPtr<ObjType> method_ptr>
        static ReturnType const_method_stub(void* const p_ObjectPtr, Args&&... p_Args)
        {
            return (reinterpret_cast<ObjType const*>(p_ObjectPtr)->*method_ptr)(std::forward<Args>(p_Args)...);
        }

        template<typename T>
        static ReturnType functor_stub(void* const p_FunctorPtr, Args&&... p_Args)
        {
            return static_cast<T*>(p_FunctorPtr)->operator()(std::forward<Args>(p_Args)...);
        }

        template<typename T>
        static void functor_deleter(void* const p)
        {
            static_cast<T*>(p)->~T();
        }

    private:
        void*          m_ObjectPointer{};
        StubType       m_StubPointer{};
        FunctorDeleter m_FunctorDeleter{};
        std::size_t    m_FunctorSize{};
        bool           m_IsMoved{};
    };
}
