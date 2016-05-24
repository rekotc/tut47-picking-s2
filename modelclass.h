////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "C:\Program Files (x86)\Windows Kits\8.1\Include\um\d3d11.h"
#include <directxmath.h>
using namespace DirectX;
#include <fstream>
using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "aabbclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	bool InitializeFlatRectangle(ID3D11Device*, ID3D11DeviceContext*, float width, float height, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	void setPosition(XMFLOAT3);
	void updatePosition(XMFLOAT3);
	XMFLOAT3 getPosition();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	AabbClass* getBoundingBox();
	void updateBoundingBoxPos(XMFLOAT3);

	void setId(int);
	int getId();

	void setColor(XMFLOAT4);
	XMFLOAT4 getColor();

	XMFLOAT3 getInstantRotation();
	XMFLOAT3 getFixedRotation();

	void setInstantRotation(XMFLOAT3);
	void setInstantRotationX(float);
	void setInstantRotationY(float);
	void setInstantRotationZ(float);

	void setFixedRotation(XMFLOAT3);
	void setFixedRotationX(float);
	void setFixedRotationY(float);
	void setFixedRotationZ(float);

	XMMATRIX* getRotationMatrix();
//	void setRotationMatrix(XMMATRIX);

private:
	bool InitializeBuffers(ID3D11Device*);
	bool InitializeBuffersFlatRectangle(ID3D11Device*);

	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	AabbClass* m_boundingBox;
	int m_id;

	XMMATRIX m_rotationMatrix;
	XMMATRIX rotX, rotY, rotZ;
	XMFLOAT3 instantRotation;
	XMFLOAT3 fixedRotation;
	XMFLOAT3 position;
	XMFLOAT4 pickedUpColor;
};

#endif