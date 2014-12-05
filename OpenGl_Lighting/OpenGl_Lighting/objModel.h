#pragma once

#include "texture.h"
#include "vertexBufferObject.h"

/********************************

Class:	CObjModel

Purpose: Class for handling obj
		 model files.

********************************/

class CObjModel
{
public:
	bool loadModel(string sFileName, string sMtlFileName);
	void renderModel();
	void releaseModel();

	int getPolygonCount();

	CObjModel();
private:
	bool bLoaded;
	int iAttrBitField;
	int iNumFaces;

	bool loadMaterial(string sFullMtlFileName);

	CVertexBufferObject vboModelData;
	UINT uiVAO;
	//CTexture tAmbientTexture;
	std::vector<CTexture> tAmbientTextures;
};