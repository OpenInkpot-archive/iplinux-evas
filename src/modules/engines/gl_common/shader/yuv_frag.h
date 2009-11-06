"#ifdef GL_ES\n"
"precision mediump float;\n"
"#endif\n"
"uniform sampler2D tex, texu, texv;\n"
"varying vec4 col;\n"
"varying vec2 tex_c;\n"
"varying vec2 tex_c2;\n"
"varying vec2 tex_c3;\n"
"void main()\n"
"{\n"
"   mat4 yuv2rgb = mat4( 1.16400,  1.16400,  1.16400, 0.00000,\n"
"                        0.00000, -0.34410,  1.77200, 0.00000,\n"
"                        1.40200, -0.71410,  0.00000, 0.00000,\n"
"                       -0.77380,  0.45630, -0.95880, 1.00000);\n"
"   vec4 yuv = vec4(texture2D(tex, tex_c.xy).r,\n"
"                   texture2D(texu, tex_c2.xy).r,\n"
"                   texture2D(texv, tex_c3.xy).r, 1.0);\n"
"   gl_FragColor = (yuv2rgb * yuv) * col;\n"
"}\n"
