	#version 330 core
	layout (location = 0) in vec3 aPos;
	uniform float translateX;
	void main(){
		// Flip triangle by add - to y-coordinate
		gl_Position = vec4(aPos.x + translateX , -aPos.y, aPos.z, 1.0); 
	}