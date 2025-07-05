#pragma once  
#include <functional>  
#include <vector>  

namespace GOTOEngine
{
    template <typename T,typename... Args>
    class Delegate
    {
    public:        
        
        struct Callback
        {
            std::function<T(Args...)> func;
            void* instance = nullptr;
            void* identifier = nullptr;

            bool Matches(void* inst, void* id) const
            {
                return instance == inst && identifier == id;
            }
        };

        Delegate() = default;

        // 일반 함수
        void Add(T(*func)(Args...))
        {
            m_callbacks.push_back({ func, nullptr, reinterpret_cast<void*>(func) });
        }

        // 멤버 함수
        template<typename U>
        void Add(U* obj, T(U::* method)(Args...))
        {
            auto lambda = [obj, method](Args... args) -> T {
                return (obj->*method)(args...);
                };
            m_callbacks.push_back({ lambda, obj, *reinterpret_cast<void**>(&method) });
        }

        // 람다
        void Add(std::function<T(Args...)> f)
        {
            m_callbacks.push_back({ std::move(f), nullptr, nullptr });
        }

        // Remove
        void Remove(T(*func)(Args...))
        {
            void* id = reinterpret_cast<void*>(func);
            m_callbacks.erase(
                std::remove_if(m_callbacks.begin(), m_callbacks.end(),
                    [&](const Callback& cb) { return cb.Matches(nullptr, id); }),
                m_callbacks.end());
        }

        template<typename U>
        void Remove(U* obj, T(U::* method)(Args...))
        {
            void* id = *reinterpret_cast<void**>(&method);
            m_callbacks.erase(
                std::remove_if(m_callbacks.begin(), m_callbacks.end(),
                    [&](const Callback& cb) { return cb.Matches(obj, id); }),
                m_callbacks.end());
        }

        // operator+= (함수포인터)
        Delegate& operator+=(T(*func)(Args...))
        {
            Add(func);
            return *this;
        }

        // operator+= (멤버함수)
        template<typename U>
        Delegate& operator+=(std::pair<U*, T(U::*)(Args...)> p)
        {
            Add(p.first, p.second);
            return *this;
        }

        // operator+= (람다 / std::function)
        Delegate& operator+=(std::function<T(Args...)> f)
        {
            Add(std::move(f));
            return *this;
        }

        // operator-= (함수포인터)
        Delegate& operator-=(T(*func)(Args...))
        {
            Remove(func);
            return *this;
        }

        // operator-= (멤버함수)
        template<typename U>
        Delegate& operator-=(std::pair<U*, T(U::*)(Args...)> p)
        {
            Remove(p.first, p.second);
            return *this;
        }

        void Clear()
        {
            m_callbacks.clear();
        }

        T operator()(Args... args) const
        {
            return Invoke(args...);
        }

        T Invoke(Args... args) const
        {
            if (m_callbacks.empty())
                return T();

            for (size_t i = 0; i < m_callbacks.size() - 1; ++i)
                m_callbacks[i].func(args...);
            return m_callbacks.back().func(args...);
        }

    private:
        std::vector<Callback> m_callbacks;
    };

    // --- Specialization for void return type (변경 없음) ---
    template <typename... Args>
    class Delegate<void, Args...>
    {
    public:
        struct Callback
        {
            std::function<void(Args...)> func;
            void* instance = nullptr;
            void* identifier = nullptr;

            bool Matches(void* inst, void* id) const
            {
                return instance == inst && identifier == id;
            }
        };

        Delegate() = default;

        void Add(void(*func)(Args...))
        {
            m_callbacks.push_back({ func, nullptr, reinterpret_cast<void*>(func) });
        }

        template<typename U>
        void Add(U* obj, void(U::* method)(Args...))
        {
            auto lambda = [obj, method](Args... args) {
                (obj->*method)(args...);
                };
            m_callbacks.push_back({ lambda, obj, *reinterpret_cast<void**>(&method) });
        }

        void Add(std::function<void(Args...)> f)
        {
            m_callbacks.push_back({ std::move(f), nullptr, nullptr });
        }

        void Remove(void(*func)(Args...))
        {
            void* id = reinterpret_cast<void*>(func);
            m_callbacks.erase(
                std::remove_if(m_callbacks.begin(), m_callbacks.end(),
                    [&](const Callback& cb) { return cb.Matches(nullptr, id); }),
                m_callbacks.end());
        }

        template<typename U>
        void Remove(U* obj, void(U::* method)(Args...))
        {
            void* id = *reinterpret_cast<void**>(&method);
            m_callbacks.erase(
                std::remove_if(m_callbacks.begin(), m_callbacks.end(),
                    [&](const Callback& cb) { return cb.Matches(obj, id); }),
                m_callbacks.end());
        }

        void Clear()
        {
            m_callbacks.clear();
        }

        // operator+= (함수포인터)
        Delegate& operator+=(void(*func)(Args...))
        {
            Add(func);
            return *this;
        }

        // operator+= (멤버함수)
        template<typename U>
        Delegate& operator+=(std::pair<U*, void(U::*)(Args...)> p)
        {
            Add(p.first, p.second);
            return *this;
        }

        // operator+= (람다 / std::function)
        Delegate& operator+=(std::function<void(Args...)> f)
        {
            Add(std::move(f));
            return *this;
        }

        // operator-= (함수포인터)
        Delegate& operator-=(void(*func)(Args...))
        {
            Remove(func);
            return *this;
        }

        // operator-= (멤버함수)
        template<typename U>
        Delegate& operator-=(std::pair<U*, void(U::*)(Args...)> p)
        {
            Remove(p.first, p.second);
            return *this;
        }

        void operator()(Args... args) const
        {
            Invoke(args...);
        }

        void Invoke(Args... args) const
        {
            for (const auto& callback : m_callbacks)
            {
                callback.func(args...);
            }
        }

    private:
        std::vector<Callback> m_callbacks;
    };
}
