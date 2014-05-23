
#include "stdafx.h"
#include "material.h"

using namespace graphic;

cMaterial::cMaterial()
{

}

cMaterial::~cMaterial()
{

}


void cMaterial::Init(const Vector4 &ambient, const Vector4 &diffuse, const Vector4 &specular, const Vector4 &emisive, const float power)
{
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_mtrl.Ambient = *(D3DXCOLOR*) &ambient; // ������?.... 
	m_mtrl.Diffuse = *(D3DXCOLOR*) &diffuse;
	m_mtrl.Specular = *(D3DXCOLOR*) &specular;
	m_mtrl.Emissive = *(D3DXCOLOR*) &emisive;
	m_mtrl.Power = power;
}

void cMaterial::Bind() // �ٸ������� ���� �������� �Լ�
{
	GetDevice()->SetMaterial(&m_mtrl);
}

void cMaterial::InitRed()
{
	Init(Vector4(1,0,0,1), Vector4(1,0,0,1), Vector4(0,0,0,1), Vector4(0,0,0,1), 0);
}

void cMaterial::InitBlue()
{
	Init(Vector4(0,0,1,1), Vector4(0,0,1,1), Vector4(0,0,1,1), Vector4(0,0,0,1), 0);
}