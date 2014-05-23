
#include "stdafx.h"
#include "renderer.h"
#include "DxInit.h"

using namespace graphic;

bool graphic::InitRenderer(HWND hWnd, const int width, const int height)
{
	if(!cRenderer::getSingleton()->CreateDevice(hWnd, width, height))
		return false;

	return true;
}
void graphic::RealeaseRenderer()
{
	cRenderer::getSingleton()->releaseSingleton();
}
cRenderer* graphic::GetRenderer()
{
	return cRenderer::getSingleton();
}

LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return cRenderer::getSingleton()->GetDevice();
}



//////////////////////////////
////Renderer
cRenderer::cRenderer() {}
cRenderer::~cRenderer() {}

bool cRenderer::CreateDevice(HWND hWnd, const int width, const int height)
{
	if(!InitDirectX(hWnd, width, height, m_pDevice))
	return false;

	return true;
}
LPDIRECT3DDEVICE9 cRenderer::GetDevice()
{
	return m_pDevice;
}