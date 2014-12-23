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
		// Want to get 5 cycles in a distance of 0 - 0.5 (
		//outTex.x += (sin(Texcoord.y * 60) / 20);
		outTex.x += (sin(Texcoord.y * 60) / (sin(elapsed)+2) / 30 );
		
	}	

	outColor = texture(texKitten, outTex);
}