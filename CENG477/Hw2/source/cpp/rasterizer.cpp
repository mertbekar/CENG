#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "hw2_types.h"
#include "hw2_math_ops.h"
#include "hw2_file_ops.h"
#include <iostream>
#include <vector>

using namespace std;

Camera cameras[100];
int numberOfCameras = 0;

Model models[1000];
int numberOfModels = 0;

Color colors[100000];
int numberOfColors = 0;

Translation translations[1000];
int numberOfTranslations = 0;

Rotation rotations[1000];
int numberOfRotations = 0;

Scaling scalings[1000];
int numberOfScalings = 0;

Vec3 vertices[100000];
int numberOfVertices = 0;

Color backgroundColor;

// backface culling setting, default disabled
int backfaceCullingSetting = 0;
int culled = 0;

Color **image;


typedef struct {
    double vertex[3][3];
    int colorId[3][3];
} newTriangle;

vector<vector<newTriangle> >  modelVector;

/*
	Initializes image with background color
*/
void initializeImage(Camera cam) {
    int i, j;

    for (i = 0; i < cam.sizeX; i++)
    {
        for (j = 0; j < cam.sizeY; j++)
        {
            image[i][j].r = backgroundColor.r;
            image[i][j].g = backgroundColor.g;
            image[i][j].b = backgroundColor.b;
        }
    }
}

void cameraTransformation_(Camera cam, double cameraTransformation[4][4])
{
    cameraTransformation[0][0] = cam.u.x;
    cameraTransformation[0][1] = cam.u.y;
    cameraTransformation[0][2] = cam.u.z;
    cameraTransformation[0][3] = -(cam.u.x*cam.pos.x + cam.u.y*cam.pos.y + cam.u.z*cam.pos.z);

    cameraTransformation[1][0] = cam.v.x;
    cameraTransformation[1][1] = cam.v.y;
    cameraTransformation[1][2] = cam.v.z;
    cameraTransformation[1][3] = -(cam.v.x*cam.pos.x + cam.v.y*cam.pos.y + cam.v.z*cam.pos.z);

    cameraTransformation[2][0] = cam.w.x;
    cameraTransformation[2][1] = cam.w.y;
    cameraTransformation[2][2] = cam.w.z;
    cameraTransformation[2][3] = -(cam.w.x*cam.pos.x + cam.w.y*cam.pos.y + cam.w.z*cam.pos.z);

    cameraTransformation[3][0] = 0;
    cameraTransformation[3][1] = 0;
    cameraTransformation[3][2] = 0;
    cameraTransformation[3][3] = 1;

}

void perspectiveProjection_(Camera cam, double perspectiveProjection[4][4])
{
    perspectiveProjection[0][0] = (2*cam.n)/(cam.r-cam.l);
    perspectiveProjection[0][1] = 0;
    perspectiveProjection[0][2] = (cam.r+cam.l)/(cam.r-cam.l);
    perspectiveProjection[0][3] = 0;

    perspectiveProjection[1][0] = 0;
    perspectiveProjection[1][1] = (2*cam.n)/(cam.t-cam.b);
    perspectiveProjection[1][2] = (cam.t+cam.b)/(cam.t-cam.b);
    perspectiveProjection[1][3] = 0;

    perspectiveProjection[2][0] = 0;
    perspectiveProjection[2][1] = 0;
    perspectiveProjection[2][2] = -(cam.f+cam.n)/(cam.f-cam.n);
    perspectiveProjection[2][3] = -(2*cam.f*cam.n)/(cam.f-cam.n);

    perspectiveProjection[3][0] = 0;
    perspectiveProjection[3][1] = 0;
    perspectiveProjection[3][2] = -1;
    perspectiveProjection[3][3] = 0;
}

void viewportTransformation_(Camera cam, double viewportTransformation[3][4])
{
    viewportTransformation[0][0] = cam.sizeX/2.0;
    viewportTransformation[0][1] = 0;
    viewportTransformation[0][2] = 0;
    viewportTransformation[0][3] = (cam.sizeX-1)/2.0;

    viewportTransformation[1][0] = 0;
    viewportTransformation[1][1] = cam.sizeY/2.0;
    viewportTransformation[1][2] = 0;
    viewportTransformation[1][3] = (cam.sizeY-1)/2.0;

    viewportTransformation[2][0] = 0;
    viewportTransformation[2][1] = 0;
    viewportTransformation[2][2] = 0.5;
    viewportTransformation[2][3] = 0.5;

}

void modelTranslation(double translationResult[4][4], Translation translationVector)
{
    translationResult[0][0] = 1;
    translationResult[0][1] = 0;
    translationResult[0][2] = 0;
    translationResult[0][3] = translationVector.tx;

    translationResult[1][0] = 0;
    translationResult[1][1] = 1;
    translationResult[1][2] = 0;
    translationResult[1][3] = translationVector.ty;

    translationResult[2][0] = 0;
    translationResult[2][1] = 0;
    translationResult[2][2] = 1;
    translationResult[2][3] = translationVector.tz;

    translationResult[3][0] = 0;
    translationResult[3][1] = 0;
    translationResult[3][2] = 0;
    translationResult[3][3] = 1;
}

void modelRotation(double rotationResult[4][4], Rotation rotationVector)
{
    double rMatrix[4][4], matrix[4][4], matrix2[4][4], result[4][4], a, b, c;
    Vec3 uVector, vVector, wVector;

    uVector.x = rotationVector.ux;
    uVector.y = rotationVector.uy;
    uVector.z = rotationVector.uz;
    uVector = normalizeVec3(uVector);

    if(abs(uVector.x) < abs(uVector.y))
    {
        if(abs(uVector.x) < abs(uVector.z))
        {
            vVector.x = 0;
            vVector.y = -uVector.z;
            vVector.z = uVector.y;
        }
        else
        {
            vVector.x = -uVector.y;
            vVector.y = uVector.x;
            vVector.z = 0;
        }
    }
    else
    {
        if(abs(uVector.y) < abs(uVector.z))
        {
            vVector.x = -uVector.z;
            vVector.y = 0;
            vVector.z = uVector.x;
        }
        else
        {
            vVector.x = -uVector.y;
            vVector.y = uVector.x;
            vVector.z = 0;
        }
    }
    vVector = normalizeVec3(vVector);
    wVector = crossProductVec3(uVector, vVector);
    wVector = normalizeVec3(wVector);

    matrix[0][0] = uVector.x;
    matrix[0][1] = vVector.x;
    matrix[0][2] = wVector.x;
    matrix[0][3] = 0;

    matrix[1][0] = uVector.y;
    matrix[1][1] = vVector.y;
    matrix[1][2] = wVector.y;
    matrix[1][3] = 0;

    matrix[2][0] = uVector.z;
    matrix[2][1] = vVector.z;
    matrix[2][2] = wVector.z;
    matrix[2][3] = 0;

    matrix[3][0] = 0;
    matrix[3][1] = 0;
    matrix[3][2] = 0;
    matrix[3][3] = 1;

    matrix2[0][0] = uVector.x;
    matrix2[0][1] = uVector.y;
    matrix2[0][2] = uVector.z;
    matrix2[0][3] = 0;

    matrix2[1][0] = vVector.x;
    matrix2[1][1] = vVector.y;
    matrix2[1][2] = vVector.z;
    matrix2[1][3] = 0;

    matrix2[2][0] = wVector.x;
    matrix2[2][1] = wVector.y;
    matrix2[2][2] = wVector.z;
    matrix2[2][3] = 0;

    matrix2[3][0] = 0;
    matrix2[3][1] = 0;
    matrix2[3][2] = 0;
    matrix2[3][3] = 1;

    rMatrix[0][0] = 1;
    rMatrix[0][1] = 0;
    rMatrix[0][2] = 0;
    rMatrix[0][3] = 0;

    rMatrix[1][0] = 0;
    rMatrix[1][1] = cos((rotationVector.angle*M_PI)/180.0);
    rMatrix[1][2] = -(sin((rotationVector.angle*M_PI)/180.0));
    rMatrix[1][3] = 0;

    rMatrix[2][0] = 0;
    rMatrix[2][1] = sin((rotationVector.angle*M_PI)/180.0);
    rMatrix[2][2] = cos((rotationVector.angle*M_PI)/180.0);
    rMatrix[2][3] = 0;

    rMatrix[3][0] = 0;
    rMatrix[3][1] = 0;
    rMatrix[3][2] = 0;
    rMatrix[3][3] = 1;

    multiplyMatrixWithMatrix(result, rMatrix, matrix2);

    multiplyMatrixWithMatrix(rotationResult, matrix, result);

}

void modelScaling(double scalingResult[4][4], Scaling scalingVector)
{
    scalingResult[0][0] = scalingVector.sx;
    scalingResult[0][1] = 0;
    scalingResult[0][2] = 0;
    scalingResult[0][3] = 0;

    scalingResult[1][0] = 0;
    scalingResult[1][1] = scalingVector.sy;
    scalingResult[1][2] = 0;
    scalingResult[1][3] = 0;

    scalingResult[2][0] = 0;
    scalingResult[2][1] = 0;
    scalingResult[2][2] = scalingVector.sz;
    scalingResult[2][3] = 0;

    scalingResult[3][0] = 0;
    scalingResult[3][1] = 0;
    scalingResult[3][2] = 0;
    scalingResult[3][3] = 1;
}

void multiply3x4(double r[3], double m[3][4], double v[4])
{
    int i, j;
    double total;
    for (i = 0; i < 3; i++) {
        total = 0;
        for (j = 0; j < 4; j++)
            total += m[i][j] * v[j];
        r[i] = total;
    }
}


/*
	Transformations, culling, rasterization are done here.
	You can define helper functions inside this file (rasterizer.cpp) only.
	Using types in "hw2_types.h" and functions in "hw2_math_ops.cpp" will speed you up while working.
*/
void forwardRenderingPipeline(Camera cam) {
    // TODO: IMPLEMENT HERE
    cout<<"*****************"<<endl;
    double cameraTransformation[4][4], perspectiveProjection[4][4];
    double viewportTransformation[3][4];
    //makeIdentityMatrix(resultModel);
    //makeIdentityMatrix(resultViewing);
    //makeIdentityMatrix(deep);
    cameraTransformation_(cam, cameraTransformation);
    perspectiveProjection_(cam, perspectiveProjection);
    viewportTransformation_(cam, viewportTransformation);
    for(int i = 0; i<numberOfModels; i++)
    {
        double translationResult[4][4], rotationResult[4][4], scalingResult[4][4], resultModel[4][4], resultViewing[4][4], deep[4][4], transformPoint[4], deepPoint[4];
        makeIdentityMatrix(resultModel);
        makeIdentityMatrix(resultViewing);
        makeIdentityMatrix(deep);
        makeIdentityMatrix(rotationResult);
        Vec3 eye, normal;
        vector<newTriangle> triangleModel;
        culled = 0;
        //identity to here
        //triangleModel.clear();
        for(int k = 0; k<models[i].numberOfTransformations; k++)
        {
            if(models[i].transformationTypes[k] == 't')
            {
                modelTranslation(translationResult, translations[models[i].transformationIDs[k]]);
                multiplyMatrixWithMatrix(deep, translationResult, resultModel);
                for(int u = 0; u<4; u++)
                {
                    for(int v = 0; v<4; v++)
                    {
                        resultModel[u][v] = deep[u][v];
                    }
                }
            }
            if(models[i].transformationTypes[k] == 'r')
            {
                modelRotation(rotationResult, rotations[models[i].transformationIDs[k]]);
                multiplyMatrixWithMatrix(deep, rotationResult, resultModel);
                for(int u = 0; u<4; u++)
                {
                    for(int v = 0; v<4; v++)
                    {
                        resultModel[u][v] = deep[u][v];
                    }
                }
            }
            if(models[i].transformationTypes[k] == 's')
            {
                modelScaling(scalingResult, scalings[models[i].transformationIDs[k]]);
                multiplyMatrixWithMatrix(deep, scalingResult, resultModel);
                for(int u = 0; u<4; u++)
                {
                    for(int v = 0; v<4; v++)
                    {
                        resultModel[u][v] = deep[u][v];
                    }
                }
            }
        }

        multiplyMatrixWithMatrix(resultViewing, cameraTransformation, resultModel);
        makeIdentityMatrix(resultModel);

        multiplyMatrixWithMatrix(deep, perspectiveProjection, resultViewing);
        for(int u = 0; u<4; u++)
        {
            for(int v = 0; v<4; v++)
            {
                resultViewing[u][v] = deep[u][v];
            }
        }

        for(int j = 0; j<models[i].numberOfTriangles; j++)
        {
            double point1[3], point2[3], point3[3];
            newTriangle triangle;
            transformPoint[0] = vertices[models[i].triangles[j].vertexIds[0]].x;
            transformPoint[1] = vertices[models[i].triangles[j].vertexIds[0]].y;
            transformPoint[2] = vertices[models[i].triangles[j].vertexIds[0]].z;
            transformPoint[3] = 1.0;

            multiplyMatrixWithVec4d(deepPoint, resultViewing, transformPoint);

            for(int u = 0; u<4; u++)
            {
                transformPoint[u] = deepPoint[u];
            }
            Vec3 v0, v1, v2;
            
            v0.x = transformPoint[0];
            v0.y = transformPoint[1];
            v0.z = transformPoint[2];

            transformPoint[0] /= transformPoint[3];
            transformPoint[1] /= transformPoint[3];
            transformPoint[2] /= transformPoint[3];
            transformPoint[3] /= transformPoint[3];

            /*v0 = vertices[models[i].triangles[j].vertexIds[0]];
            v1 = vertices[models[i].triangles[j].vertexIds[1]];
            v2 = vertices[models[i].triangles[j].vertexIds[2]];*/

            multiply3x4(point1, viewportTransformation, transformPoint);

            triangle.vertex[0][0] = (int)point1[0];
            triangle.vertex[0][1] = (int)point1[1];
            triangle.vertex[0][2] = (int)point1[2];

            triangle.colorId[0][0] = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].r;
            triangle.colorId[0][1] = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].g;
            triangle.colorId[0][2] = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].b;
            /*image[(int)point1[0]][(int)point1[1]].r = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].r;
            image[(int)point1[0]][(int)point1[1]].g = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].g;
            image[(int)point1[0]][(int)point1[1]].b = colors[vertices[models[i].triangles[j].vertexIds[0]].colorId].b;*/

            transformPoint[0] = vertices[models[i].triangles[j].vertexIds[1]].x;
            transformPoint[1] = vertices[models[i].triangles[j].vertexIds[1]].y;
            transformPoint[2] = vertices[models[i].triangles[j].vertexIds[1]].z;
            transformPoint[3] = 1.0;

            multiplyMatrixWithVec4d(deepPoint, resultViewing, transformPoint);
            for(int u = 0; u<4; u++)
            {
                transformPoint[u] = deepPoint[u];
            }

            v1.x = transformPoint[0];
            v1.y = transformPoint[1];
            v1.z = transformPoint[2];

            transformPoint[0] /= transformPoint[3];
            transformPoint[1] /= transformPoint[3];
            transformPoint[2] /= transformPoint[3];
            transformPoint[3] /= transformPoint[3];

            multiply3x4(point2, viewportTransformation, transformPoint);

            triangle.vertex[1][0] = (int)point2[0];
            triangle.vertex[1][1] = (int)point2[1];
            triangle.vertex[1][2] = (int)point2[2];

            triangle.colorId[1][0] = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].r;
            triangle.colorId[1][1] = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].g;
            triangle.colorId[1][2] = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].b;

            /*image[(int)point2[0]][(int)point2[1]].r = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].r;
            image[(int)point2[0]][(int)point2[1]].g = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].g;
            image[(int)point2[0]][(int)point2[1]].b = colors[vertices[models[i].triangles[j].vertexIds[1]].colorId].b;*/

            transformPoint[0] = vertices[models[i].triangles[j].vertexIds[2]].x;
            transformPoint[1] = vertices[models[i].triangles[j].vertexIds[2]].y;
            transformPoint[2] = vertices[models[i].triangles[j].vertexIds[2]].z;
            transformPoint[3] = 1.0;

            multiplyMatrixWithVec4d(deepPoint, resultViewing, transformPoint);

            for(int u = 0; u<4; u++)
            {
                transformPoint[u] = deepPoint[u];
            }

            v2.x = transformPoint[0];
            v2.y = transformPoint[1];
            v2.z = transformPoint[2];

            transformPoint[0] /= transformPoint[3];
            transformPoint[1] /= transformPoint[3];
            transformPoint[2] /= transformPoint[3];
            transformPoint[3] /= transformPoint[3];


            multiply3x4(point3, viewportTransformation, transformPoint);

            triangle.vertex[2][0] = (int)point3[0];
            triangle.vertex[2][1] = (int)point3[1];
            triangle.vertex[2][2] = (int)point3[2];

            triangle.colorId[2][0] = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].r;
            triangle.colorId[2][1] = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].g;
            triangle.colorId[2][2] = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].b;
            /*image[(int)point3[0]][(int)point3[1]].r = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].r;
            image[(int)point3[0]][(int)point3[1]].g = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].g;
            image[(int)point3[0]][(int)point3[1]].b = colors[vertices[models[i].triangles[j].vertexIds[2]].colorId].b;*/
            if(backfaceCullingSetting)
            {
                Vec3 temp;
                Vec3 center;
                center.x = (v0.x + v1.x + v2.x)/3.0;
                center.y = (v0.y + v1.y + v2.y)/3.0;
                center.z = (v0.z + v1.z + v2.z)/3.0;
                if(dotProductVec3(normalizeVec3(crossProductVec3(normalizeVec3(subtractVec3(v1, v2)), normalizeVec3(subtractVec3(v0, v2)))), normalizeVec3(subtractVec3(center, cam.v))) < 0.0)
                {
                    triangleModel.push_back(triangle);
                }
                else
                {
                    culled += 1;
                }
            }
            else
            {
                triangleModel.push_back(triangle);
            }
        }
        modelVector.push_back(triangleModel);
        if(models[i].type == 0)
        {
            for(int j = 0; j<models[i].numberOfTriangles-culled; j++)
            {
                //cout<<models[i].numberOfTriangles-culled<<endl;
                /*cout<<triangleModel[j].vertex[0][0]<<" "<<triangleModel[j].vertex[0][1]<<endl;
                cout<<triangleModel[j].vertex[1][0]<<" "<<triangleModel[j].vertex[1][1]<<endl;
                cout<<triangleModel[j].vertex[2][0]<<" "<<triangleModel[j].vertex[2][1]<<endl;*/
                int a = -1;
                int b = 0;
                int x, y;
                for(int k = 0; k<3; k++)
                {
                    a++;
                    b++;
                    if(b == 3)
                    {
                        b = 2;
                        a = 0;
                    }
                    int x0 = triangleModel[j].vertex[a][0];
                    int x1 = triangleModel[j].vertex[b][0];
                    int y0 = triangleModel[j].vertex[a][1];
                    int y1 = triangleModel[j].vertex[b][1];
                    int yMin = min(y0, y1);
                    int yMax = max(y0, y1);
                    int xMin = min(x0, x1);
                    int xMax = max(x0, x1);
                    double m = double(y1-y0)/double(x1-x0);
                    double d;
                    if(m == 0.0)
                    {
                        if((x1-x0) < 0.0)
                        {
                            int tempx0 = x0;
                            x0 = x1;
                            x1 = tempx0;
                            for(x = x0; x<=x1; x++)
                            {
                                image[x][y0].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y0].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                                image[x][y0].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            }
                        }
                    }
                    else if(m == INFINITY || m == -INFINITY)
                    {
                        for(y = yMin; y<=yMax; y++)
                        {
                            image[x0][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x0][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x0][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                        }
                    }
                    else if(m < 1.0 && m > 0.0)
                    {
                        if((x1-x0) < 0.0 && (y1-y0) < 0.0)
                        {
                            int tempx0 = x0;
                            int tempy0 = y0;
                            x0 = x1;
                            y0 = y1;
                            x1 = tempx0;
                            y1 = tempy0;
                        }
                        d = (y0-y1) + 0.5*(x1-x0);
                        y = y0;
                        for(x = x0; x <= x1; x++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            if(d<0.f)
                            {
                                y += 1;
                                d += (y0-y1) + (x1-x0);
                            }
                            else
                            {
                                d += (y0-y1);
                            }
                        }
                    }
                    else if(m >= 1.0 && m < INFINITY)
                    {
                        if((x1-x0) < 0.0 && (y1-y0) < 0.0)
                        {
                            int tempx0 = x0;
                            int tempy0 = y0;
                            x0 = x1;
                            y0 = y1;
                            x1 = tempx0;
                            y1 = tempy0;
                        }
                        x = x0;
                        d = (0.5f*(y0-y1)) + (x1-x0);
                        for(y = y0; y <= y1; y++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            if(d>0.f)
                            {
                                x += 1;
                                d += (y0-y1) + (x1-x0);
                            }
                            else
                            {
                                d += (x1-x0);
                            }
                        }
                    }
                    else if(m >= -1.0 && m <= 0.0)
                    {
                        if((x1-x0) > 0.0 && (y1-y0) < 0.0)
                        {
                            int tempx0 = x0;
                            int tempy0 = y0;
                            x0 = x1;
                            y0 = y1;
                            x1 = tempx0;
                            y1 = tempy0;
                        }
                        y = y0;
                        d = (-1.f*(y0-y1)) + 0.5f*(x1-x0);
                        for(x = x0; x >= x1; x--)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(abs(x1-x0));
                            if(d>=0.f)
                            {
                                y++;
                                d += (-1.f*(y0-y1)) + (x1-x0);
                            }
                            else
                            {
                                d += -1.f*(y0-y1);
                            }
                        }
                    }
                    else if(m < -1.0 && m > -INFINITY)
                    {
                        if((x1-x0) > 0.0 && (y1-y0) < 0.0)
                        {
                            int tempx0 = x0;
                            int tempy0 = y0;
                            x0 = x1;
                            y0 = y1;
                            x1 = tempx0;
                            y1 = tempy0;
                        }
                        x = x0;
                        d = (-0.5f*(y0-y1)) + (x1-x0);
                        for(y=y0; y<=y1; y++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(abs(y1-y0));
                            if(d<=0.f)
                            {
                                x--;
                                d += (-1.f*(y0-y1)) + (x1-x0);
                            }
                            else
                            {
                                d += (x1-x0);
                            }
                        }
                    }
                    /*int x0 = triangleModel[j].vertex[a][0];
                    int x1 = triangleModel[j].vertex[b][0];
                    int y0 = triangleModel[j].vertex[a][1];
                    int y1 = triangleModel[j].vertex[b][1];
                    int yMin = min(y0, y1);
                    int yMax = max(y0, y1);
                    int xMin = min(x0, x1);
                    int xMax = max(x0, x1);
                    int absX = abs(x1-x0);
                    int absY = abs(y1-y0);
                    double m = double(y1-y0)/(x1-x0);
                    int d = 2*absY - absX;
                    if(m == INFINITY || m == -INFINITY)
                    {
                        for(y = yMin; y<yMax; y++)
                        {
                            image[x0][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x0][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x0][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                        }
                    }
                    else if(m == 0)
                    {
                        for(x = xMin; x<xMax; x++)
                        {
                            image[x][y0].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y0].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y0].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                        }
                    }
                    else if(m <= 1.0 && m > 0.0)
                    {
                        y = yMin;
                        for(x = xMin; x<xMax; x++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            if(d>0)
                            {
                                y++;
                                d = d + 2*(absY - absX);
                            }
                            else
                            {
                                d = d + 2*absY;
                            }
                        }
                    }
                    else if(m > 1.0)
                    {
                        x = xMin;
                        for(y = yMin; y<yMax; y++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            if(d>0)
                            {
                                x++;
                                d = d + 2*(absX - absY);
                            }
                            else
                            {
                                d = d + 2*absX;
                            }
                        }
                    }
                    else if(m < 0.0 && m > -1.0)
                    {
                        y = yMax;
                        for(x = xMin; x<xMax; x++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][0]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][1]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(x-triangleModel[j].vertex[b][0]) + triangleModel[j].colorId[b][2]*abs(x-triangleModel[j].vertex[a][0]))/float(absX);
                            if(d>0)
                            {
                                y--;
                                d = d + 2*(absY - absX);
                            }
                            else
                            {
                                d = d + 2*absY;
                            }
                        }
                    }
                    else//else if(m <= -1.0)
                    {
                        x = xMax;
                        for(y = yMin; y<yMax; y++)
                        {
                            image[x][y].r = (triangleModel[j].colorId[a][0]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][0]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x][y].g = (triangleModel[j].colorId[a][1]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][1]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            image[x][y].b = (triangleModel[j].colorId[a][2]*abs(y-triangleModel[j].vertex[b][1]) + triangleModel[j].colorId[b][2]*abs(y-triangleModel[j].vertex[a][1]))/float(absY);
                            if(d>0)
                            {
                                x--;
                                d = d + 2*(absX - absY);
                            }
                            else
                            {
                                d = d + 2*absX;
                            }
                        }
                    }*/
                }
            }
        }
        if(models[i].type == 1)
        {
            for(int j = 0; j<models[i].numberOfTriangles-culled; j++)
            {
                int x0 = triangleModel[j].vertex[0][0];
                int x1 = triangleModel[j].vertex[1][0];
                int y0 = triangleModel[j].vertex[0][1];
                int y1 = triangleModel[j].vertex[1][1];
                int x2 = triangleModel[j].vertex[2][0];
                int y2 = triangleModel[j].vertex[2][1];
                int yMin = min(y0, y1);
                yMin = min(y2, yMin);
                int yMax = max(y0, y1);
                yMax = max(yMax, y2);
                int xMin = min(x0, x1);
                xMin = min(xMin, x2);
                int xMax = max(x0, x1);
                xMax = max(xMax, x2);
                //cout<<xMin<<" "<<xMax<<" "<<yMin<<" "<<yMax<<endl;
                for(int y = yMin; y<=yMax; y++)
                {
                    for(int x = xMin; x<=xMax; x++)
                    {
                        double alpha = double((x*(y1-y2) + y*(x2-x1) + x1*y2 - y1*x2))/double((x0*(y1-y2) + y0*(x2-x1) + x1*y2 - y1*x2));
                        double beta = double((x*(y2-y0) + y*(x0-x2) + x2*y0 - y2*x0))/double((x1*(y2-y0) + y1*(x0-x2) + x2*y0 - y2*x0));
                        double gamma = double((x*(y0-y1) + y*(x1-x0) + x0*y1 - y0*x1))/double((x2*(y0-y1) + y2*(x1-x0) + x0*y1 - y0*x1));
                        /*cout<<"bary start"<<endl;
                        cout<<alpha<<" "<<beta<<" "<<gamma<<endl;
                        cout<<"bary end"<<endl;*/
                        if(alpha >= 0 && beta >= 0 && gamma >= 0)
                        {
                            image[x][y].r = (int)round((triangleModel[j].colorId[0][0]*alpha + triangleModel[j].colorId[1][0]*beta + triangleModel[j].colorId[2][0]*gamma));
                            image[x][y].g = (int)round((triangleModel[j].colorId[0][1]*alpha + triangleModel[j].colorId[1][1]*beta + triangleModel[j].colorId[2][1]*gamma));
                            image[x][y].b = (int)round((triangleModel[j].colorId[0][2]*alpha + triangleModel[j].colorId[1][2]*beta + triangleModel[j].colorId[2][2]*gamma));
                        }
                    }
                }
            }
        }
    }
}


int main(int argc, char **argv) {
    int i, j;

    if (argc < 2) {
        std::cout << "Usage: ./rasterizer <scene file> <camera file>" << std::endl;
        return 1;
    }

    // read camera and scene files
    readSceneFile(argv[1]);
    readCameraFile(argv[2]);

    image = 0;

    for (i = 0; i < numberOfCameras; i++) {

        // allocate memory for image
        if (image) {
			for (j = 0; j < cameras[i].sizeX; j++) {
		        delete image[j];
		    }

			delete[] image;
		}

        image = new Color*[cameras[i].sizeX];

        if (image == NULL) {
            std::cout << "ERROR: Cannot allocate memory for image." << std::endl;
            exit(1);
        }

        for (j = 0; j < cameras[i].sizeX; j++) {
            image[j] = new Color[cameras[i].sizeY];
            if (image[j] == NULL) {
                std::cout << "ERROR: Cannot allocate memory for image." << std::endl;
                exit(1);
            }
        }


        // initialize image with basic values
        initializeImage(cameras[i]);

        // do forward rendering pipeline operations
        forwardRenderingPipeline(cameras[i]);

        // generate PPM file
        writeImageToPPMFile(cameras[i]);

        // Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
        // Notice that os_type is not given as 1 (Ubuntu) or 2 (Windows), below call doesn't do conversion.
        // Change os_type to 1 or 2, after being sure that you have ImageMagick installed.
        convertPPMToPNG(cameras[i].outputFileName, 1);
    }

    return 0;

}
