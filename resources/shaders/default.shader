//+type: vertex
#version 330 core

in vec3 pos;

void main() {
  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}

//+type: fragment
#version 330 core

void main() {
  gl_FragColor = vec4(1.f, 1.f, 1.f, 1.f);
}
