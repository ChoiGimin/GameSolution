
#pragma once

namespace graphic
{

	class cMaterial
	{
	public:
		cMaterial();
		virtual ~cMaterial();

		void Init(const Vector4 &ambient, const Vector4 &diffuse,
			const Vector4 &specular, const Vector4 &emisive, const float power);
		void Bind(); // �ٸ������� ���� �������� �Լ�

		void InitRed();
		void InitBlue();

		D3DMATERIAL9 m_mtrl;
	};
};