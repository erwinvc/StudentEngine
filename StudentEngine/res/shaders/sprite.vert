#version 330

layout(location = 0) in vec3 vsPosition;
layout(location = 1) in vec3 vsUv;
layout(location = 2) in float vsTextureID;
layout(location = 3) in float vsMaterialID;
layout(location = 4) in float vsColor;

uniform mat3 _Projection;
void main(){

	//vec4 viewPos = MVMatrix * vec4(vsPos, 1.0);
	//fsData.normal = (MVMatrix * vec4(vsNormal, 0.0)).xyz;
	//fsData.TBNMatrix = mat3(MVMatrix) * mat3(vsTangent, vsBitangent, vsNormal);
	//
	//fsData.uv = vsUv;
	//fsData.viewDirection = _CameraPosition - viewPos.xyz;

	//gl_Position = _ProjectionMatrix * viewPos;
}