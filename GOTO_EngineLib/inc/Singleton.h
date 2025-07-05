#pragma once
#include <memory>

namespace GOTOEngine
{
    template <typename T>
    class Singleton
    {
    private:
        static std::unique_ptr<T> m_instance;
    public:
        // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        static T* Get()
        {
            if (!m_instance)
            {
                // �⺻ �����ڸ� ȣ���ϴ� ���
                // �Ļ� Ŭ�������� �ʿ信 ���� Initialize �޼��� �߰� ����
                m_instance = std::unique_ptr<T>(new T());
            }
            return m_instance.get();
        }

        static void DestroyInstance()
        {
            m_instance.reset();
        }
    protected:
        // �����ڴ� protected�� �Ļ� Ŭ������ ���� ����
        Singleton() {}
        // ���� �Ҹ���
        virtual ~Singleton() {}
    };

    template <typename T>
    std::unique_ptr<T> Singleton<T>::m_instance = nullptr;
}