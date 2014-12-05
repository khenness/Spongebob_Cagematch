#pragma once

#include "shaders.h"

#define FOG_EQUATION_LINEAR	0
#define FOG_EQUATION_EXP	1
#define FOG_EQUATION_EXP2	2

/********************************

Class:	CFogParameters

Purpose: Support class for managing
		fog in the scene.

********************************/

class CFogParameters
{
public:
	glm::vec4 vFogColor;
	
	float fStart;
	float fEnd;
	float fDensity;

	int iEquation;

	void setUniformData(CShaderProgram* spProgram, string sFogVarName);

	CFogParameters();
	CFogParameters(glm::vec4 a_vFogColor, float a_fStart, float a_fEnd, float a_fDensity, int a_iEquation);
};