#version 150

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float imageBlend;

void main(){
	vec2 outTex = Texcoord;
	outTex.y = outTex.y < 0.5 ? outTex.y * -1 : outTex.y;

	outColor = texture(texKitten, outTex);
}