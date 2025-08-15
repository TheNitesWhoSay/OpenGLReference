#version 330 core
in vec2 Pos;
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec4 textColor;
uniform mat4 textPosToNdc;
uniform vec2 lowerRightBound;
uniform float verticalFlip;

void main()
{
    vec4 ndcUpperRightBound = textPosToNdc * vec4(lowerRightBound, 0.0, 1.0);
    if ( texture(tex, TexCoords).r == 0.0 || Pos.x > ndcUpperRightBound.x || verticalFlip * Pos.y < verticalFlip * ndcUpperRightBound.y )
        discard;
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, TexCoords).r);
    color = textColor * sampled;
}
