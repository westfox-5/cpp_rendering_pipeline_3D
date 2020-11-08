#pragma once // include only one time at compile-time

#include <array>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <functional>
#include <algorithm>

#include "math.h"
#include "mesh.h"
#include "target.h"
#include "transformations.h"


namespace rpl {
    
    template <typename target_t>
    class Engine {
    public:
        Engine(const Target<target_t>* target,const std::function<target_t(float)> shader) :
            target_(target),
            shader_(shader)
        {
            InitializeDepthBuffer();

            this->mTransformation = rpl::Math::Identity();
            this->mProjection = rpl::Transformations::Perspective(1, -1, 1, -1, .1, 2);

            can_run = true;
        }

        ~Engine()
        { 
            for(int i=0; i<ScreenHeight(); i++) 
                delete[] depth_buff[i];
            delete depth_buff;
        }
    public:
        virtual bool OnCreate()  /*user defined*/   = 0;
        virtual void OnDestroy() /*user defined*/   = 0;
    public:

        int32_t ScreenWidth() { return target_->ScreenWidth(); }
        int32_t ScreenHeight() { return target_->ScreenHeight(); }

        void Run()
        {
            // initialize user content
            if (!OnCreate())  {
                can_run = false;
            }

            // end of the games
            OnDestroy();
        }

        void ApplyProjection(rpl::Math::Matrix4D projectionMatrix)
        {
            this->mProjection = projectionMatrix; //override
        }

        void ApplyTransformation(rpl::Math::Matrix4D scaleMatrix)
        {
            rpl::Math::Matrix4D tmpMatrix = this->mTransformation;
            tmpMatrix = tmpMatrix * scaleMatrix;

            this->mTransformation = tmpMatrix;
        }

        void DrawMesh(rpl::Mesh::Mesh mesh)
        {
            if (!&mesh) 
            {
                return;
            }

            for (auto &triangle : mesh.triangles)
            {
                std::array<rpl::Math::Vector3D, 3> transformedP;
                std::array<rpl::Math::Vector3D, 3> projectedP;

                for (int i = 0; i < 3; ++i)
                {
                    rpl::Math::MultPointMatrix(triangle.points[i], transformedP[i], mTransformation);
                    transformedP[i].z *= -1;
                    rpl::Math::MultPointMatrix(transformedP[i], projectedP[i], mProjection);

                    // Offset into the screen
                    projectedP[i].x +=  (float)ScreenWidth() / 2;
                    projectedP[i].y +=  (float)ScreenHeight() / 2;

                    projectedP[i].z = 1 / projectedP[i].z ;                    
                }

                DrawTriangle(projectedP);
            }
        }

    private:
        void InitializeDepthBuffer()
        {
            depth_buff = new float*[ScreenHeight()];
            for(int i=0; i<ScreenHeight(); i++) {
                float * subArr = new float[ScreenWidth()];
                for(int j=0; j<ScreenWidth(); j++)
                    subArr[j] = INFINITY;
                depth_buff[i] = subArr;
            }
        }

        float edgeFun(rpl::Math::Vector2D &a,rpl::Math::Vector2D &b, rpl::Math::Vector2D &c) 
        {
            return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
            //return (c.x - a.x)*(b.y - a.y) - (c.y - a.y)*(b.x - a.x);
        }

        float Area(std::array<rpl::Math::Vector3D, 3> vertices) 
        {
            return edgeFun(vertices[0],vertices[1],vertices[2]);
        }

        void BoundingBox(int &xMax, int &xMin, int &yMax, int &yMin,std::array<rpl::Math::Vector3D, 3> vertices)
        {
            xMax = std::max({vertices[0].x, vertices[1].x, vertices[2].x});
            xMin = std::min({vertices[0].x, vertices[1].x, vertices[2].x});

            yMax = std::max({vertices[0].y, vertices[1].y, vertices[2].y});
            yMin = std::min({vertices[0].y, vertices[1].y, vertices[2].y});

            //Clip against screen
            xMax = std::min(xMax, ScreenWidth() -1);
            xMin = std::max(std::min(xMin, ScreenWidth() -1), 0);

            yMax = std::min(yMax, ScreenHeight() -1);
            yMin = std::max(std::min(yMin, ScreenHeight() -1), 0);
        }
/*
        const rpl::Math::Vector3D Barycentrics(std::array<rpl::Math::Vector3D, 3> vertices, rpl::Math::Vector2D point)
        {
            // https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Relationship_with_Cartesian_or_affine_coordinates
            float l_1, l_2, l_3;
            rpl::Math::Vector3D v1, v2, v3;

            v1 = vertices[0]; v2 = vertices[1]; v3 = vertices[2];

            float denominator = (v2.y - v3.y)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.y - v3.y);
            float numerator_1 = (v2.y - v3.y)*(point.x - v3.x) + (v3.x - v2.x)*(point.y - v3.y);
            float numerator_2 = (v3.y - v1.y)*(point.x - v3.x) + (v1.x -v3.x)*(point.y - v3.y);

            l_1 = numerator_1 / denominator;
            l_2 = numerator_2 / denominator;
            l_3 = 1 - l_1 - l_2;

            return {l_1, l_2, l_3};
        }
*/
       void TransformViewport(std::array<rpl::Math::Vector3D, 3> vertices) 
       {
            for(int i = 0; i < 3; ++i){
                //Adding half a pixel to avoid gaps on small vertex values
                vertices.at(i).x = ((vertices.at(i).x + 1 ) * ScreenWidth() * 0.5)  + 0.5;
                vertices.at(i).y = ((vertices.at(i).y + 1 ) * ScreenHeight() * 0.5) + 0.5;
            }
        }

        void DrawTriangle(std::array<rpl::Math::Vector3D, 3> vertices)
        {
            TransformViewport(vertices);

            float area = Area(vertices);
            if (area <= 0) return; // invalid triangle shape
            area = 1/area;

            int xMax, xMin, yMax, yMin;
            BoundingBox(xMax, xMin, yMax, yMin, vertices);

            std::cout << "------------------"<<std::endl;
            std::cout << "BOUNDING BOX: " <<std::endl;
            std::cout << "x: [" << xMin << ", " << xMax <<"]" <<std::endl;
            std::cout << "y: [" << yMin << ", " << yMax <<"]" <<std::endl;
            std::cout << "------------------"<<std::endl<<std::endl;

            rpl::Math::Vector3D zVals = {vertices[0].z, vertices[1].z, vertices[2].z};
            rpl::Math::Vector3D point = {0, 0, 0};

            for( point.y = yMin; point.y<yMax; ++point.y) 
            {
                for (point.x=xMin; point.x<xMax; ++point.x) 
                {
                    rpl::Math::Vector3D bcs;// = Barycentrics(vertices, point);
                    bcs.x = edgeFun(vertices[1], vertices[2], point);
                    bcs.y = edgeFun(vertices[2], vertices[0], point);
                    bcs.z = edgeFun(vertices[0], vertices[1], point);

                    if ( bcs.x >= 0 && bcs.y >= 0 && bcs.z >= 0 )
                    {
                        const float oneOverZ = zVals.dotProduct(bcs * area);
                        const float z = 1 / oneOverZ;

                        if (z < depth_buff[(int)point.y][(int)point.x]) {
                            depth_buff[(int)point.y][(int)point.x] = z;

                            target_->Set(point.y, point.x, shader_(z));
                        }
                    }
                }
            }
        }

    private:
        bool can_run;
        int32_t width_, height_;
        rpl::Math::Matrix4D mTransformation, mProjection;

        std::vector<target_t> buff;
        float** depth_buff;
    protected:
        const Target<target_t>* target_;
        const std::function<target_t(float)> shader_;
    };
}