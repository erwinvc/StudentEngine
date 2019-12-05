#version 330 core
out vec4 _Out;
  
in vec2 fsUv;
  
uniform sampler2D _Texture;
uniform float _Transparency;
void main() {
    _Out = vec4(texture(_Texture, fsUv).xyz, _Transparency);
}  