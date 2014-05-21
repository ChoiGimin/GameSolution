 y
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
   m_windowName = L"GameApp";
   const RECT r = {0, 0, 800, 600};
   m_windowRect = r;

}

cGameApp::~cGameApp()
{

}


bool cGameApp::OnInit()
{
   m_VtxSize = 0;
   m_FaceSize = 0;
   m_verBuff = NULL;
   m_indexBuff = NULL;
   InitVertexBuffer();

   return true;
}


void cGameApp::OnUpdate(const float elapseT)
{


}


void cGameApp::OnRender(const float elapseT)
{
   //화면 청소
   if (SUCCEEDED(m_DxDevice->Clear( 
      0,         //청소할 영역의 D3DRECT 배열 갯수      ( 전체 클리어 0 )
      NULL,      //청소할 영역의 D3DRECT 배열 포인터      ( 전체 클리어 NULL )
      D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,   //청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
      D3DCOLOR_XRGB(255, 255, 255),         //컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
      1.0f,            //깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
      0               //스텐실 버퍼를 채울값
      )))
   {
      m_DxDevice->BeginScene();

      static float y = 0;
      y += elapseT / 1000.f;
      // 각도가 2*PI 에 이르면 0으로 초기화한다.
      if (y >= 6.28f)
         y = 0;

      Matrix44 rx, ry, r;
      rx.SetRotationX(MATH_PI/4.f);    // x축으로 45도 회전시킨다.
      ry.SetRotationY(0); // y축으로 회전

      r = rx * ry;

      m_DxDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&r);
      
      m_DxDevice->SetStreamSource( 0, m_verBuff, 0, sizeof(Vertex) );
      m_DxDevice->SetIndices(m_indexBuff);
      m_DxDevice->SetFVF( Vertex::FVF );
      m_DxDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_VtxSize, 0, m_FaceSize);

      //랜더링 끝
      m_DxDevice->EndScene();
      //랜더링이 끝났으면 랜더링된 내용 화면으로 전송
      m_DxDevice->Present( NULL, NULL, NULL, NULL );
   }
}


void cGameApp::OnShutdown()
{
   
   if (m_verBuff)
      m_verBuff->Release();
   if (m_indexBuff)
      m_indexBuff->Release();
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
               Vector3 movX(-10,0,0);
               Vertex* vertices;
               if (SUCCEEDED(m_verBuff->Lock( 0, 0, (void**)&vertices, 0)))
               {
                  for(int i = 0; i < m_VtxSize; ++i)
                  {
                     vertices[i].p += movX;
                  }
               }
               m_verBuff->Unlock();
            }
            break;
         case VK_RIGHT:
            {
               Vector3 movX(10,0,0);
               Vertex* vertices;
               if (SUCCEEDED(m_verBuff->Lock( 0, 0, (void**)&vertices, 0)))
               {
                  for(int i = 0; i < m_VtxSize; ++i)
                  {
                     vertices[i].p += movX;
                  }
               }
               m_verBuff->Unlock();
            }
            break;
         case VK_UP:
            {
               Matrix44 z;
               //z.SetIdentity();
               z.SetRotationY(0.1f);
               Vertex* vertices;
               if (SUCCEEDED(m_verBuff->Lock( 0, 0, (void**)&vertices, 0)))
               {
                  for(int i = 0; i < m_VtxSize; ++i)
                  {
                     vertices[i].p *= z;
                  }
               }
               m_verBuff->Unlock();
            }
            break;
         }
      }
      break;
   }
}

bool cGameApp::InitVertexBuffer()
{
   ReadModelFile("cube.dat", m_verBuff, m_VtxSize, m_indexBuff, m_FaceSize);

   Matrix44 V;
   Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-5);
   dir.Normalize();
   V.SetView(Vector3(0,0,-500), dir, Vector3(0,1,0));
   m_DxDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

   Matrix44 proj;
   proj.SetProjection(D3DX_PI * 0.5f, (float)800 / (float) 600, 1.f, 1000.0f) ;
   m_DxDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

   //m_DxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
      //m_DxDevice->SetRenderState(D3DRS_LIGHTING, false);
   m_DxDevice->LightEnable (
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
   if (FAILED(m_DxDevice->CreateVertexBuffer( numVertices * sizeof(Vertex),
      D3DUSAGE_WRITEONLY, Vertex::FVF,
      D3DPOOL_MANAGED, &vtxBuff, NULL)))
   {
      return false;
   }

   // 버텍스 버퍼 초기화.
   Vertex* vertices;
   if (FAILED(m_verBuff->Lock( 0, sizeof(Vertex), (void**)&vertices, 0)))
      return false;
   float num1, num2, num3;
   for (int i = 0; i < numVertices; i++)
   {
      fin >> num1 >> num2 >> num3;
      vertices[i] = Vertex(num1, num2, num3, D3DCOLOR_XRGB(255,255,0));
   }
   m_verBuff->Unlock();


   string idx, idx_eq;
   int numIndices;
   fin >> idx >> idx_eq >> numIndices;

   if (numIndices <= 0)
      return false;

   faceSize = numIndices;

   if (FAILED(m_DxDevice->CreateIndexBuffer(numIndices*3*sizeof(WORD), 
      D3DUSAGE_WRITEONLY,
      D3DFMT_INDEX16,
      D3DPOOL_MANAGED,
      &idxBuff, NULL)))
   {
      return false;
   }

   WORD *indices = NULL;
   m_indexBuff->Lock(0, 0, (void**)&indices, 0);
   int num4, num5, num6;
   for (int i = 0; i < numIndices*3; i+=3)
   {
      fin >> num4 >> num5 >> num6;
      indices[ i] = num4;
      indices[ i+1] = num5;
      indices[ i+2] = num6;   
   }
   m_indexBuff->Unlock();

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