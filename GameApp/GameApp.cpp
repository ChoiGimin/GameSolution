
#include "stdafx.h"
#include "GameApp.h"

INIT_FRAMEWORK(cGameApp)

// 버텍스 구조체
struct Vertex
{
   Vertex() {}
   Vertex(float x0, float y0, float z0, DWORD color0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0)), color(color0) {}
   Vector3 p;
   Vector3 n;
   DWORD color;
   static const DWORD FVF;
};

const DWORD Vertex::FVF  = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

cGameApp::cGameApp()
{
   m_windowName = L"South pole";
   const RECT r = {0, 0, WINSIZEX, WINSIZEY};
   m_windowRect = r;

}

cGameApp::~cGameApp()
{

}


bool cGameApp::OnInit()
{
	//타일 초기화
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			mat_tile[i][j]._41 = -250 + (150 * j);
			mat_tile[i][j]._43 = (180 * i);
		}
	}

	for(int j = 0; j < OBSTACLEOBJ; j++)
	{
		mat_obstacle[j]._41 = rand()%(-300 + rand()%300);
		mat_obstacle[j]._43 = rand()%1000;
	}

	_penguinState = RRUN;
	_isJump = false;
	_jumpCount = 0;

   srand((unsigned int)timeGetTime());

   m_penguinVtxSize = 0;
   m_penguinFaceSize = 0;
   m_penguinBuff = NULL;
   m_penguinIndexBuff = NULL;

   m_tileVtxSize = 0;
   m_tileFaceSize = 0;
   m_tileBuff = NULL;
   m_tileIndexBuff = NULL;
   
   m_obstacleVtxSize = 0;
   m_obstacleFaceSize = 0;
   m_obstacleBuff = NULL;
   m_obstacleIndexBuff = NULL;

   m_mtrl.InitRed();

   /*
   ZeroMemory(&g_redMtrl, sizeof(g_redMtrl));
   g_redMtrl.Ambient = D3DXCOLOR(1,0,0,1);
   g_redMtrl.Diffuse = D3DXCOLOR(1,0,0,1);
   g_redMtrl.Specular = D3DXCOLOR(1,0,0,1);
   g_redMtrl.Emissive = D3DXCOLOR(0,0,0,1);
   g_redMtrl.Power = 0;

   ZeroMemory(&g_blueMtrl, sizeof(g_blueMtrl));
   g_blueMtrl.Ambient = D3DXCOLOR(0,0,1,1);
   g_blueMtrl.Diffuse = D3DXCOLOR(0,0,1,1);
   g_blueMtrl.Specular = D3DXCOLOR(0,0,1,1);
   g_blueMtrl.Emissive = D3DXCOLOR(0,0,0,1);
   g_blueMtrl.Power = 0;
   */
   ReadModelFile("penguin.dat", m_penguinBuff, m_penguinVtxSize, m_penguinIndexBuff, m_penguinFaceSize);
   ReadModelFile("tile.dat", m_tileBuff, m_tileVtxSize, m_tileIndexBuff, m_tileFaceSize);
   ReadModelFile("obstacle.dat", m_obstacleBuff, m_obstacleVtxSize, m_obstacleIndexBuff, m_obstacleFaceSize);

   //InitVertexBuffer();

   //카메라 , 투영, 조명 초기화
   
   //조명
   D3DXCOLOR color(1,1,1,1);
   ZeroMemory(&g_Light, sizeof(g_Light));
   g_Light.Type = D3DLIGHT_DIRECTIONAL;
   g_Light.Ambient = color * 0.4f;
   g_Light.Diffuse = color;
   g_Light.Specular = color * 0.75f;
   Vector3 lightDir(1,-1,0);
   lightDir.Normalize();
   g_Light.Direction = *(D3DXVECTOR3*)&lightDir;	
   //do 마우스로 광원 바꾸는거
   
   //카메라부분 초기화
   Matrix44 V;
   Vector3 dir = Vector3(0,0,0)-Vector3(0,2,-5);
   dir.Normalize();
   V.SetView(Vector3(0,200,-200), dir, Vector3(0,1,0));
   graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

   Matrix44 proj;
   proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZEX / (float) WINSIZEY, 1.f, 1000.0f) ;
   graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

   graphic::GetDevice()->SetLight(0, &g_Light); // 광원 설정.
   graphic::GetDevice()->LightEnable (
	   0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
	   true); // true = 활성화 ， false = 비활성화




   //graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

   return true;
}


void cGameApp::OnUpdate(const float elapseT)
{
	Matrix44 V;
	Vector3 dir = Vector3(0,0,0)-Vector3(0,2,-5);
	dir.Normalize();
	V.SetView(Vector3(0,200, mat_local._43 - 200), dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);
	

	mat_local._43 += 6.f;

	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(mat_tile[i][j]._43 <= mat_local._43 - 200)
			{
				mat_tile[i][j]._43 += 1000;
			}
		}
	}

	for(int j = 0; j < OBSTACLEOBJ; j++)
	{
		if(mat_obstacle[j]._43 < mat_local._43 -200)
		{
			mat_obstacle[j]._41 = rand()%(-300 + rand()%600);
			mat_obstacle[j]._43 = mat_local._43 + rand()%1000;
		}
	}



	if(_penguinState == JUMP && _isJump)
	{
		mat_local._42 += 13;
		_jumpCount ++;
	}
	
	if(_penguinState == JUMPDOWN && _isJump)
	{
		mat_local._42 -= 13;
		_jumpCount ++;
	}
	

	if(_jumpCount == 9)
	{
		_penguinState = JUMPDOWN;
	}
	if(_jumpCount == 18)
	{
		_penguinState = RRUN;
		_isJump = false;
		_jumpCount = 0;
	}

}


void cGameApp::OnRender(const float elapseT)
{
   //화면 청소
   if (SUCCEEDED(graphic::GetDevice()->Clear( 
      0,         //청소할 영역의 D3DRECT 배열 갯수      ( 전체 클리어 0 )
      NULL,      //청소할 영역의 D3DRECT 배열 포인터      ( 전체 클리어 NULL )
      D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,   //청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
      D3DCOLOR_XRGB(255, 255, 255),         //컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
      1.0f,            //깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
      0               //스텐실 버퍼를 채울값
      )))
   {
      graphic::GetDevice()->BeginScene();

	  //펭귄을 그릴 때 필요한 것 ( 함수로 만듦 )
	  DxRender(mat_local, g_redMtrl, m_penguinBuff, m_penguinIndexBuff, 
		  m_penguinVtxSize, m_penguinFaceSize);
	  
	  // Tile
	  for (int i = 0; i < 4; i++)
	  {
		  for(int j = 0; j < 5; j++)
		  {
			  DxRender(mat_tile[i][j], g_blueMtrl, m_tileBuff, 
				  m_tileIndexBuff, m_tileVtxSize, m_tileFaceSize);
		  }
	  }

	  // Obstacle
	  //mat_obstacle.SetIdentity();
	  //mat_obstacle._43 = rand()%100;

	  for(int j = 0; j < OBSTACLEOBJ; j++)
	  {
		  m_mtrl.Bind();
		  DxRender(mat_obstacle[j], g_blueMtrl, m_obstacleBuff, m_obstacleIndexBuff,
			  m_obstacleVtxSize, m_obstacleFaceSize);
	  }

	  

	  

	  //랜더링 끝
      graphic::GetDevice()->EndScene();
      //랜더링이 끝났으면 랜더링된 내용 화면으로 전송
      graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
   }
}


void cGameApp::OnShutdown()
{
   
   if (m_penguinBuff)
      m_penguinBuff->Release();
   if (m_penguinIndexBuff)
      m_penguinIndexBuff->Release();
}


void cGameApp::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
   switch(message)
   {
   case WM_KEYDOWN:
      {
         switch(wParam)
         {
         case VK_LEFT:
            {
				mat_local._41 -= 6.f;
            }
            break;
         case VK_RIGHT:
            {
				mat_local._41 += 6.f;
            }
            break;
         case VK_UP:
            {
               if(!_isJump && _penguinState == RRUN)
			   {
				   _isJump = true;
				   _penguinState = JUMP;
			   }
            }
            break;
         }
      }
      break;
   }
}

bool cGameApp::InitVertexBuffer()
{

   Matrix44 V;
   Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-5);
   dir.Normalize();
   V.SetView(Vector3(0,0,-500), dir, Vector3(0,1,0));
   graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

   Matrix44 proj;
   proj.SetProjection(D3DX_PI * 0.5f, (float)800 / (float) 600, 1.f, 1000.0f) ;
   graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

   graphic::GetDevice()->LightEnable (
      1, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
      true); // true = 활성화 ， false = 비활성화

   return true;
}

bool cGameApp::ReadModelFile( const string &fileName, LPDIRECT3DVERTEXBUFFER9 &vtxBuff, int &vtxSize,  LPDIRECT3DINDEXBUFFER9 &idxBuff, int &faceSize )
{
   using namespace std;
   ifstream fin(fileName.c_str());
   if (!fin.is_open())
      return false;

   string vtx, vtx_eq;
   int numVertices;
   fin >> vtx >> vtx_eq >> numVertices;

   if (numVertices <= 0)
      return  false;

   vtxSize = numVertices;

   // 버텍스 버퍼 생성.
   if (FAILED(graphic::GetDevice()->CreateVertexBuffer( numVertices * sizeof(Vertex),
      D3DUSAGE_WRITEONLY, Vertex::FVF,
      D3DPOOL_MANAGED, &vtxBuff, NULL)))
   {
      return false;
   }

   // 버텍스 버퍼 초기화.
   Vertex* vertices;
   if (FAILED(vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0)))
      return false;
   float num1, num2, num3;
   for (int i = 0; i < numVertices; i++)
   {
      fin >> num1 >> num2 >> num3;
      vertices[i] = Vertex(num1, num2, num3, D3DCOLOR_XRGB(255,255,0));
   }
   vtxBuff->Unlock();


   string idx, idx_eq;
   int numIndices;
   fin >> idx >> idx_eq >> numIndices;

   if (numIndices <= 0)
      return false;

   faceSize = numIndices;

   if (FAILED(graphic::GetDevice()->CreateIndexBuffer(numIndices*3*sizeof(WORD), 
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX16,
      D3DPOOL_MANAGED,
      &idxBuff, NULL)))
   {
      return false;
   }

   WORD *indices = NULL;
   idxBuff->Lock(0, 0, (void**)&indices, 0);
   int num4, num5, num6;
   for (int i = 0; i < numIndices*3; i+=3)
   {
      fin >> num4 >> num5 >> num6;
      indices[ i] = num4;
      indices[ i+1] = num5;
      indices[ i+2] = num6;   
   }
   idxBuff->Unlock();

   ComputeNormals(vtxBuff, vtxSize, idxBuff, faceSize);
   return true;
}

void cGameApp::ComputeNormals(LPDIRECT3DVERTEXBUFFER9 vtxBuff, int vtxSize,  LPDIRECT3DINDEXBUFFER9 idxBuff, int faceSize)
{
   Vertex* vertices;
   vtxBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0);
   WORD *indices = NULL;
   idxBuff->Lock(0, 0, (void**)&indices, 0);

   for (int i=0; i < faceSize*3; i+=3)
   {
      Vector3 p1 = vertices[ indices[ i]].p;
      Vector3 p2 = vertices[ indices[ i+1]].p;
      Vector3 p3 = vertices[ indices[ i+2]].p;

      Vector3 v1 = p2 - p1;
      Vector3 v2 = p3 - p1;
      v1.Normalize();
      v2.Normalize();
      Vector3 n = v1.CrossProduct(v2);
      n.Normalize();

      if (vertices[ indices[ i]].n.IsEmpty())
      {
         vertices[ indices[ i]].n = n;
      }
      else
      {
         vertices[ indices[ i]].n += n;
         vertices[ indices[ i]].n /= 2.f;
      }

      if (vertices[ indices[ i+1]].n.IsEmpty())
      {
         vertices[ indices[ i+1]].n = n;
      }
      else
      {
         vertices[ indices[ i+1]].n += n;
         vertices[ indices[ i+1]].n /= 2.f;
      }

      if (vertices[ indices[ i+2]].n.IsEmpty())
      {
         vertices[ indices[ i+2]].n = n;
      }
      else
      {
         vertices[ indices[ i+2]].n += n;
         vertices[ indices[ i+2]].n /= 2.f;
      }
   }

   vtxBuff->Unlock();
   idxBuff->Unlock();
}

void cGameApp::DxRender (Matrix44 &tm, D3DMATERIAL9 &material, LPDIRECT3DVERTEXBUFFER9 vertexBuff, LPDIRECT3DINDEXBUFFER9 idxBuff, int vertexSize, int faceSize)
{
	graphic::GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&tm);
	
	graphic::GetDevice()->SetStreamSource( 0, vertexBuff, 0, sizeof(Vertex) ); //그릴 물체
	graphic::GetDevice()->SetIndices(idxBuff);
	graphic::GetDevice()->SetFVF( Vertex::FVF );
	graphic::GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, vertexSize, 0, faceSize);
}