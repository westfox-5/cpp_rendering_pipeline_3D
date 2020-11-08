#pragma once // include only one time at compile-time

#include <array>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <functional>

#include "math.h"
#include "mesh.h"
#include "target.h"
#include "transformations.h"


namespace rpl {
    
    template <typename target_t>
    class Engine {
    public:
        Engine(
            const TargetInterface<target_t>* target,
            const std::function<target_t(float)> shader
        ) :
            target_(target),
            shader_(shader)
        {
            zBuff = new float*[ScreenHeight()];
            for(int i=0; i<ScreenHeight(); i++) {
                float * subArr = new float[ScreenWidth()];
                for(int j=0; j<ScreenWidth(); j++)
                    subArr[j] = 1000000000000.0f;
                zBuff[i] = subArr;
            }

            can_run = true;
        }

        ~Engine()
        { 
            for(int i=0; i<ScreenHeight(); i++) 
                delete[] zBuff[i];
            delete zBuff;
        }
    public:
        virtual bool OnCreate()  /*user defined*/   = 0;
        virtual void OnDestroy() /*user defined*/   = 0;
    public:

        int32_t ScreenWidth() { return target_->ScreenWidth(); }
        int32_t ScreenHeight() { return target_->ScreenHeight(); }

        void Run() {

            // initialize user content
            if (!OnCreate())  {
                can_run = false;
            }

            // end of the games
            OnDestroy();
        }

        void DrawTriangle(std::array<rpl::Math::Vector3D, 3> vertices)
        {
            TransformViewport(vertices);

            float area = Area(vertices);
            if (area <= 0) return;
            area = 1/area;

            int xMax, xMin, yMax, yMin;
            BoundingBox(xMax, xMin, yMax, yMin, vertices);

            std::cout << "------------------"<<std::endl;
            std::cout << "BOUNDING BOX: " <<std::endl;
            std::cout << "x: [" << xMin << ", " << xMax <<"]" <<std::endl;
            std::cout << "y: [" << yMin << ", " << yMax <<"]" <<std::endl;
            std::cout << "------------------"<<std::endl<<std::endl;

            rpl::Math::Vector3D zVals = {vertices[0].z,vertices[1].z,vertices[2].z};
            // float A01 = vertices[0].y - vertices[1].y, B01= vertices[1].x - vertices[0].x;
            // float A12 = vertices[1].y - vertices[2].y, B12= vertices[2].x - vertices[1].x;
            // float A20 = vertices[2].y - vertices[0].y, B20= vertices[0].x - vertices[2].x;
            
            // rpl::Math::Vector3D minPoint = {(float)xMin, (float)yMin, 0};


            // rpl::Math::Vector3D e_row; //= BaryCentrics(vertices, minPoint);
            rpl::Math::Vector3D e;

            // e_row.x = edgeFun(vertices[1], vertices[2], minPoint);
            // e_row.y = edgeFun(vertices[2], vertices[0], minPoint);
            // e_row.z = edgeFun(vertices[0], vertices[1], minPoint);

            
            rpl::Math::Vector2D point;
            for( point.y = yMin; point.y<yMax; ++point.y) 
            {
                // e.x = e_row.x;
                // e.y = e_row.y;
                // e.z = e_row.z;

                for (point.x=xMin; point.x<xMax; ++point.x) 
                {   
                    e.x = edgeFun(vertices[1], vertices[2], point);
                    e.y = edgeFun(vertices[2], vertices[0], point);
                    e.z = edgeFun(vertices[0], vertices[1], point);

                    if ( (e.x >= 0) && (e.y >= 0 ) && (e.z >= 0 ))
                    {
                        // point inside triangle!
                        //std::cout << "------------------"<<std::endl;
                        // std::cout << "BARYCENTRICS FOR POINT: " << row<< ", "<< col <<std::endl;
                        // std::cout << bcs.x << ", "<< bcs.y << ", "<< bcs.z<<std::endl; 
                        //std::cout << "POINT "<< point <<" IS IN TRIANGLE" <<std::endl;                        

                        //float z = InterpolateDepth( vertices, {(float)row,(float)col}, bcs );

                        const float z = rpl::Math::Vector3D::dotProduct((e*area), zVals);
                        //std::cout << "Z IS: " << z << std::endl << "Z_BUFF["<<row<<"]["<<col<<"] IS: " << zBuff[row][col] <<std::endl; 

                        
                        if (z <= zBuff[ (int)point.y][ (int)point.x]) {
                            zBuff[ (int)point.y][ (int)point.x] = z;

                           

                            target_->Set(point.y, point.x, shader_(z));
                            //std::cout << "------------------"<<std::endl;

                        }
                    }

                    //One step to the right
                    // e.x += A12;
                    // e.y += A20;
                    // e.z += A01;
                }

                //One row step
                // e_row.x += B12;
                // e_row.y += B20;
                // e_row.z += B01;
            }
        }
    private:
        float edgeFun(rpl::Math::Vector2D &a,rpl::Math::Vector2D &b, rpl::Math::Vector2D &c) {
            return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
        };

        void BoundingBox(int &xMax, int &xMin, int &yMax, int &yMin,std::array<rpl::Math::Vector3D, 3> vertices){
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


        rpl::Math::Vector3D BaryCentrics(std::array<rpl::Math::Vector3D, 3> vertices, rpl::Math::Vector2D pointToTest) {
            // barycentric function
            // http://www.inf.ed.ac.uk/teaching/courses/cg/lectures/cg7_2016.pdf
            // auto edgeFun = [&]( rpl::Math::Vector2D point,rpl::Math::Vector3D vA, rpl::Math::Vector3D vB) {
            //     return (vA.y - vB.y)*point.x + (vB.x - vA.x)*point.y + vA.x*vB.y - vB.x*vA.y;
            // };
            
            auto alpha = // f[1,2](x,y) / f[1,2](x0,y0)
                edgeFun(pointToTest, vertices[1], vertices[2]) / 
                edgeFun({vertices[0].x, vertices[0].y}, vertices[1], vertices[2]);
            
            auto beta = // f[2,0](x,y) / f[2,0](x1,y1)
                edgeFun(pointToTest, vertices[2], vertices[0]) / 
                edgeFun({vertices[1].x, vertices[1].y}, vertices[2], vertices[0]);
            
            auto gamma = // f[0,1](x,y) / f[0,1](x2,y2)
                edgeFun(pointToTest, vertices[0], vertices[1]) / 
                edgeFun({vertices[2].x, vertices[2].y}, vertices[0], vertices[1]);
            
            return {alpha, beta, gamma};

        }

        float Area(std::array<rpl::Math::Vector3D, 3> vertices) {
            return edgeFun(vertices[0],vertices[1],vertices[2]);
        }


        float InterpolateDepth(std::array<rpl::Math::Vector3D, 3> vertices, rpl::Math::Vector2D point, rpl::Math::Vector3D barycentrics ) {
            // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation

            float v0x = vertices[0].x / vertices[0].z;
            float v1x = vertices[1].x / vertices[1].z;

            float delta = (v1x - v0x) != 0 ? (point.x - v0x) / (v1x - v0x) : (point.x - v0x);

            float zInverse = (1 / vertices[0].z) * (1-delta)  +  (1/vertices[1].z) * delta;

            return 1 / zInverse;
        }

        void TransformViewport(std::array<rpl::Math::Vector3D, 3> vertices) {
            for(int i = 0; i < 3; ++i){
                //Adding half a pixel to avoid gaps on small vertex values
                vertices.at(i).x = ((vertices.at(i).x + 1 ) * ScreenWidth() * 0.5)  + 0.5;
                vertices.at(i).y = ((vertices.at(i).y + 1 ) * ScreenHeight() * 0.5) + 0.5;
            }
        }

    private:
        bool can_run;
        int32_t width_, height_;

        std::vector<target_t> buff;
        float** zBuff;
    protected:
        const TargetInterface<target_t>* target_;
        const std::function<target_t(float)> shader_;
    };
}