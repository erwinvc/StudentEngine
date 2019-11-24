#version 330

layout(location = 0) in vec3 vsPosition;
layout(location = 1) in vec2 vsUv;
layout(location = 2) in float vsTextureID;
layout(location = 3) in float vsMaterialID;
layout(location = 4) in vec4 vsColor;

struct Data {
	vec2 uv;
	float textureID;
	float materialID;
	vec4 color;
};

out Data fsData;

uniform mat4 _Projection;
void main(){
	fsData.uv = vsUv;
	fsData.textureID = vsTextureID;
	fsData.materialID = vsMaterialID;
	fsData.color = vsColor;

	gl_Position = _Projection * vec4(vsPosition, 1.0);
	//gl_Position = vec4(vsPosition, 1.0);
}