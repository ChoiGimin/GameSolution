#pragma once

namespace graphic

{
	class lighting

	{
		public:
		lighting(void);
		virtual ~lighting(void);

		D3DLIGHT9 m_Light;

		void InitLighting(const D3DXCOLOR &light, _D3DLIGHTTYPE type, float ambient,
			float diffuse, float specular, const Vector3 lightDirection);
		void Bind();

	};

}