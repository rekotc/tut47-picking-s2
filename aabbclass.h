////////////////////////////////////////////////////////////////////////////////
// Filename: aabbclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _AABBCLASS_H_
#define _AABBCLASS_H

class AabbClass
{
private:
	/*struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};*/



public:
	AabbClass();
	AabbClass(const AabbClass&);
	~AabbClass();

	void Shutdown();

	void BuildVertexArray(void*, char*, float, float);

	float min[3];
	float max[3];

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	
	void ReleaseTexture();

private:
	//FontType* m_Font;
	
	
};

#endif