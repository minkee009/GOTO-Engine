#pragma once
namespace GOTOEngine
{
	class IDispose
	{
	public:
		virtual ~IDispose() = default;
		// ��ü�� �ı��ϴ� �Լ�
		virtual void Dispose() = 0;
		// ��ü�� ��� �ı��ϴ� �Լ�
		virtual void DisposeImmediate() = 0;
	};
}