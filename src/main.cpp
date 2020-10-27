#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include <fstream>

#include "rpl/engine.h"


class MyTarget : public rpl::TargetInterface<char> 
{
public:
    MyTarget(int w, int h): 
        width(w),
        height(h)
    {
        for(int i=0; i<h; i++) {
            char * subVector;
            for(int j=0; j<w; j++)
                subVector[j] = '.';
            buff[i] = subVector;
        }
    }

    ~MyTarget() {}

public:

    void Set(int row, int col, char val) 
    {
        std::cout << "Setting in target: ["<< row <<", "<<col<<"] ("<< val <<") : " <<  val <<std::endl;
        if (row >= 0 && row < height && col >=0 && col < width)
            buff[row][col] = val;
    }

    char Get(int row, int col) 
    {
        return buff[row][col];
    }

    void Print() 
    {        
        std::ofstream myfile;

        std::remove("../test.txt");
        myfile.open ("../test.txt");

        // frame
        myfile << '+';
        for (int c=0; c<width; ++c)
            myfile<< '-';
        myfile << '+' << '\n';
        //


        for (int r=0; r<height; ++r) {
            myfile << '|'; // frame
            for (int c=0; c<width; ++c) {
                myfile << Get(r,c) ;
            }
            myfile << '|'; // frame
            myfile << '\n';
        }


        // frame
        myfile << '+';
        for (int c=0; c<width; ++c) 
            myfile<< '-';
        myfile << '+'<< '\n';
        //

        myfile.flush();
    }

private:
    int width, height;
    char** buff;


};



class MyEngine : public rpl::Engine<char> 
{
    private:
        int i = 0;
        rpl::Mesh::Mesh mesh;
    public:
        MyEngine(int32_t width, int32_t height, MyTarget* target) : Engine(width, height, target) 
        {

        }
    
    public: 
        bool OnCreate() final {

            rpl::Math::Vector3D v1 = {1,-1,1.5};
            rpl::Math::Vector3D v2 = {1,1,1.1};
            rpl::Math::Vector3D v3 = {-1,1,1.5};
            rpl::Math::Vector3D v4 = {-1,-1,1.9};       
     
            mesh.triangles.push_back( {{v1, v2, v3}});
            mesh.triangles.push_back( {{v1, v3, v4}});

            // rpl::Math::Matrix4D worldToCamera = {
            //     0,0,0,0,
            //     0,0,0,0,
            //     0,0,0,0,
            //     0,-10,-20,0,
            // };

            // std::cout << "------------------"<<std::endl;
            // std::cout << "INITIAL TRIANGLES: " <<std::endl;
            // for( auto &triangle: mesh.triangles) {
            //     for (auto & p : triangle.points)
            //         std::cout<<p<<std::endl;
            //     std::cout << std::endl;
            // }
            // std::cout << "------------------"<<std::endl<<std::endl;


            // projection matrix
            //rpl::Math::Matrix4D projectionMatrix = rpl::Math::perspective_fov(90.f, 0.1f ,100.0f);
            rpl::Math::Matrix4D projectionMatrix = rpl::Math::perspective(1,-1,1,-1,1,2);
            rpl::Math::Matrix4D transformations = rpl::Math::identity();
                rpl::Math::MatrixMultiply(
                rpl::Math::translation({ (float)ScreenWidth()/2, (float)ScreenHeight()/2, 0.0f}),
                rpl::Math::scale({1.3f, 1.3f, 1.0f})
            );

//            rpl::Math::Matrix4D  transformInverse = rpl::Math::transpose(rpl::Math::invert(transformationMatrix));

            for( auto &triangle: mesh.triangles) {
                rpl::Math::Vector3D translatedP[3], scaledP[3], projectedP[3];

                // 3D -> 2D projection each vector
                for (int i=0; i<3; ++i) {
                    //rpl::Math::MultMatrixVector(triangle.points[i], toCameraPoints[i], worldToCamera);
                    rpl::Math::MultMatrixVector(triangle.points[i], translatedP[i], transformations ); 
                    // rpl::Math::MultMatrixVector(translatedP[i], scaledP[i], scaleMatrix ); 
                    // rpl::Math::MultMatrixVector(scaledP[i], projectedP[i], projectionMatrix);
                   // rpl::Math::MultMatrixVector(toCameraPoints[i], projectedPoints[i], projectionMatrix);

                    rpl::Math::MultMatrixVector(translatedP[i], projectedP[i], projectionMatrix);

                    // if (projectedPoints[i].w != 1) {
                    //     projectedPoints[i].x /= projectedPoints[i].w;
                    //     projectedPoints[i].y /= projectedPoints[i].w;
                    //     projectedPoints[i].z /= projectedPoints[i].w;
                    // }
                }


                // std::cout << "------------------"<<std::endl;
                // std::cout << "TRANSFORMED: " <<std::endl;
                // for (auto & p : transformedPoints)
                //     std::cout<<p<<std::endl;
                // std::cout << "------------------"<<std::endl<<std::endl;

                // std::cout << "------------------"<<std::endl;
                // std::cout << "PROJECTION: " <<std::endl;
                // for (auto & p : projectedP)
                //     std::cout<<p<<std::endl;
                // std::cout << "------------------"<<std::endl<<std::endl;

                DrawTriangle(projectedP);
            }

            return true;
        }

        void OnDestroy() final {
        }
};



int main(int, char**) {
    MyTarget myTarget(150, 50);
    MyEngine myEngine(150, 50, &myTarget);

    myEngine.Run();

    myTarget.Print();
    return 0;

}