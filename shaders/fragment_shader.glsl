#version 150

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float elapsed;

void main(){
	vec2 outTex = Texcoord;
	
	if (outTex.y < 0.5){
		outTex.y *= -1;
		outTex.x += sin((Texcoord.y * 60.0) + elapsed) * 1.5 / (sin(elapsed)+3) / 30 ;
	}	

	outColor = texture(texKitten, outTex);
}