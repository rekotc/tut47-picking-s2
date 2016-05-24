////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textclass.h"
#include "bitmapclass.h"
//#include "aabbclass.h"
#include "collisionclass.h"
//#include <math.h>
//added s2
#include <directxmath.h>
using namespace DirectX;
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame(int, int);

private:
	bool HandleInput();
	bool Render();
	void TestIntersection(int, int);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);
	bool RayAABBIntersect(CollisionClass*, int, XMFLOAT3, XMFLOAT3, AabbClass*);
	bool setPickedUpMesh(CollisionClass*);
	bool setPickedUpColor(int);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	//TextClass* m_Text;
	BitmapClass* m_Bitmap;
	bool m_beginCheck;
	int m_screenWidth, m_screenHeight;

	int m_mouseX, m_mouseY;

	//AabbClass* bCube;
	//AabbClass* bCube2;
	XMFLOAT4 pickedUpColor;
	XMFLOAT4 pickedUpColor2;
	XMFLOAT4 pickedUpColors[2];

	CollisionClass* pickedUpMesh;
};

#endif