uniform sampler2D texture;
uniform vec2 texel_size;

void main()
{
	vec2 dg1 = texel_size * .5;
	vec2 dg2 = vec2(-dg1.x, dg1.y);
	vec2 sd1 = dg1 * .5;
	vec2 sd2 = dg2 * .5;
	
	vec4 c  = texture2D(texture, gl_TexCoord[0].xy);
	vec4 i1 = texture2D(texture, gl_TexCoord[0].xy - sd1);
	vec4 i2 = texture2D(texture, gl_TexCoord[0].xy - sd2);
	vec4 i3 = texture2D(texture, gl_TexCoord[0].xy + sd1);
	vec4 i4 = texture2D(texture, gl_TexCoord[0].xy + sd2);
	vec4 o1 = texture2D(texture, gl_TexCoord[0].xy - dg1);
	vec4 o3 = texture2D(texture, gl_TexCoord[0].xy + dg1);
	vec4 o2 = texture2D(texture, gl_TexCoord[0].xy - dg2);
	vec4 o4 = texture2D(texture, gl_TexCoord[0].xy + dg2);
	
	float ko1 = dot(abs(o1 - c), vec4(1.));
	float ko2 = dot(abs(o2 - c), vec4(1.));
	float ko3 = dot(abs(o3 - c), vec4(1.));
	float ko4 = dot(abs(o4 - c), vec4(1.));
	
	float k1 = min(dot(abs(i1 - i3), vec4(1.)), max(ko1, ko3));
	float k2 = min(dot(abs(i2 - i4), vec4(1.)), max(ko2, ko4));
	
	float w1 = k2; if (ko3 < ko1) w1 *= ko3 / ko1;
	float w2 = k1; if (ko4 < ko2) w2 *= ko4 / ko2;
	float w3 = k2; if (ko1 < ko3) w3 *= ko1 / ko3;
	float w4 = k1; if (ko2 < ko4) w4 *= ko2 / ko4;
	
	gl_FragColor = (w1 * o1 + w2 * o2 + w3 * o3 + w4 * o4 + .001 * c) / (w1 + w2 + w3 + w4 + .001) * gl_Color;
}
