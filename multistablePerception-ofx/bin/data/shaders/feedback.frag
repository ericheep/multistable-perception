#version 410

// feedback
// fragment shader

#define PI 3.14159265359
#define TWO_PI 6.28318530718

in vec2 texCoordVarying;
uniform float u_feedback = 0.01;
uniform float u_offset = 1.0;
uniform float u_time = 0.0;

uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

out vec4 outputColor;

// All components are in the range [0…1], including hue.
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    // get pixel color
    /*
    vec4 color0 = texture(tex0, texCoordVarying);
        
    float angleX = cos(TWO_PI * u_time) * u_offset;
    float angleY = sin(TWO_PI * u_time) * u_offset;
    
    vec4 color1 = texture(fboTexture, texCoordVarying.xy);
    
    vec3 hsv0 = rgb2hsv(color0.rgb);
    vec3 hsv1 = rgb2hsv(color1.rgb);
    
    vec3 feedbackHsv = vec3(mod(hsv0.x + hsv1.x, 1.0), max(hsv0.y, hsv0.y), max(hsv0.z, hsv0.z));
    vec3 feedbackRgb = hsv2rgb(feedbackHsv);
    vec4 feedbackColor = vec4(feedbackRgb, mix(color0.w, color1.w, 0.5));
                        
    // render the output
    outputColor = mix(feedbackColor, texture(tex0, texCoordVarying), 1.0 - u_feedback);
     */
    
    vec2 tc0 = texcoordvarying + vec2(-u_offset, -u_offset);
    vec2 tc1 = vec2(0.5, 0.5) + vec2(0.0, -u_offset);
    vec2 tc2 = vec2(0.5, 0.5) + vec2(+u_offset, -u_offset);
    vec2 tc3 = vec2(0.5, 0.5) + vec2(-u_offset, 0.0);
    vec2 tc4 = vec2(0.5, 0.5) + vec2(0.0, 0.0);
    vec2 tc5 = vec2(0.5, 0.5) + vec2(+u_offset, 0.0);
    vec2 tc6 = vec2(0.5, 0.5) + vec2(-u_offset, +u_offset);
    vec2 tc7 = vec2(0.5, 0.5) + vec2(0.0, +u_offset);
    vec2 tc8 = vec2(0.5, 0.5) + vec2(+u_offset, +u_offset);

    vec4 col0 = texture(tex1, tc0);
    vec4 col1 = texture(tex1, tc1);
    vec4 col2 = texture(tex1, tc2);
    vec4 col3 = texture(tex1, tc3);
    vec4 col4 = texture(tex1, tc4);
    vec4 col5 = texture(tex1, tc5);
    vec4 col6 = texture(tex1, tc6);
    vec4 col7 = texture(tex1, tc7);
    vec4 col8 = texture(tex1, tc8);

    vec4 sum = (1.0 * col0 + (2.0 * u_feedback) * col1 + 1.0 * col2 +
                (2.0 * u_feedback) * col3 + (4.0 * u_feedback) * col4 + (2.0 * u_feedback) * col5 +
                1.0 * col6 + (2.0 * u_feedback) * col7 + 1.0 * col8) / 16.0;
       
    outputColor = vec4(sum.rgb, 1.0) * texture(tex0, texCoordVarying);
}

