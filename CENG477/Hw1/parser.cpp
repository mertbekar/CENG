#include "parser.h"
#include "tinyxml2.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <limits.h>
#include <float.h>

using namespace std;
int cam = -1;
int xx = -1;
int yy = -1;
int zz = -1;

parser::Vec3f parser::Vec3f::Vec3fConstruct(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}

parser::Vec3f parser::Vec3f::minus(Vec3f vec1, Vec3f vec2) {
    Vec3f result;
    this->x = vec1.x - vec2.x;
    this->y = vec1.y - vec2.y;
    this->z = vec1.z - vec2.z;
    return *this;
}

float parser::Vec3f::dotProduct3f(Vec3f vec1)
{
    float result = vec1.x*this->x + vec1.y*this->y + vec1.z*this->z;
    return result;
}

parser::Vec3f parser::Vec3f::crossProduct3f(Vec3f vec1, Vec3f vec2)
{
    this->x = vec1.y*vec2.z - vec1.z*vec2.y;
    this->y = vec1.z*vec2.x - vec1.x*vec2.z;
    this->z = vec1.x*vec2.y - vec1.y*vec2.x;
    return *this;
}

bool parser::Vec3f::sameSide(Vec3f p2, Vec3f a, Vec3f b) {
    Vec3f ba, p1a, p2a, cp1, cp2;
    ba.minus(b, a);
    p1a.minus(*this, a);
    p2a.minus(p2, a);
    cp1.crossProduct3f(ba, p1a);
    cp2.crossProduct3f(ba, p2a);
    float result = cp1.dotProduct3f(cp2);
    return (result >= 0);
}


parser::Vec3f parser::Scene::intersectSphere(Vec3f originCenter, Vec3f sVector, Vec3f camPos, float &currentT, int &currentMID, int &type, int &shapeID)
{
    Vec3f tempPos;
    tempPos.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
    for(int i = 0; i<this->spheres.size(); i++)
    {
        originCenter.minus(camPos, this->vertex_data[this->spheres[i].center_vertex_id-1]);

        float dSquare = sVector.x*sVector.x + sVector.y*sVector.y + sVector.z*sVector.z;
        float rayOrigin = sVector.x*originCenter.x + sVector.y*originCenter.y + sVector.z*originCenter.z;
        float ocSquare = originCenter.x*originCenter.x + originCenter.y*originCenter.y + originCenter.z*originCenter.z;
        float discriminant = rayOrigin*rayOrigin - dSquare*(ocSquare - this->spheres[i].radius*this->spheres[i].radius);
        if(discriminant > 0.0f)
        {
            float t1 = (-rayOrigin + sqrt(discriminant))/dSquare;
            float t2 = (-rayOrigin - sqrt(discriminant))/dSquare;
            if(t1<t2 && t1>0.0f)
            {
                if(t1<currentT)
                {
                    currentT = t1;
                    currentMID = this->spheres[i].material_id;
                    type = 1;
                    shapeID = i;
                    tempPos.x = camPos.x + sVector.x*t1;
                    tempPos.y = camPos.y + sVector.y*t1;
                    tempPos.z = camPos.z + sVector.z*t1;
                }
            }
            else if(t2>0.0f)
            {
                if(t2<currentT)
                {
                    currentT = t2;
                    currentMID = this->spheres[i].material_id;
                    type = 1;
                    shapeID = i;
                    tempPos.x = camPos.x + sVector.x*t2;
                    tempPos.y = camPos.y + sVector.y*t2;
                    tempPos.z = camPos.z + sVector.z*t2;
                }
            }
        }
    }
    return tempPos;
}

parser::Vec3f parser::Scene::intersectTriangle(Vec3f sVector, Vec3f camPos, float &currentT, int &currentMID, int &type, int &shapeID)
{
    Vec3f tempPos;
    tempPos.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
    for(int i = 0; i<this->triangles.size(); i++)
    {
        float a, b, c, detA, detB, detG, detT, beta, gamma, t;
        Vec3f aVector, bVector, cVector;
        a = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v1_id-1].x;
        b = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v2_id-1].x;
        c = sVector.x;
        aVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v1_id-1].y;
        b = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v2_id-1].y;
        c = sVector.y;
        bVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v1_id-1].z;
        b = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v2_id-1].z;
        c = sVector.z;
        cVector.Vec3fConstruct(a, b, c);

        detA = this->det(aVector, bVector, cVector);

        a = vertex_data[this->triangles[i].indices.v0_id-1].x - camPos.x;
        b = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v2_id-1].x;
        c = sVector.x;
        aVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].y - camPos.y;
        b = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v2_id-1].y;
        c = sVector.y;
        bVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].z - camPos.z;
        b = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v2_id-1].z;
        c = sVector.z;
        cVector.Vec3fConstruct(a, b, c);

        detB = this->det(aVector, bVector, cVector);
        beta = detB/detA;

        a = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v1_id-1].x;
        b = vertex_data[this->triangles[i].indices.v0_id-1].x - camPos.x;
        c = sVector.x;
        aVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v1_id-1].y;
        b = vertex_data[this->triangles[i].indices.v0_id-1].y - camPos.y;
        c = sVector.y;
        bVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v1_id-1].z;
        b = vertex_data[this->triangles[i].indices.v0_id-1].z - camPos.z;
        c = sVector.z;
        cVector.Vec3fConstruct(a, b, c);

        detG = this->det(aVector, bVector, cVector);
        gamma = detG/detA;

        a = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v1_id-1].x;
        b = vertex_data[this->triangles[i].indices.v0_id-1].x - vertex_data[this->triangles[i].indices.v2_id-1].x;
        c = vertex_data[this->triangles[i].indices.v0_id-1].x - camPos.x;
        aVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v1_id-1].y;
        b = vertex_data[this->triangles[i].indices.v0_id-1].y - vertex_data[this->triangles[i].indices.v2_id-1].y;
        c = vertex_data[this->triangles[i].indices.v0_id-1].y - camPos.y;
        bVector.Vec3fConstruct(a, b, c);
        a = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v1_id-1].z;
        b = vertex_data[this->triangles[i].indices.v0_id-1].z - vertex_data[this->triangles[i].indices.v2_id-1].z;
        c = vertex_data[this->triangles[i].indices.v0_id-1].z - camPos.z;
        cVector.Vec3fConstruct(a, b, c);
        
        detT = this->det(aVector, bVector, cVector);
        t = detT/detA;

        if(beta+gamma<=1+this->shadow_ray_epsilon && 0-this->shadow_ray_epsilon<=beta && 0-this->shadow_ray_epsilon<=gamma && t>0)
        {
            if(t<currentT)
            {
                currentT = t;
                currentMID = this->triangles[i].material_id;
                type = 2;
                shapeID = i;
                tempPos.x = camPos.x + sVector.x*t;
                tempPos.y = camPos.y + sVector.y*t;
                tempPos.z = camPos.z + sVector.z*t;
            }
        }
    }
    return tempPos;
}

parser::Vec3f parser::Scene::intersectMesh(Vec3f sVector, Vec3f camPos, float &currentT, int &currentMID, int &type, int &shapeID, int &faceID)
{
    Vec3f tempPos;
    tempPos.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
    for(int i = 0; i<this->meshes.size(); i++)
    {
        for(int j = 0; j<this->meshes[i].faces.size(); j++)
        {
            float a, b, c, detA, detB, detG, detT, beta, gamma, t;
            Vec3f aVector, bVector, cVector;
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v1_id-1].x;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v2_id-1].x;
            c = sVector.x;
            aVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v1_id-1].y;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v2_id-1].y;
            c = sVector.y;
            bVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v1_id-1].z;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v2_id-1].z;
            c = sVector.z;
            cVector.Vec3fConstruct(a, b, c);

            detA = this->det(aVector, bVector, cVector);

            a = vertex_data[this->meshes[i].faces[j].v0_id-1].x - camPos.x;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v2_id-1].x;
            c = sVector.x;
            aVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].y - camPos.y;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v2_id-1].y;
            c = sVector.y;
            bVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].z - camPos.z;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v2_id-1].z;
            c = sVector.z;
            cVector.Vec3fConstruct(a, b, c);

            detB = this->det(aVector, bVector, cVector);
            beta = detB/detA;

            a = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v1_id-1].x;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].x - camPos.x;
            c = sVector.x;
            aVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v1_id-1].y;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].y - camPos.y;
            c = sVector.y;
            bVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v1_id-1].z;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].z - camPos.z;
            c = sVector.z;
            cVector.Vec3fConstruct(a, b, c);

            detG = this->det(aVector, bVector, cVector);
            gamma = detG/detA;

            a = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v1_id-1].x;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].x - vertex_data[this->meshes[i].faces[j].v2_id-1].x;
            c = vertex_data[this->meshes[i].faces[j].v0_id-1].x - camPos.x;
            aVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v1_id-1].y;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].y - vertex_data[this->meshes[i].faces[j].v2_id-1].y;
            c = vertex_data[this->meshes[i].faces[j].v0_id-1].y - camPos.y;
            bVector.Vec3fConstruct(a, b, c);
            a = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v1_id-1].z;
            b = vertex_data[this->meshes[i].faces[j].v0_id-1].z - vertex_data[this->meshes[i].faces[j].v2_id-1].z;
            c = vertex_data[this->meshes[i].faces[j].v0_id-1].z - camPos.z;
            cVector.Vec3fConstruct(a, b, c);

            detT = this->det(aVector, bVector, cVector);
            t = detT/detA;
            
            if(beta+gamma<=1+this->shadow_ray_epsilon && 0-this->shadow_ray_epsilon<=beta && 0-this->shadow_ray_epsilon<=gamma && t>0)
            {
                if(t<currentT)
                {
                    currentT = t;
                    currentMID = this->meshes[i].material_id;
                    type = 3;
                    shapeID = i;
                    faceID = j;
                    tempPos.x = camPos.x + sVector.x*t;
                    tempPos.y = camPos.y + sVector.y*t;
                    tempPos.z = camPos.z + sVector.z*t;
                }
            }
        }
    }
    return tempPos;
}

/*void calculateDiffusePong()
{

}*/

void parser::Scene::calculateRGB(Vec3f sVector, Vec3f camPos, float &currentT, int currentMID, int type, int shapeID, int faceID, Vec3f &dummyVector, Vec3f &rgb, Vec3f &rgb2, bool mirrorEff, int depth, bool flag)
{
    Vec3f originCenter;
    bool mirrorFlag = true;

    if(currentMID != -1)
    {
        if(mirrorEff)
        {
            if(flag)
            {
                rgb2.x = rgb2.x + (this->ambient_light.x * this->materials[currentMID-1].ambient.x);
                rgb2.y = rgb2.y + (this->ambient_light.y * this->materials[currentMID-1].ambient.y); 
                rgb2.z = rgb2.z + (this->ambient_light.z * this->materials[currentMID-1].ambient.z);                
            }
            else
            {
                rgb2.x =  (this->ambient_light.x * this->materials[currentMID-1].ambient.x);
                rgb2.y =  (this->ambient_light.y * this->materials[currentMID-1].ambient.y); 
                rgb2.z =  (this->ambient_light.z * this->materials[currentMID-1].ambient.z);
            }
        }
        else
        {
            rgb.x =  (this->ambient_light.x * this->materials[currentMID-1].ambient.x);
            rgb.y =  (this->ambient_light.y * this->materials[currentMID-1].ambient.y); 
            rgb.z =  (this->ambient_light.z * this->materials[currentMID-1].ambient.z);
        }

        for(int i = 0; i < this->point_lights.size(); i++)
        {
            Vec3f tempRgb, xVector, normalVector, corner1, corner2, corner3, c1c2Vector, c1c3Vector, lightPos, wVector, diffuseCoef, woVector, hVector, specularRef, tempPos1, tempPos2, tempPos3, tempPos, mirrorVector, tempMirrorVector;
            tempRgb.Vec3fConstruct(0, 0, 0);
            diffuseCoef.Vec3fConstruct(this->materials[currentMID-1].diffuse.x, this->materials[currentMID-1].diffuse.y, this->materials[currentMID-1].diffuse.z);
            lightPos.Vec3fConstruct(this->point_lights[i].position.x, this->point_lights[i].position.y, this->point_lights[i].position.z);
            xVector.Vec3fConstruct(camPos.x + sVector.x*currentT, camPos.y + sVector.y*currentT, camPos.z + sVector.z*currentT);
            wVector.minus(xVector, lightPos);
            Vec3f intens = this->point_lights[i].intensity;
            float wVectorSize = sqrt(wVector.x*wVector.x + wVector.y*wVector.y + wVector.z*wVector.z);
            wVector.x = -wVector.x / wVectorSize;
            wVector.y = -wVector.y / wVectorSize;
            wVector.z = -wVector.z / wVectorSize;
            float cosTh, cosAl;
            float phongExponent = this->materials[currentMID - 1].phong_exponent;
            float tempT1 = FLT_MAX;
            float tempT2 = FLT_MAX;
            float tempT3 = FLT_MAX;
            specularRef.Vec3fConstruct(this->materials[currentMID - 1].specular.x, this->materials[currentMID - 1].specular.y, this->materials[currentMID - 1].specular.z);
            woVector.Vec3fConstruct(-xVector.x + camPos.x, -xVector.y + camPos.y, -xVector.z + camPos.z);
            float woVectorSize = sqrt(woVector.x*woVector.x + woVector.y*woVector.y + woVector.z*woVector.z);
            woVector.Vec3fConstruct(woVector.x/woVectorSize, woVector.y/woVectorSize, woVector.z/woVectorSize);
            hVector.Vec3fConstruct(wVector.x + woVector.x, wVector.y + woVector.y, wVector.z + woVector.z);
            float hVectorSize = sqrt(hVector.x*hVector.x + hVector.y*hVector.y + hVector.z*hVector.z);
            hVector.Vec3fConstruct(hVector.x/hVectorSize, hVector.y/hVectorSize, hVector.z/hVectorSize);
            int dummy1 = -1;
            int dummy2 = 0;
            int dummy3 = -1;
            int dummy4 = -1;
            bool detector = true;
            bool shadingFlag = true;
            tempPos1.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
            tempPos2.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
            tempPos3.Vec3fConstruct(FLT_MAX, FLT_MAX, FLT_MAX);
            if(type == 1)
            {
                normalVector.minus(xVector, this->vertex_data[this->spheres[shapeID].center_vertex_id-1]);
                float normalVectorSize = sqrt(normalVector.x*normalVector.x + normalVector.y*normalVector.y + normalVector.z*normalVector.z);
                normalVector.x = normalVector.x / normalVectorSize;
                normalVector.y = normalVector.y / normalVectorSize;
                normalVector.z = normalVector.z / normalVectorSize;
                cosTh = max(0.0f, wVector.dotProduct3f(normalVector));
                cosAl = max(0.0f, normalVector.dotProduct3f(hVector));
                tempRgb.x = intens.x*diffuseCoef.x*cosTh/(wVectorSize*wVectorSize) + specularRef.x*intens.x*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.y = intens.y*diffuseCoef.y*cosTh/(wVectorSize*wVectorSize) + specularRef.y*intens.y*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.z = intens.z*diffuseCoef.z*cosTh/(wVectorSize*wVectorSize) + specularRef.z*intens.z*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                if(mirrorEff)
                {
                    rgb2.x = rgb2.x + tempRgb.x;
                    rgb2.y = rgb2.y + tempRgb.y;
                    rgb2.z = rgb2.z + tempRgb.z;
                }
                else
                {
                    rgb.x = rgb.x + tempRgb.x;
                    rgb.y = rgb.y + tempRgb.y;
                    rgb.z = rgb.z + tempRgb.z;
                }
                xVector.x = xVector.x + wVector.x*this->shadow_ray_epsilon;
                xVector.y = xVector.y + wVector.y*this->shadow_ray_epsilon;
                xVector.z = xVector.z + wVector.z*this->shadow_ray_epsilon;
                tempPos1 = this->intersectSphere(originCenter, wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos2 = this->intersectTriangle(wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos3 = this->intersectMesh(wVector, xVector, tempT1, dummy1, dummy2, dummy3, dummy4);

                if(detector && tempPos1.x != FLT_MAX && tempPos1.y != FLT_MAX && tempPos1.z != FLT_MAX)
                {
                    //if((tempPos1.x > xVector.x + this->shadow_ray_epsilon*wVector.x) && (tempPos1.y > xVector.y + this->shadow_ray_epsilon*wVector.y) && (tempPos1.z > xVector.z + this->shadow_ray_epsilon*wVector.z))
                    //{
                        if(sqrt(pow(tempPos1.x - xVector.x, 2) + pow(tempPos1.y - xVector.y, 2) + pow(tempPos1.z - xVector.z, 2)) < wVectorSize)
                        {
                            if(mirrorEff)
                            {
                                rgb2.x = rgb2.x - tempRgb.x;
                                rgb2.y = rgb2.y - tempRgb.y;
                                rgb2.z = rgb2.z - tempRgb.z;
                                detector = false;
                            }
                            else
                            {
                                rgb.x = rgb.x - tempRgb.x;
                                rgb.y = rgb.y - tempRgb.y;
                                rgb.z = rgb.z - tempRgb.z;
                                detector = false;
                            }
                        }
                    //}
                }
                if(detector && tempPos2.x != FLT_MAX && tempPos2.y != FLT_MAX && tempPos2.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos2.x - xVector.x, 2) + pow(tempPos2.y - xVector.y, 2) + pow(tempPos2.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    }
                }
                if(detector && tempPos3.x != FLT_MAX && tempPos3.y != FLT_MAX && tempPos3.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos3.x - xVector.x, 2) + pow(tempPos3.y - xVector.y, 2) + pow(tempPos3.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                        }
                    }
                }
                if(mirrorFlag && this->materials[currentMID-1].mirror.x != 0 || this->materials[currentMID-1].mirror.y != 0 || this->materials[currentMID-1].mirror.z != 0)
                {
                    mirrorVector.x = normalVector.x*normalVector.dotProduct3f(woVector)*2 - woVector.x;
                    mirrorVector.y = normalVector.y*normalVector.dotProduct3f(woVector)*2 - woVector.y;
                    mirrorVector.z = normalVector.z*normalVector.dotProduct3f(woVector)*2 - woVector.z;
                    mirrorFlag = false;
                    this->calculateMirror(mirrorVector, xVector, depth, rgb, rgb2);
                }
            }
            else if(type == 2)
            {
                corner1.Vec3fConstruct(this->vertex_data[this->triangles[shapeID].indices.v0_id-1].x, this->vertex_data[this->triangles[shapeID].indices.v0_id-1].y, this->vertex_data[this->triangles[shapeID].indices.v0_id-1].z);
                corner2.Vec3fConstruct(this->vertex_data[this->triangles[shapeID].indices.v1_id-1].x, this->vertex_data[this->triangles[shapeID].indices.v1_id-1].y, this->vertex_data[this->triangles[shapeID].indices.v1_id-1].z);
                corner3.Vec3fConstruct(this->vertex_data[this->triangles[shapeID].indices.v2_id-1].x, this->vertex_data[this->triangles[shapeID].indices.v2_id-1].y, this->vertex_data[this->triangles[shapeID].indices.v2_id-1].z);
                c1c2Vector.minus(corner2, corner1);
                c1c3Vector.minus(corner3, corner1);
                normalVector.crossProduct3f(c1c2Vector, c1c3Vector);
                float normalVectorSize = sqrt(normalVector.x*normalVector.x + normalVector.y*normalVector.y + normalVector.z*normalVector.z);
                normalVector.x = normalVector.x / normalVectorSize;
                normalVector.y = normalVector.y / normalVectorSize;
                normalVector.z = normalVector.z / normalVectorSize;
                cosTh = max(0.0f, wVector.dotProduct3f(normalVector));
                cosAl = max(0.0f, normalVector.dotProduct3f(hVector));
                tempRgb.x = intens.x*diffuseCoef.x*cosTh/(wVectorSize*wVectorSize) + specularRef.x*intens.x*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.y = intens.y*diffuseCoef.y*cosTh/(wVectorSize*wVectorSize) + specularRef.y*intens.y*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.z = intens.z*diffuseCoef.z*cosTh/(wVectorSize*wVectorSize) + specularRef.z*intens.z*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                if(mirrorEff)
                {
                    rgb2.x = rgb2.x + tempRgb.x;
                    rgb2.y = rgb2.y + tempRgb.y;
                    rgb2.z = rgb2.z + tempRgb.z;
                }
                else
                {
                    rgb.x = rgb.x + tempRgb.x;
                    rgb.y = rgb.y + tempRgb.y;
                    rgb.z = rgb.z + tempRgb.z;
                }
                xVector.x = xVector.x + wVector.x*this->shadow_ray_epsilon;
                xVector.y = xVector.y + wVector.y*this->shadow_ray_epsilon;
                xVector.z = xVector.z + wVector.z*this->shadow_ray_epsilon;
                tempPos1 = this->intersectSphere(originCenter, wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos2 = this->intersectTriangle(wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos3 = this->intersectMesh(wVector, xVector, tempT1, dummy1, dummy2, dummy3, dummy4);
                if(detector && tempPos1.x != FLT_MAX && tempPos1.y != FLT_MAX && tempPos1.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos1.x - xVector.x, 2) + pow(tempPos1.y - xVector.y, 2) + pow(tempPos1.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    }
                }
                if(detector && tempPos2.x != FLT_MAX && tempPos2.y != FLT_MAX && tempPos2.z != FLT_MAX)
                {
                    //if((tempPos2.x > xVector.x + this->shadow_ray_epsilon*wVector.x) && (tempPos2.y > xVector.y + this->shadow_ray_epsilon*wVector.y) && (tempPos2.z > xVector.z + this->shadow_ray_epsilon*wVector.z))
                    //{
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    //}
                }
                if(detector && tempPos3.x != FLT_MAX && tempPos3.y != FLT_MAX && tempPos3.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos3.x - xVector.x, 2) + pow(tempPos3.y - xVector.y, 2) + pow(tempPos3.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    }
                }
                if(mirrorFlag && this->materials[currentMID-1].mirror.x != 0 || this->materials[currentMID-1].mirror.y != 0 || this->materials[currentMID-1].mirror.z != 0)
                {
                    mirrorVector.x = normalVector.x*normalVector.dotProduct3f(woVector)*2 - woVector.x;
                    mirrorVector.y = normalVector.y*normalVector.dotProduct3f(woVector)*2 - woVector.y;
                    mirrorVector.z = normalVector.z*normalVector.dotProduct3f(woVector)*2 - woVector.z;
                    mirrorFlag = false;
                    this->calculateMirror(mirrorVector, xVector, depth, rgb, rgb2);
                }
            }
            else if(type == 3)
            {
                corner1.Vec3fConstruct(this->vertex_data[this->meshes[shapeID].faces[faceID].v0_id-1].x, this->vertex_data[this->meshes[shapeID].faces[faceID].v0_id-1].y, this->vertex_data[this->meshes[shapeID].faces[faceID].v0_id-1].z);
                corner2.Vec3fConstruct(this->vertex_data[this->meshes[shapeID].faces[faceID].v1_id-1].x, this->vertex_data[this->meshes[shapeID].faces[faceID].v1_id-1].y, this->vertex_data[this->meshes[shapeID].faces[faceID].v1_id-1].z);
                corner3.Vec3fConstruct(this->vertex_data[this->meshes[shapeID].faces[faceID].v2_id-1].x, this->vertex_data[this->meshes[shapeID].faces[faceID].v2_id-1].y, this->vertex_data[this->meshes[shapeID].faces[faceID].v2_id-1].z);
                c1c2Vector.minus(corner2, corner1);
                c1c3Vector.minus(corner3, corner1);
                normalVector.crossProduct3f(c1c2Vector, c1c3Vector);
                float normalVectorSize = sqrt(normalVector.x*normalVector.x + normalVector.y*normalVector.y + normalVector.z*normalVector.z);
                normalVector.x = normalVector.x / normalVectorSize;
                normalVector.y = normalVector.y / normalVectorSize;
                normalVector.z = normalVector.z / normalVectorSize;
                cosTh = max(0.0f, wVector.dotProduct3f(normalVector));
                cosAl = max(0.0f, normalVector.dotProduct3f(hVector));
                tempRgb.x = intens.x*diffuseCoef.x*cosTh/(wVectorSize*wVectorSize) + specularRef.x*intens.x*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.y = intens.y*diffuseCoef.y*cosTh/(wVectorSize*wVectorSize) + specularRef.y*intens.y*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                tempRgb.z = intens.z*diffuseCoef.z*cosTh/(wVectorSize*wVectorSize) + specularRef.z*intens.z*pow(cosAl, phongExponent)/(wVectorSize*wVectorSize);
                if(mirrorEff)
                {
                    rgb2.x = rgb2.x + tempRgb.x;
                    rgb2.y = rgb2.y + tempRgb.y;
                    rgb2.z = rgb2.z + tempRgb.z;
                }
                else
                {
                    rgb.x = rgb.x + tempRgb.x;
                    rgb.y = rgb.y + tempRgb.y;
                    rgb.z = rgb.z + tempRgb.z;
                }
                xVector.x = xVector.x + wVector.x*this->shadow_ray_epsilon;
                xVector.y = xVector.y + wVector.y*this->shadow_ray_epsilon;
                xVector.z = xVector.z + wVector.z*this->shadow_ray_epsilon;
                tempPos1 = this->intersectSphere(originCenter, wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos2 = this->intersectTriangle(wVector, xVector, tempT1, dummy1, dummy2, dummy3);
                tempPos3 = this->intersectMesh(wVector, xVector, tempT1, dummy1, dummy2, dummy3, dummy4);

                if(detector && tempPos1.x != FLT_MAX && tempPos1.y != FLT_MAX && tempPos1.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos1.x - xVector.x, 2) + pow(tempPos1.y - xVector.y, 2) + pow(tempPos1.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    }
                }
                if(detector && tempPos2.x != FLT_MAX && tempPos2.y != FLT_MAX && tempPos2.z != FLT_MAX)
                {
                    if(sqrt(pow(tempPos2.x - xVector.x, 2) + pow(tempPos2.y - xVector.y, 2) + pow(tempPos2.z - xVector.z, 2)) < wVectorSize)
                    {
                        if(mirrorEff)
                        {
                            rgb2.x = rgb2.x - tempRgb.x;
                            rgb2.y = rgb2.y - tempRgb.y;
                            rgb2.z = rgb2.z - tempRgb.z;
                            detector = false;
                        }
                        else
                        {
                            rgb.x = rgb.x - tempRgb.x;
                            rgb.y = rgb.y - tempRgb.y;
                            rgb.z = rgb.z - tempRgb.z;
                            detector = false;
                        }
                    }
                }
                if(detector && tempPos3.x != FLT_MAX && tempPos3.y != FLT_MAX && tempPos3.z != FLT_MAX)
                {
                    //if((tempPos3.x != xVector.x - this->shadow_ray_epsilon*wVector.x) && (tempPos3.y != xVector.y - this->shadow_ray_epsilon*wVector.y) && (tempPos3.z != xVector.z - this->shadow_ray_epsilon*wVector.z))
                    //{
                        if(sqrt(pow(tempPos3.x - xVector.x, 2) + pow(tempPos3.y - xVector.y, 2) + pow(tempPos3.z - xVector.z, 2)) < wVectorSize)
                        {
                            if(mirrorEff)
                            {
                                rgb2.x = rgb2.x - tempRgb.x;
                                rgb2.y = rgb2.y - tempRgb.y;
                                rgb2.z = rgb2.z - tempRgb.z;
                                detector = false;
                            }
                            else
                            {
                                rgb.x = rgb.x - tempRgb.x;
                                rgb.y = rgb.y - tempRgb.y;
                                rgb.z = rgb.z - tempRgb.z;
                                detector = false;
                            }
                        }
                    //}
                }
                if(mirrorFlag && this->materials[currentMID-1].mirror.x != 0 || this->materials[currentMID-1].mirror.y != 0 || this->materials[currentMID-1].mirror.z != 0)
                {
                    mirrorVector.x = normalVector.x*normalVector.dotProduct3f(woVector)*2 - woVector.x;
                    mirrorVector.y = normalVector.y*normalVector.dotProduct3f(woVector)*2 - woVector.y;
                    mirrorVector.z = normalVector.z*normalVector.dotProduct3f(woVector)*2 - woVector.z;
                    mirrorFlag = false;
                    this->calculateMirror(mirrorVector, xVector, depth, rgb, rgb2);
                }
            }
        }
        
    }
}

void parser::Scene::calculateMirror(Vec3f mirrorVector, Vec3f xVector, int depth, Vec3f &rgb, Vec3f &rgb2)
{
    float currentT = FLT_MAX;
    int currentMID = -1;
    int type = 0;
    int shapeID = -1;
    int faceID = -1;
    Vec3f originCenter, dummyVector, tempPos2, tempPos3, tempPos1;
    bool mirrorEffect = true;
    bool flag = false;
    tempPos1.x = FLT_MAX;
    tempPos1.y = FLT_MAX;
    tempPos1.z = FLT_MAX;
    tempPos2.x = FLT_MAX;
    tempPos2.y = FLT_MAX;
    tempPos2.z = FLT_MAX;
    tempPos3.x = FLT_MAX;
    tempPos3.y = FLT_MAX;
    tempPos3.z = FLT_MAX;

    depth = depth + 1;
    if(depth > this->max_recursion_depth)
    {
        //cout<<"1"<<endl;
        return;
    }
    if(depth > 1)
    {
        flag = true;
    }
    currentMID = -1;
    tempPos1 = this->intersectSphere(originCenter, mirrorVector, xVector, currentT, currentMID, type, shapeID);
    tempPos2 = this->intersectTriangle(mirrorVector, xVector, currentT, currentMID, type, shapeID);
    tempPos3 = this->intersectMesh(mirrorVector, xVector, currentT, currentMID, type, shapeID, faceID);

    if(currentMID != -1)
    {
        this->calculateRGB(mirrorVector, xVector, currentT, currentMID, type, shapeID, faceID, dummyVector, rgb, rgb2, mirrorEffect, depth, flag);
        //cout<<"2"<<endl;
        return;
    }
    else
    {
        return;
    }

}

parser::Vec3f parser::Scene::rayEquation(int x, int y, int cm, Vec3f &rgb)
{
    parser::Vec3f camPos = cameras[cm].position;
    parser::Vec3f camGaze = cameras[cm].gaze;
    parser::Vec3f upVector = cameras[cm].up;
    parser::Vec4f nearPlane = cameras[cm].near_plane;
    float nearDistance = cameras[cm].near_distance;
    float imageWidth = cameras[cm].image_width;
    float imageHeight = cameras[cm].image_height;
    Vec3f originCenter, rgb2;
    

    Vec3f mVector, uVector, vVector, qVector, sVector, dummyVector, minusGaze;
    float Su, Sv;
    float currentT = FLT_MAX;
    int currentMID = -1;
    int type = 0;
    int shapeID = -1;
    int faceID = -1;

    minusGaze.Vec3fConstruct(-camGaze.x, -camGaze.y, -camGaze.z);

    mVector.Vec3fConstruct(camPos.x, camPos.y, camPos.z);
    mVector.x = camPos.x + camGaze.x*nearDistance;
    mVector.y = camPos.y + camGaze.y*nearDistance;
    mVector.z = camPos.z + camGaze.z*nearDistance;

    uVector.crossProduct3f(upVector, minusGaze);

    float uVectorLength = sqrt(uVector.x*uVector.x + uVector.y*uVector.y + uVector.z*uVector.z);
    uVector.x = uVector.x / uVectorLength;
    uVector.y = uVector.y / uVectorLength;
    uVector.z = uVector.z / uVectorLength;

    upVector.crossProduct3f(minusGaze, uVector);
    vVector.Vec3fConstruct(upVector.x*nearPlane.w, upVector.y*nearPlane.w, upVector.z*nearPlane.w);

    qVector.Vec3fConstruct(mVector.x + uVector.x*nearPlane.x + vVector.x, mVector.y + uVector.y*nearPlane.x + vVector.y, mVector.z + uVector.z*nearPlane.x + vVector.z);
 
    Su = (nearPlane.y - nearPlane.x) * (x + 0.5f) / imageWidth;
    Sv = (nearPlane.w - nearPlane.z) * (y + 0.5f) / imageHeight;

    sVector.Vec3fConstruct(qVector.x + Su*uVector.x - Sv*upVector.x - camPos.x, qVector.y + Su*uVector.y - Sv*upVector.y - camPos.y, qVector.z + Su*uVector.z - Sv*upVector.z - camPos.z);

    float sVectorLength = sqrt(sVector.x*sVector.x + sVector.y*sVector.y + sVector.z*sVector.z);
    sVector.x = sVector.x / sVectorLength;
    sVector.y = sVector.y / sVectorLength;
    sVector.z = sVector.z / sVectorLength;
    rgb.Vec3fConstruct(this->background_color.x, this->background_color.y, this->background_color.z);
    //rgb2.Vec3fConstruct(this->background_color.x, this->background_color.y, this->background_color.z);
    rgb2.Vec3fConstruct(0, 0, 0);


    this->intersectSphere(originCenter, sVector, camPos, currentT, currentMID, type, shapeID);
    this->intersectTriangle(sVector, camPos, currentT, currentMID, type, shapeID);
    this->intersectMesh(sVector, camPos, currentT, currentMID, type, shapeID, faceID);

    this->calculateRGB(sVector, camPos, currentT, currentMID, type, shapeID, faceID, dummyVector, rgb, rgb2, false, 0, false);
    if(rgb2.x == 0 && rgb2.y == 0 && rgb2.z == 0)
    {
        rgb2.Vec3fConstruct(this->background_color.x, this->background_color.y, this->background_color.z);
    }

    rgb.x = rgb.x + rgb2.x*this->materials[currentMID-1].mirror.x;
    rgb.y = rgb.y + rgb2.y*this->materials[currentMID-1].mirror.y;
    rgb.z = rgb.z + rgb2.z*this->materials[currentMID-1].mirror.z;

    if(rgb.x > 255.0f)
    {
        rgb.x = 255.0f;
    }
    if(rgb.y > 255.0f)
    {
        rgb.y = 255.0f;
    }
    if(rgb.z > 255.0f)
    {
        rgb.z = 255.0f;
    }
    return rgb;
}

float parser::Scene::det(Vec3f a, Vec3f b, Vec3f c)
{
    return a.x*b.y*c.z + a.y*b.z*c.x + a.z*b.x*c.y - a.z*b.y*c.x - b.z*c.y*a.x - c.z*a.y*b.x;
}

parser::Vec3f parser::Scene::lightShape(int materialID)
{
    float red;
    float green;
    float blue;
    parser::Vec3f normal;
    
    red =  (this->ambient_light.x * this->materials[materialID-1].ambient.x);
    green =  (this->ambient_light.y * this->materials[materialID-1].ambient.y); 
    blue =  (this->ambient_light.z * this->materials[materialID-1].ambient.z);

    normal.Vec3fConstruct(red, green, blue);
    return normal;
}

void parser::Scene::loadFromXml(const std::string& filepath)
{
    tinyxml2::XMLDocument file;
    std::stringstream stream;

    auto res = file.LoadFile(filepath.c_str());
    if (res)
    {
        throw std::runtime_error("Error: The xml file cannot be loaded.");
    }

    auto root = file.FirstChild();
    if (!root)
    {
        throw std::runtime_error("Error: Root is not found.");
    }

    //Get BackgroundColor
    auto element = root->FirstChildElement("BackgroundColor");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0 0 0" << std::endl;
    }
    stream >> background_color.x >> background_color.y >> background_color.z;

    //Get ShadowRayEpsilon
    element = root->FirstChildElement("ShadowRayEpsilon");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0.001" << std::endl;
    }
    stream >> shadow_ray_epsilon;

    //Get MaxRecursionDepth
    element = root->FirstChildElement("MaxRecursionDepth");
    if (element)
    {
        stream << element->GetText() << std::endl;
    }
    else
    {
        stream << "0" << std::endl;
    }
    stream >> max_recursion_depth;

    //Get Cameras
    element = root->FirstChildElement("Cameras");
    element = element->FirstChildElement("Camera");
    Camera camera;
    while (element)
    {
        auto child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Gaze");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Up");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearPlane");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("NearDistance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageResolution");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("ImageName");
        stream << child->GetText() << std::endl;

        stream >> camera.position.x >> camera.position.y >> camera.position.z;
        stream >> camera.gaze.x >> camera.gaze.y >> camera.gaze.z;
        stream >> camera.up.x >> camera.up.y >> camera.up.z;
        stream >> camera.near_plane.x >> camera.near_plane.y >> camera.near_plane.z >> camera.near_plane.w;
        stream >> camera.near_distance;
        stream >> camera.image_width >> camera.image_height;
        stream >> camera.image_name;

        cameras.push_back(camera);
        element = element->NextSiblingElement("Camera");
    }

    //Get Lights
    element = root->FirstChildElement("Lights");
    auto child = element->FirstChildElement("AmbientLight");
    stream << child->GetText() << std::endl;
    stream >> ambient_light.x >> ambient_light.y >> ambient_light.z;
    element = element->FirstChildElement("PointLight");
    PointLight point_light;
    while (element)
    {
        child = element->FirstChildElement("Position");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("Intensity");
        stream << child->GetText() << std::endl;

        stream >> point_light.position.x >> point_light.position.y >> point_light.position.z;
        stream >> point_light.intensity.x >> point_light.intensity.y >> point_light.intensity.z;

        point_lights.push_back(point_light);
        element = element->NextSiblingElement("PointLight");
    }

    //Get Materials
    element = root->FirstChildElement("Materials");
    element = element->FirstChildElement("Material");
    Material material;
    while (element)
    {
        child = element->FirstChildElement("AmbientReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("DiffuseReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("SpecularReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("MirrorReflectance");
        stream << child->GetText() << std::endl;
        child = element->FirstChildElement("PhongExponent");
        stream << child->GetText() << std::endl;

        stream >> material.ambient.x >> material.ambient.y >> material.ambient.z;
        stream >> material.diffuse.x >> material.diffuse.y >> material.diffuse.z;
        stream >> material.specular.x >> material.specular.y >> material.specular.z;
        stream >> material.mirror.x >> material.mirror.y >> material.mirror.z;
        stream >> material.phong_exponent;

        materials.push_back(material);
        element = element->NextSiblingElement("Material");
    }

    //Get VertexData
    element = root->FirstChildElement("VertexData");
    stream << element->GetText() << std::endl;
    Vec3f vertex;
    while (!(stream >> vertex.x).eof())
    {
        stream >> vertex.y >> vertex.z;
        vertex_data.push_back(vertex);
    }
    stream.clear();

    //Get Meshes
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Mesh");
    Mesh mesh;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> mesh.material_id;

        child = element->FirstChildElement("Faces");
        stream << child->GetText() << std::endl;
        Face face;
        while (!(stream >> face.v0_id).eof())
        {
            stream >> face.v1_id >> face.v2_id;
            mesh.faces.push_back(face);
        }
        stream.clear();

        meshes.push_back(mesh);
        mesh.faces.clear();
        element = element->NextSiblingElement("Mesh");
    }
    stream.clear();

    //Get Triangles
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Triangle");
    Triangle triangle;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> triangle.material_id;

        child = element->FirstChildElement("Indices");
        stream << child->GetText() << std::endl;
        stream >> triangle.indices.v0_id >> triangle.indices.v1_id >> triangle.indices.v2_id;

        triangles.push_back(triangle);
        element = element->NextSiblingElement("Triangle");
    }

    //Get Spheres
    element = root->FirstChildElement("Objects");
    element = element->FirstChildElement("Sphere");
    Sphere sphere;
    while (element)
    {
        child = element->FirstChildElement("Material");
        stream << child->GetText() << std::endl;
        stream >> sphere.material_id;

        child = element->FirstChildElement("Center");
        stream << child->GetText() << std::endl;
        stream >> sphere.center_vertex_id;

        child = element->FirstChildElement("Radius");
        stream << child->GetText() << std::endl;
        stream >> sphere.radius;

        spheres.push_back(sphere);
        element = element->NextSiblingElement("Sphere");
    }
}
