#version 330

uniform struct Matrices
{
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

smooth out vec2 texCoord;
smooth out vec3 vNormal;
smooth out vec3 vEyeSpacePos;
smooth out vec3 vWorldPos;

void main()
{
	vec4 vEyeSpacePosVertex = matrices.viewMatrix*matrices.modelMatrix*vec4(inPosition, 1.0);
	gl_Position = matrices.projectionMatrix*vEyeSpacePosVertex;

	texCoord = inCoord;
	vec4 vRes = matrices.normalMatrix*vec4(inNormal, 0.0);
	vNormal = vRes.xyz;
	
	vEyeSpacePos = vEyeSpacePosVertex.xyz;
   
   vec4 vWorldPosVertex = matrices.modelMatrix*vec4(inPosition, 1.0);
   vWorldPos = vWorldPosVertex.xyz;
}