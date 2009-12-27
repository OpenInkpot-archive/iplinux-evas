"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"uniform sampler2D tex;\n"
"varying vec4 col;\n"
"varying vec2 tex_c;\n"
"void main()\n"
"{\n"
"   gl_FragColor = texture2D(tex, tex_c.xy).aaaa * col;\n"
"}\n"
