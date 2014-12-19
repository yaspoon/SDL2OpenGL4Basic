varying float diffuse_value;

void main() {
	// Set the output color of our current pixel
	gl_FragColor = gl_Color * diffuse_value;
}
