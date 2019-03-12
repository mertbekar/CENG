#version 410

// Output Color
out vec4 color;

uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;

// Texture-related data;
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;

// Data from Vertex Shader
in vec2 textureCoordinate;
in vec3 vertexNormal; // For Lighting computation
in vec3 ToLightVector; // Vector from Vertex to Light;
in vec3 ToCameraVector; // Vector from Vertex to Camera;

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

void main(){

    // Assignment Constants below
    // get the texture color
    vec4 textureColor = texture(rgbTexture, textureCoordinate);

	// apply Phong shading by using the following parameters
    vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
    vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient
    vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
    vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
    vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
    vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
    int specExp = 100; // specular exponent

    vec3 L = normalize(ToLightVector); // lightPos - vec3(fragWorldPos)
    vec3 V = normalize(ToCameraVector); // eyePos - vec3(fragWorldPos)
    vec3 H = normalize(L + V);
    vec3 N = normalize(vertexNormal);

    float NdotL = dot(N, L); // for diffuse component
    float NdotH = dot(N, H); // for specular component

	// compute ambient component
	vec4 ambient = Ia * ka;
    // compute diffuse component
	vec4 diffuse = Id * kd * max(0, NdotL);
    // compute specular component
    vec4 specular = Is * ks * pow(max(0, NdotH), specExp);

    // compute the color using the following equation
  	color = vec4(clamp( textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);
}
