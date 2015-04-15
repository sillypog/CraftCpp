#version 150

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float elapsed;

void main(){
	vec2 outTex = Texcoord;

	outColor = texture(texKitten, outTex) * vec4(Color,1.0);
}