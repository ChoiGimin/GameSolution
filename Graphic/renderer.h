
#pragma once

namespace graphic
{
	
	class cRenderer : public common::singletonBase<cRenderer>
	{
	public:
		cRenderer();
		virtual ~cRenderer();

		bool CreateDevice(HWND hWnd, const int width, const int height);
		LPDIRECT3DDEVICE9 GetDevice();

	private:
		LPDIRECT3DDEVICE9 m_pDevice;
	};

	bool InitRenderer(HWND hWnd, const int width, const int height);
	void RealeaseRenderer();
	cRenderer* GetRenderer();
	LPDIRECT3DDEVICE9 GetDevice();
}