#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>


#include "rpl/engine.h"


class MyTarget : public rpl::TargetInterface<char>
{
public:
    MyTarget(int w, int h) : TargetInterface(w, h)
    {
        buff = new char *[h];
        for (int r = 0; r < h; r++)
        {
            char *subVector = new char[w];
            for (int c = 0; c < w; c++)
                subVector[c] = '.';
            buff[r] = subVector;
        }
    }

    ~MyTarget() { delete[] buff; }

public:
    void Set(int row, int col, char val) const
    {
        //std::cout << " ************ " << std::endl;
        //std::cout << "Setting in target: [" << row << ", " << col << "]  ( val: " << val << ", SW: " << ScreenWidth() << ", SH: " << ScreenHeight() << ") : " << std::endl;
        if (row >= 0 && row < ScreenHeight() && col >= 0 && col < ScreenWidth())
        {
            //std::cout << "Current val: " << buff[row][col] << std::endl;
            buff[row][col] = val;
        }
        //std::cout << " ************ " << std::endl;
    }

    char Get(int row, int col) const
    {
        //if (buff[row][col] != '.')
        //std::cout << "Getting from target at [" << row << ", " << col << "] : " << buff[row][col] << std::endl;
        return buff[row][col];
    }

    void Print() const
    {
        std::ofstream myfile;

        std::cout << "ok" << std::endl;
        myfile.open("../test.txt");

        // frame
        myfile << '+';
        for (int c = 0; c < ScreenWidth(); ++c)
            myfile << '-';
        myfile << '+' << '\n';
        //

        for (int r = 0; r < ScreenHeight(); ++r)
        {
            myfile << '|'; // frame
            for (int c = 0; c < ScreenWidth(); ++c)
            {
                myfile << Get(r, c);
            }
            myfile << '|'; // frame
            myfile << '\n';
        }

        // frame
        myfile << '+';
        for (int c = 0; c < ScreenWidth(); ++c)
            myfile << '-';
        myfile << '+' << '\n';
        //

        myfile.flush();
    }
};


class MyEngine : public rpl::Engine<char>
{
private:
    int i = 0;
    rpl::Mesh::Mesh mesh;
    rpl::Math::Matrix4D mTransformations, mProjection;

public:
    MyEngine(const MyTarget *target, std::function<char(float)> shader) : Engine(target, shader) {}

public:
    bool OnCreate() final
    {
        rpl::Math::Vector3D v1 = {1, -1, 1.5};
        rpl::Math::Vector3D v2 = {1, 1, 1.1};
        rpl::Math::Vector3D v3 = {-1, 1, 1.5};
        rpl::Math::Vector3D v4 = {-1, -1, 1.9};

        mesh.triangles.push_back({{v1, v2, v3}});
        mesh.triangles.push_back({{v1, v3, v4}});

        // projection matrix
        
        float b, t, l ,r, near, far; 
        r = b = 1;
        t = l = -r;
        near = 1;
        far = 2;

        //mProjection = rpl::Transformations::Perspective(r, l, b, t, near, far);
        mProjection = rpl::Transformations::Projection(90, near, far);
        mTransformations = rpl::Transformations::Scaling({60, 30, 1});
            

        for (auto &triangle : mesh.triangles)
        {
            std::array<rpl::Math::Vector3D, 3> transformedP;
            std::array<rpl::Math::Vector3D, 3> projectedP;

            for (int i = 0; i < 3; ++i)
            {
                rpl::Math::MultPointMatrix(triangle.points[i], transformedP[i], mTransformations);
                rpl::Math::MultPointMatrix(transformedP[i], projectedP[i], mProjection);

                // Offset into the screen
                projectedP.at(i).x +=  (float)ScreenWidth() / 2;
                projectedP.at(i).y +=  (float)ScreenHeight() / 2;

                //projectedP.at(i).z = 1 / projectedP.at(i).z;
            }

            DrawTriangle(projectedP);
        }

        return true;
    }

    void OnDestroy() final
    {
    }
};

int main(int, char **)
{
    const MyTarget myTarget(150, 50);
    auto ShaderFunc = [&](float z)   
    {
        char arr[] = {'0','1','2','3','4','5','6','7','8','9'};
        int idx = abs( (int)std::floor(z) % 10);
        //std::cout << "SHADER returned "<< arr[ idx ] << " for Z = " << z_ << std::endl;
        return arr[ idx ]; 
    };
    
    MyEngine myEngine(&myTarget, ShaderFunc);

    myEngine.Run();

    myTarget.Print();

    return 0;
}