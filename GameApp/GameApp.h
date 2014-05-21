#pragma once


class cGameApp : public framework::cGameMain
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_verBuff;
	LPDIRECT3DINDEXBUFFER9 m_indexBuff;
	int m_VtxSize;
	int m_FaceSize;

	Matrix44 mat_local;
	Matrix44 mat_world;
	vector<Vector3> verteices;

public:
	cGameApp();
	virtual ~cGameApp();

	virtual bool OnInit() override;
	virtual void OnUpdate(const float elapseT) override;
	virtual void OnRender(const float elapseT) override;
	virtual void OnShutdown() override;
	virtual void MessageProc( UINT message, WPARAM wParam, LPARAM lParam) override;

	bool ReadModelFile( const string &fileName, LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize,  LPDIRECT3DINDEXBUFFER9 &idxBuff, int &faceSize );
	void ComputeNormals(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int vtxSize,  LPDIRECT3DINDEXBUFFER9 idxBuff, int faceSize);
	bool InitVertexBuffer();
protected:

};