#include "common_header.h"

#include "dirLight.h"

CDirectionalLight::CDirectionalLight()
{
	vColor = glm::vec3(1.0f, 1.0f, 1.0f);
	vDirection = glm::vec3(0.0f, -1.0f, 0.0f);

	fAmbient = 0.25f;
}
CDirectionalLight::CDirectionalLight(glm::vec3 a_vColor, glm::vec3 a_vDirection, float a_fAmbient)
{
	vColor = a_vColor;
	vDirection = a_vDirection;

	fAmbient = a_fAmbient;
}

/*-----------------------------------------------

Name:	setUniformData

Params:	spProgram - shader program
		sLightVarName - name of directional light variable

Result:	Sets all directional light data.

/*---------------------------------------------*/

void CDirectionalLight::setUniformData(CShaderProgram *spProgram, std::string sLightVarName)
{
	spProgram->setUniform(sLightVarName+".vColor", vColor);
	spProgram->setUniform(sLightVarName+".vDirection", vDirection);

	spProgram->setUniform(sLightVarName+".fAmbient", fAmbient);
}