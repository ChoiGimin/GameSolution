#include "StdAfx.h"
#include "lighting.h"

using namespace graphic;

void lighting::InitLighting(const D3DXCOLOR &light,
	_D3DLIGHTTYPE type, float ambient, float diffuse, float specular, const Vector3 lightDirection)
{
	D3DXCOLOR color(light);
	ZeroMemory(&m_Light, sizeof(m_Light));
	m_Light.Type = type;
	m_Light.Ambient = color * ambient;
	m_Light.Diffuse = color * diffuse;
	m_Light.Specular = color * specular;
	Vector3 lightDir(lightDirection);
	lightDir.Normalize();
	m_Light.Direction = *(D3DXVECTOR3*)&lightDir;	
}

void lighting::Bind()
{
//	GetDevice()->SetLight(&m_Light);
}