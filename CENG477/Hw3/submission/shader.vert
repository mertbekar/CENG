#version 410

layout(location = 0) in vec3 vertex;

// Data from CPU
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform float widthTexture;
uniform float heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

// This functions normalizes the given vector by dividing its x, y and z
// components by its length.
vec3 normalize(vec3 v){
	vec3 outV = vec3(0, 0, 0);
	float vLen = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    outV.x = v.x / vLen;
    outV.y = v.y / vLen;
    outV.z = v.z / vLen;
	return outV;
}

// This function is used to compute the dot product of 2 vectors.
float dot(vec3 v1, vec3 v2){
	float dotProduct = 0.0;
	dotProduct += v1.x * v2.x;
	dotProduct += v1.y * v2.y;
	dotProduct += v1.z * v2.z;
    return dotProduct;
}

vec3 computeNormal(	vec4 a,	vec4 b) {
	a *= MV;
	b *= MV;
	return normalize( cross(vec3(a.xyz), vec3(b.xyz) ) );
}
float computeHeight( float r, float g, float b ) {
	return heightFactor * (r * 0.2126 + g * 0.7152 + b * 0.0722);
}
vec4 createVec(int offsetX, float r, float g, float b, int offsetZ) {
	return vec4(vertex.x + offsetX, computeHeight(r, g, b), vertex.z + offsetZ, 1);
}

vec4[9] createColor(vec2 uv)
{
	vec4 colors[9];
	float xPlus1 = uv.x + 1.0f / widthTexture;
	float xMinus1 = uv.x - 1.0f / widthTexture;
	float yPlus1 = uv.y + 1.0f / heightTexture;
	float yMinus1 = uv.y - 1.0f / heightTexture;
	colors[0] = texture(rgbTexture, vec2(uv.x, uv.y));
	colors[1] = texture(rgbTexture, vec2(xPlus1, uv.y));
	colors[2] = texture(rgbTexture, vec2(uv.x, yMinus1));
	colors[3] = texture(rgbTexture, vec2(xMinus1, yMinus1));
	colors[4] = texture(rgbTexture, vec2(xMinus1, uv.y));
	colors[5] = texture(rgbTexture, vec2(uv.x, yPlus1));
	colors[6] = texture(rgbTexture, vec2(xPlus1, yPlus1));
	colors[7] = texture(rgbTexture, vec2(xMinus1, yPlus1));
	colors[8] = texture(rgbTexture, vec2(xPlus1, yMinus1));
	return colors;
}

vec4[9] createVertices(vec4 cArray[9])
{
	vec4 vertices[9];
	vertices[0] = createVec( 0, cArray[0].x, cArray[0].y, cArray[0].z,  0);
	vertices[1] = createVec(-1, cArray[1].x, cArray[1].y, cArray[1].z,  0);
	vertices[2] = createVec( 0, cArray[2].x, cArray[2].y, cArray[2].z,  1);
	vertices[3] = createVec( 1, cArray[3].x, cArray[3].y, cArray[3].z,  1);
	vertices[4] = createVec( 1, cArray[4].x, cArray[4].y, cArray[4].z,  0);
	vertices[5] = createVec( 0, cArray[5].x, cArray[5].y, cArray[5].z, -1);
	vertices[6] = createVec(-1, cArray[6].x, cArray[6].y, cArray[6].z, -1);
	vertices[7] = createVec( 1, cArray[7].x, cArray[7].y, cArray[7].z, -1);
	vertices[8] = createVec(-1, cArray[8].x, cArray[8].y, cArray[8].z,  1);
	return vertices;
}

void main()
{
	vec4 vArray[9];
	int i;
	vec2 uv = vec2( 1.0f - vertex.x / widthTexture, 1.0f -
		vertex.z / heightTexture );
	textureCoordinate = uv;
	// next
	vArray = createVertices(createColor(uv));

	ToCameraVector = (-1 * MV * vArray[0]).xyz;
	ToLightVector = (MV * vec4( widthTexture / 2.0f,
		widthTexture + heightTexture, heightTexture / 2.0f, 1)  - MV * vArray[0]).xyz;

	vec4 v1MinusV0 = vArray[1] - vArray[0];
	vec4 v2MinusV0 = vArray[2] - vArray[0];
	vec4 v3MinusV0 = vArray[3] - vArray[0];
	vec4 v4MinusV0 = vArray[4] - vArray[0];
	vec4 v5MinusV0 = vArray[5] - vArray[0];
	vec4 v6MinusV0 = vArray[6] - vArray[0];
	vec4 v7MinusV0 = vArray[7] - vArray[0];
	vec4 v8MinusV0 = vArray[8] - vArray[0];

	vertexNormal = normalize( cross(vec3(( MV * (v1MinusV0)).xyz),
		vec3(( MV * v2MinusV0).xyz) )) +
	normalize( cross(vec3(( MV * v2MinusV0).xyz),
		vec3(( MV * v3MinusV0).xyz) )) +
	normalize( cross(vec3(( MV * v3MinusV0).xyz),
		vec3(( MV * v4MinusV0).xyz) )) +
	normalize( cross(vec3(( MV * v4MinusV0).xyz),
		vec3(( MV * v5MinusV0).xyz) )) +
	normalize( cross(vec3(( MV * v5MinusV0).xyz),
		vec3(( MV * v6MinusV0).xyz) )) +
	normalize( cross(vec3(( MV * v6MinusV0).xyz),
		vec3(( MV * v1MinusV0).xyz) ));

	vec4 c = texture(rgbTexture, uv);
  	gl_Position = MVP * vec4(vertex.x, computeHeight(c.x, c.y, c.z), vertex.z,
		1);

}
