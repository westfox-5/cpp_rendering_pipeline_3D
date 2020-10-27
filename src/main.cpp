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
        std::cout << " ************ " << std::endl;
        std::cout << "Setting in target: [" << row << ", " << col << "]  ( val: " << val << ", SW: " << ScreenWidth() << ", SH: " << ScreenHeight() << ") : " << std::endl;
        if (row >= 0 && row < ScreenHeight() && col >= 0 && col < ScreenWidth())
        {
            std::cout << "Current val: " << buff[row][col] << std::endl;
            buff[row][col] = val;
        }
        std::cout << " ************ " << std::endl;
    }

    char Get(int row, int col) const
    {
        if (buff[row][col] != '.')
            std::cout << "Getting from target at [" << row << ", " << col << "] : " << buff[row][col] << std::endl;
        return buff[row][col];
    }

    void Print() const
    {
        std::ofstream myfile;

        //if (!std::remove("../test.txt")) return;
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

public:
    MyEngine(const MyTarget *target) : Engine(target) {}

public:
    bool OnCreate() final
    {
        rpl::Math::Vector3D v1 = {1, -1, 1.5};
        rpl::Math::Vector3D v2 = {1,  1, 1.1};
        rpl::Math::Vector3D v3 = {-1, 1, 1.5};
        rpl::Math::Vector3D v4 = {-1,-1, 1.9};

        mesh.triangles.push_back({{v1, v2, v3}});
        mesh.triangles.push_back({{v1, v3, v4}});

        std::cout << "------------------" << std::endl;
        std::cout << "INITIAL TRIANGLES: " << std::endl;
        for (auto &triangle : mesh.triangles)
        {
            for (auto &p : triangle.points)
                std::cout << p << std::endl;
            std::cout << std::endl;
        }
        std::cout << "------------------" << std::endl
                  << std::endl;
        
        // projection matrix
        //rpl::Math::Matrix4D projectionMatrix = rpl::Math::Perspective(1.0f, -1.0f, 1.0f, -1.0f, .1f, 2.f);
        rpl::Math::Matrix4D projectionMatrix = rpl::Math::Perspective(-1.0f, 1.0f, -1.0f, 1.0f, 1.f, 2.f);

        rpl::Math::Matrix4D scaling = rpl::Math::Scale({80.f, 80.f, .8f});

        for (auto &triangle : mesh.triangles)
        {
            rpl::Math::Vector3D transformedP[3];
            rpl::Math::Vector3D projectedP[3];
             

            // 3D -> 2D projection each vector
            for (int i = 0; i < 3; ++i)
            {
                rpl::Math::MultMatrixVector3(triangle.points[i], transformedP[i], scaling);
                rpl::Math::MultMatrixVector3(transformedP[i], projectedP[i], projectionMatrix);

                // Offset into the screen
                projectedP[i].x +=  (float)ScreenHeight() / 2;
                projectedP[i].y +=  (float)ScreenWidth() / 2;
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
    const MyTarget myTarget(200, 72);
    MyEngine myEngine(&myTarget);
    myEngine.Run();

    myTarget.Print();
    return 0;
}