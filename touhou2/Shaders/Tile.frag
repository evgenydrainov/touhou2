varying vec2 v_vPosition;

uniform sampler2D texture;
uniform vec2 rect_pos;
uniform vec2 rect_size;
uniform vec2 texture_size;

void main()
{
	vec2 co = rect_pos + mod(v_vPosition, rect_size);
	
	gl_FragColor = gl_Color * texture2D(texture, co / texture_size);
}
