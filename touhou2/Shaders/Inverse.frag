uniform sampler2D texture;

void main()
{
	vec4 col = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = gl_Color * vec4(1. - col.rgb, col.a);
}
