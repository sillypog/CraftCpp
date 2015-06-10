#version 150

in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texFramebuffer;

void main(){
	// Invert the colors
	//outColor = vec4(1.0, 1.0, 1.0, 1.0) - texture(texFramebuffer, Texcoord);

	// Greyscale
	//outColor = texture(texFramebuffer, Texcoord);
	//float avg = 0.2126 * outColor.r + 0.7152 * outColor.g + 0.0722 * outColor.b;
	//outColor = vec4(avg, avg, avg, 1.0);

	// Sobel edge detection
	vec4 s1 = textureOffset(texFramebuffer, Texcoord, ivec2(-1, -1));
	vec4 s2 = textureOffset(texFramebuffer, Texcoord, ivec2(+1, -1));
	vec4 s3 = textureOffset(texFramebuffer, Texcoord, ivec2(-1, +1));
	vec4 s4 = textureOffset(texFramebuffer, Texcoord, ivec2(+1, +1));
	vec4 sx = 4.0 * ((s4 + s3) - (s2 + s1));
	vec4 sy = 4.0 * ((s2 + s4) - (s1 + s3));
	vec4 sobel = sqrt(sx * sx + sy * sy);
	outColor = sobel;

}
