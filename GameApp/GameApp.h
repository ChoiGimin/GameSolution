#pragma once

#define WINSIZEX 800
#define WINSIZEY 600
#define OBSTACLEOBJ 3

enum penguinState
{
	JUMP,
	JUMPDOWN,
	RRUN
};


class cGameApp : public framework::cGameMain
{
private:
	LPDIRECT3DVERTEXBUFFER9 m_penguinBuff;
	LPDIRECT3DINDEXBUFFER9 m_penguinIndexBuff;
	int m_penguinVtxSize;
	int m_penguinFaceSize;

	LPDIRECT3DVERTEXBUFFER9 m_tileBuff;
	LPDIRECT3DINDEXBUFFER9 m_tileIndexBuff;
	int m_tileVtxSize;
	int m_tileFaceSize;

	LPDIRECT3DVERTEXBUFFER9 m_obstacleBuff;
	LPDIRECT3DINDEXBUFFER9 m_obstacleIndexBuff;
	int m_obstacleVtxSize;
	int m_obstacleFaceSize;

	Matrix44 mat_local;//팽귄 매트릭스 값
	Matrix44 mat_tile[4][5]; //타일
	Matrix44 mat_obstacle[OBSTACLEOBJ]; // 장애물

	Matrix44 mat_world;

	vector<Vector3> verteices;

	graphic::cMaterial m_mtrl;

	// 재질
	D3DMATERIAL9 g_redMtrl;
	D3DMATERIAL9 g_blueMtrl;
	
	// 조명
	D3DLIGHT9 g_Light;

	//펭귄 점프 설정
	penguinState _penguinState;
	bool _isJump;
	int _jumpCount;
	



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
	void DxRender (Matrix44 &tm, D3DMATERIAL9 &material, LPDIRECT3DVERTEXBUFFER9 vertexBuff, LPDIRECT3DINDEXBUFFER9 idxBuff, int vertexSize, int faceSize);
protected:

};