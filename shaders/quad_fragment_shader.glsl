#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

void main(){
	outColor = texture(texFramebuffer, Texcoord);
	//outColor = vec4(0.0, 1.0, 0.0, 1.0);
}
