#include "common_header.h"

#include "pointLight.h"

CPointLight::CPointLight()
{
	vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);

	fAmbient = 0.1f;

	fConstantAtt = 0.3f;
	fLinearAtt = 0.007f;
	fExpAtt = 0.00008f;
}

CPointLight::CPointLight(glm::vec3 a_vColor, glm::vec3 a_vPosition, float a_fAmbient, float a_fConstantAtt, float a_fLinearAtt, float a_fExpAtt)
{
	vColor = a_vColor;
	vPosition = a_vPosition;

	fAmbient = a_fAmbient;
	
	fConstantAtt = a_fConstantAtt;
	fLinearAtt = a_fLinearAtt;
	fExpAtt = a_fExpAtt;
}

/*-----------------------------------------------

Name:	setUniformData

Params:	spProgram - shader program
		sLightVarName - name of point light variable

Result:	Sets all point light data.

/*---------------------------------------------*/

void CPointLight::setUniformData(CShaderProgram* spProgram, string sLightVarName)
{
	spProgram->setUniform(sLightVarName+".vColor", vColor);
	spProgram->setUniform(sLightVarName+".vPosition", vPosition);

	spProgram->setUniform(sLightVarName+".fAmbient", fAmbient);
	
	spProgram->setUniform(sLightVarName+".fConstantAtt", fConstantAtt);
	spProgram->setUniform(sLightVarName+".fLinearAtt", fLinearAtt);
	spProgram->setUniform(sLightVarName+".fExpAtt", fExpAtt);
}

