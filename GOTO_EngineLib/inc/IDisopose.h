#pragma once
namespace GOTOEngine
{
	class IDispose
	{
	public:
		virtual ~IDispose() = default;
		// 객체를 파괴하는 함수
		virtual void Dispose() = 0;
		// 객체를 즉시 파괴하는 함수
		virtual void DisposeImmediate() = 0;
	};
}