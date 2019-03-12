#version 410

layout(location = 0) in vec3 position;

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

float computeHeight( float r, float g, float b ) {
	return heightFactor * (r * 0.2126 + g * 0.7152 + b * 0.0722);
}

vec4 createVec(int offsetX, float r, float g, float b, int offsetZ) {
	return vec4(position.x + offsetX, computeHeight(r, g, b), position.z + offsetZ, 1);
}

vec4[9] createColor() {
	vec4 colors[9];
	float xPlus1 = textureCoordinate.x + 1.0f / widthTexture;
	float xMinus1 = textureCoordinate.x - 1.0f / widthTexture;
	float yPlus1 = textureCoordinate.y + 1.0f / heightTexture;
	float yMinus1 = textureCoordinate.y - 1.0f / heightTexture;
	colors[0] = texture(rgbTexture, vec2(textureCoordinate.x, textureCoordinate.y));
	colors[1] = texture(rgbTexture, vec2(xPlus1, textureCoordinate.y));
	colors[2] = texture(rgbTexture, vec2(textureCoordinate.x, yMinus1));
	colors[3] = texture(rgbTexture, vec2(xMinus1, yMinus1));
	colors[4] = texture(rgbTexture, vec2(xMinus1, textureCoordinate.y));
	colors[5] = texture(rgbTexture, vec2(textureCoordinate.x, yPlus1));
	colors[6] = texture(rgbTexture, vec2(xPlus1, yPlus1));
	colors[7] = texture(rgbTexture, vec2(xMinus1, yPlus1));
	colors[8] = texture(rgbTexture, vec2(xPlus1, yMinus1));
	return colors;
}

vec4[9] createVertices(vec4 cArray[9]) {
	vec4 vertices[9];
	int i;
	for (i = 0; i < 9; i++) {
		vertices[i] = cArray[i];
	}
	vertices[0] = createVec( 0, vertices[0].x, vertices[0].y, vertices[0].z,  0);
	vertices[1] = createVec(-1, vertices[1].x, vertices[1].y, vertices[1].z,  0);
	vertices[2] = createVec( 0, vertices[2].x, vertices[2].y, vertices[2].z,  1);
	vertices[3] = createVec( 1, vertices[3].x, vertices[3].y, vertices[3].z,  1);
	vertices[4] = createVec( 1, vertices[4].x, vertices[4].y, vertices[4].z,  0);
	vertices[5] = createVec( 0, vertices[5].x, vertices[5].y, vertices[5].z, -1);
	vertices[6] = createVec(-1, vertices[6].x, vertices[6].y, vertices[6].z, -1);
	vertices[7] = createVec( 1, vertices[7].x, vertices[7].y, vertices[7].z, -1);
	vertices[8] = createVec(-1, vertices[8].x, vertices[8].y, vertices[8].z,  1);
	return vertices;
}

void main()
{
	vec4 vArray[9];
	int i;
	for (i = 0; i < 9; i++) {
		vArray[i] = createVec(0, 0, 0, 0, 0);
	}
	textureCoordinate = vec2( 1.0f - position.x / widthTexture, 1.0f - position.z / heightTexture );
	vArray = createVertices(createColor());
	vec4 c = texture(rgbTexture, textureCoordinate);
	gl_Position = MVP * vec4(position.x, computeHeight(c.x, c.y, c.z), position.z, 1);

	ToCameraVector = (-1 * MV * vArray[0]).xyz;
	vec4 l = vec4( widthTexture / 2.0f, widthTexture + heightTexture, heightTexture / 2.0f, 1);
	ToLightVector = (MV * (l - vArray[0])).xyz;

	vec4 vxMinusV0[9];
	for (i = 0; i < 9; i++) {
		vxMinusV0[i] = vArray[i] - vArray[0];
	}

	vertexNormal = vec3(0);
	for (i = 0; i < 5; i++) {
		vertexNormal += cross(vec3((MV * (vxMinusV0[i + 1])).xyz), vec3(( MV * vxMinusV0[i + 2]).xyz));
	}
	vertexNormal += cross(vec3((MV * vxMinusV0[6]).xyz), vec3(( MV * vxMinusV0[1]).xyz));

}
