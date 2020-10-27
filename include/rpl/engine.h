#pragma once // include only one time at compile-time

#include <array>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "math.h"
#include "mesh.h"
#include "target.h"
#include "shader.h"

namespace rpl {
    
    template <typename target_t>
    class Engine {
    public:
        Engine(const TargetInterface<target_t>* target)
        {
            target_ = target;
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
/*
        // Bresenham's Line Drawing Algorithm
        void DrawLine(rpl::Math::Vector3D point1, rpl::Math::Vector3D point2, const target_t (*shader)(int, int)) {
            int dx, dy, e, x, y;
            
            dx=point2.x-point1.x;
            dy=point2.y-point1.y;
        
            x=point1.x;
            y=point1.y;
        
            e=2*dy-dx;
        
            while(x<point2.x)
            {
                if(e>=0)
                {
                    target_->Set(x, y, shader(x,y) );
                    y=y+1;
                    e=e+2*dy-2*dx;
                }
                else
                {
                    target_->Set(x, y, shader(x,y) );;
                    e=e+2*dy;
                }
                x=x+1;
            }
        }
*/
        float InterpolateDepth(rpl::Math::Vector3D *vertices, rpl::Math::Vector2D point, rpl::Math::Vector3D barycentrics ) {
            // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation

            float v0x = vertices[0].x / vertices[0].z;
            float v1x = vertices[1].x / vertices[1].z;
            
            float delta = (point.x - v0x) / (v1x - v0x);

            float zInverse = (1 / vertices[0].z) * (1-delta)  +  (1/vertices[1].z) * delta;

            return 1 / zInverse;
        }

        rpl::Math::Vector3D BaryCentrics(rpl::Math::Vector3D *vertices, rpl::Math::Vector2D pointToTest) {

            // barycentric function
            // http://www.inf.ed.ac.uk/teaching/courses/cg/lectures/cg7_2016.pdf
            auto f = [&]( rpl::Math::Vector2D point,rpl::Math::Vector3D vA, rpl::Math::Vector3D vB) {
                return (vA.y - vB.y)*point.x + (vB.x - vA.x)*point.y + vA.x*vB.y - vB.x*vA.y;
            };

            auto alpha = // f[1,2](x,y) / f[1,2](x0,y0)
                f(pointToTest, vertices[1], vertices[2]) / 
                f({vertices[0].x, vertices[0].y}, vertices[1], vertices[2]);

            auto beta = // f[2,0](x,y) / f[2,0](x1,y1)
                f(pointToTest, vertices[2], vertices[0]) / 
                f({vertices[1].x, vertices[1].y}, vertices[2], vertices[0]);
            
            auto gamma = // f[0,1](x,y) / f[0,1](x2,y2)
                f(pointToTest, vertices[0], vertices[1]) / 
                f({vertices[2].x, vertices[2].y}, vertices[0], vertices[1]);
                    
            return  { alpha, beta, gamma };
        }

        void DrawTriangle(rpl::Math::Vector3D vertices[])
        {
            // vertices[0].x /= vertices[0].w; vertices[0].y /= vertices[0].w; vertices[0].z /= vertices[0].w;
            // vertices[1].x /= vertices[1].w; vertices[1].y /= vertices[1].w; vertices[1].z /= vertices[1].w;
            // vertices[2].x /= vertices[2].w; vertices[2].y /= vertices[2].w; vertices[2].z /= vertices[2].w;

            // starts at max in order to compute min
            rpl::Math::Vector2D boundingBoxMin = {(float)ScreenWidth()-1, (float)ScreenHeight()-1};
            //starts at min in order to compute max
            rpl::Math::Vector2D boundingBoxMax = {0,0};
            
            // compute min & max boundings
            for (int i=0; i<3; ++i) 
            {
                boundingBoxMin.x = std::max(0.0f, std::min(boundingBoxMin.x, vertices[i].x));
                boundingBoxMin.y = std::max(0.0f, std::min(boundingBoxMin.y, vertices[i].y));

                boundingBoxMax.x = std::min((float)ScreenWidth()-1, std::max(boundingBoxMax.x, vertices[i].x));
                boundingBoxMax.y = std::min((float)ScreenHeight()-1, std::max(boundingBoxMax.y, vertices[i].y));
            }

            std::cout << "------------------"<<std::endl;
            std::cout << "BOUNDING BOX: " <<std::endl;
            std::cout << "min: " << boundingBoxMin <<std::endl;
            std::cout << "max: " << boundingBoxMax <<std::endl;
            std::cout << "------------------"<<std::endl<<std::endl;

            // compute only in bounding box!
            //for( int row=boundingBoxMin.x; row<boundingBoxMax.x; ++row) 
            for( int row=0; row<ScreenHeight(); ++row) 
            {
                if (row >= 0 && row <= ScreenHeight())  
                {
                    //for (int col=boundingBoxMin.y; col<boundingBoxMax.y; ++col) 
                    for (int col=0; col<ScreenWidth(); ++col) 
                    {
                        if (col >=0  && col <= ScreenWidth()) 
                        {   
                            rpl::Math::Vector2D currPoint = {(float)row,(float)col};

                            auto bcs = BaryCentrics(vertices, currPoint );

                            if (bcs.x >=0 && bcs.x <=1 && 
                                bcs.y >=0 && bcs.y <=1 && 
                                bcs.z >=0 && bcs.z <=1) 
                            {
                                // point inside triangle!
                                std::cout << "------------------"<<std::endl;
                                // std::cout << "BARYCENTRICS FOR POINT: " << row<< ", "<< col <<std::endl;
                                // std::cout << bcs.x << ", "<< bcs.y << ", "<< bcs.z<<std::endl; 
                                std::cout << "POINT "<< currPoint <<" IS IN TRIANGLE" <<std::endl;                        

                                float z = InterpolateDepth( vertices, {(float)row,(float)col}, bcs );
                                std::cout << "Z IS: " << z <<  " and Z_BUFF["<<row<<"]["<<col<<"] IS: " << zBuff[row][col] <<std::endl; 

                                
                                if (z <= zBuff[row][col] ) {
                                    zBuff[row][col] = z;

                                    // SHADERRRR
                                    auto shader = [&](float z_)   
                                    {
                                        char arr[] = {'0','1','2','3','4','5','6','7','8','9'};
                                        int iz = std::round(z);
                                        //int idx = (abs((iz % 10))+9 )%10;
                                        int idx = abs(iz % 10);
                                        std::cout << "SHADER returned "<< arr[ idx ] << " for Z = " << iz << std::endl;
                                        return arr[ idx ]; 
                                    };

                                    target_->Set(row, col, shader(z));
                                    std::cout << "------------------"<<std::endl;

                                }
                            }
                        }
                    }
                }
            }
        }   
    private:
        bool can_run;
        int32_t width_, height_;

        std::vector<target_t> buff;
        float** zBuff;

        const TargetInterface<target_t>* target_;
    };
}