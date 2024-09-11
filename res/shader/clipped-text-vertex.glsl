#version 330 core
layout (location = 0) in vec2 glyphPos;
layout (location = 1) in vec2 texPos;
out vec2 Pos;
out vec2 TexCoords;
                
uniform mat2 glyphScaling;
uniform mat4 textPosToNdc;
uniform vec2 textOrigin;

void main()
{
    gl_Position = vec4(vec4(textPosToNdc * vec4(textOrigin, 0.0, 1.0)).xy + (glyphScaling * glyphPos), 0.0, 1.0);
    Pos = gl_Position.xy;
    TexCoords = texPos.xy;
}
