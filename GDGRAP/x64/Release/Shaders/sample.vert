#version 330 core

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;
//uniform float x;
//uniform float y;

uniform mat4 transform;

uniform 
void main(){
//gl_position is predefined
//this denotes the final position of the vertext / point

//vec3 newPos = vec3(aPos.x + x, aPos.y + y, aPos.z);

 gl_Position = transform * vec4(aPos,1.0); //Turns the vec3 into a vec4
}

