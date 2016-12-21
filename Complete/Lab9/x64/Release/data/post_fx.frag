#version 330 core
uniform sampler2D colorTexture;
in vec2 UV;
out vec3 color;


uniform int mode;
uniform float param0;
uniform float param1;
uniform float param2;

vec3 rgb2hsv(vec3 c){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
 
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 fXSwirl(sampler2D tex, vec2 uv, float time) {
	const float radius = 200.0;
	const float angle = 0.8;
	const vec2 center = vec2(512.0, 384.0);
	
	vec2 texSize = vec2(1024, 768);
	vec2 tc = uv * texSize;
	tc -= center;
	float dist = length(tc);
	if (dist < radius) {
		float percent = (radius - dist) / radius;
		float theta = percent * percent * angle * 8.0 * time*0.1;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s,c)));
	}
	tc += center;
	return texture2D(tex, tc / texSize).rgb;
}

void main(){
	color = texture(colorTexture, UV).rgb; 
	if (mode == 0) {
		if (UV.x < 0.5) {
			color -= vec3(param0);
			color *= vec3(1.0 + param1);
		}
	}
	if (mode == 1) {
		const vec3 grayFactor = vec3(0.27, 0.67, 0.06);
		if (UV.x < 0.5) {
			float gray = color.r*grayFactor.r + color.g*grayFactor.g + color.b*grayFactor.b;
			color = vec3(gray);
		}
	}

	if (mode == 2) {
		vec2 centerPos = -1.0 + 2.0 * UV;
		float centerLength = length(centerPos);
		vec2 offset = centerPos/centerLength * cos(centerLength*12.0-param0*4.0)*0.03;
		vec2 newUV = UV + offset;
		vec3 newColor = texture2D(colorTexture, newUV).rgb;
		color = newColor;
	}

	if (mode == 3) {
		const float sizeZoom = 0.3;
		vec2 zoomPos = vec2(0.5,0.5);
		
		float maxX = zoomPos.x + sizeZoom;
		float minX = zoomPos.x - sizeZoom;
		float maxY = zoomPos.y + sizeZoom;
		float minY = zoomPos.y - sizeZoom;
		
		if (UV.x > minX && UV.x < maxX && UV.y > minY && UV.y < maxY) {
			vec2 relPos = UV - zoomPos;
			float angle = atan(relPos.y, relPos.x);
			float dist = sqrt(relPos.x * relPos.x + relPos.y*relPos.y);
			if (dist <= sizeZoom) {
				 float newRad = dist * ((1.0 * dist/sizeZoom) + 0.1 );
				 vec2 newUV;
				 newUV.x = zoomPos.x + cos(angle)*newRad;
				 newUV.y = zoomPos.y + sin(angle)*newRad;
				 vec3 newColor = texture2D(colorTexture, newUV).rgb;
				 color = newColor;
			}
		}
	}

	if (mode == 4) {
		color = fXSwirl(colorTexture, UV, param0);
	}
	if (mode == 5) {
		color = fXSwirl(colorTexture, UV, -param0);	
	}
	if (mode == 6 ) {
		vec3 hsv = rgb2hsv(color);
		hsv.y += param0*0.1;
		color = hsv2rgb(hsv);
	}
}
