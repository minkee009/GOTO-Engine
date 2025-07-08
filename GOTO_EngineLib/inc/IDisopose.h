#pragma once
namespace GOTOEngine
{
	class IDispose
	{
	public:
		virtual ~IDispose() = default;
		// °´Ã¼¸¦ ÆÄ±«ÇÏ´Â ÇÔ¼ö
		virtual void Dispose() = 0;
	};
}