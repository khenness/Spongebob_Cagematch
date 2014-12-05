#include "common_header.h"

#include "fog.h"

CFogParameters::CFogParameters()
{
	vFogColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	fStart = 10.0f;
	fEnd = 75.0f;
	fDensity = 0.04f;

	iEquation = FOG_EQUATION_EXP;
}

CFogParameters::CFogParameters(glm::vec4 a_vFogColor, float a_fStart, float a_fEnd, float a_fDensity, int a_iEquation)
{
	vFogColor = a_vFogColor;

	fStart = a_fStart;
	fEnd = a_fEnd;
	fDensity = a_fDensity;

	iEquation = a_iEquation;
}

/*-----------------------------------------------

Name:	setUniformData

Params:	spProgram - shader program
		sFogVarName - name of fog parameters variable

Result:	Sets all fog parameters.

/*---------------------------------------------*/

void CFogParameters::setUniformData(CShaderProgram *spProgram, std::string sFogVarName)
{
	spProgram->setUniform(sFogVarName+".vFogColor", vFogColor);

	spProgram->setUniform(sFogVarName+".fStart", fStart);
	spProgram->setUniform(sFogVarName+".fEnd", fEnd);
	spProgram->setUniform(sFogVarName+".fDensity", fDensity);

	spProgram->setUniform(sFogVarName+".iEquation", iEquation);
}