#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3)out;

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;

in vec2 vTexCoordPass[];
in vec3 vNormalPass[];

smooth out vec3 vNormal;
smooth out vec2 vTexCoord;

void main()
{
    mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;

    vec3 vNormalTransformed[3];
    for(int i = 0; i < 3; i++)vNormalTransformed[i] = (vec4(vNormalPass[i], 1.0)*matrices.normalMatrix).xyz;

    for(int i = 0; i < 3; i++)
    {
      vec3 vPos = gl_in[i].gl_Position.xyz;
      gl_Position = mMVP*vec4(vPos, 1.0);
      vNormal = (vec4(vNormalTransformed[i], 1.0)).xyz;
      vTexCoord = vTexCoordPass[i];
      EmitVertex();
    }
    EndPrimitive();
}