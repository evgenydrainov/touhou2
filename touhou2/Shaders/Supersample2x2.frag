uniform sampler2D texture;
uniform vec2 offset;

void main()
{
	gl_FragColor = vec4(0.);
	
	gl_FragColor += texture2D(texture, gl_TexCoord[0].xy + offset * vec2(-.5, -.5));
	gl_FragColor += texture2D(texture, gl_TexCoord[0].xy + offset * vec2( .5, -.5));
	gl_FragColor += texture2D(texture, gl_TexCoord[0].xy + offset * vec2(-.5,  .5));
	gl_FragColor += texture2D(texture, gl_TexCoord[0].xy + offset * vec2( .5,  .5));
	gl_FragColor *= .25;
	
	gl_FragColor *= gl_Color;
}
