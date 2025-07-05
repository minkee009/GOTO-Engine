#pragma once
#include "Vector2.h" 
#include <map> 

namespace GOTOEngine
{
    enum class KeyCode;

    class IInputSystem
    {
    public:
        virtual ~IInputSystem() = default;

        // OS�� �ʱ�ȭ �Լ� (��: ������ �ڵ� ����)
        virtual void Startup(void* windowHandle) = 0;
        virtual void Update() = 0;
        virtual Vector2 GetMousePos() = 0;
        virtual bool GetKey(KeyCode keyCode) = 0; // OS�� native key code ���
        virtual bool GetKeyDown(KeyCode keyCode) = 0;
        virtual bool GetKeyUp(KeyCode keyCode) = 0;

        // KeyCode�� ����Ƽ�� �ڵ�� ��ȯ�ϴ� ���� ���� �Լ� �߰�
        virtual int GetNativeKeyCode(KeyCode keyCode) const = 0;
    };
}