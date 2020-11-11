#include "rpl/engine.h"

/**
 * 
 * Polymorphic Rendering Pipeline - Example
 * 
 * Implement the target:
 *  In this case the target will write to a file named 'output.txt' in the root directory of the project.
 * 
 * Implement the engine:
 *  In the engine OnCreate() function, we can define the scene to render
 * 
 */

class MyTarget : public rpl::Target<char>
{
public:
    MyTarget(int w, int h) : Target(w, h) { InitBuffer(); }
    ~MyTarget() {}

public:
    void InitBuffer()
    {
        buff = new char *[ScreenHeight()];
        for (int32_t r = 0; r < ScreenHeight(); r++)
        {
            char *subVector = new char[ScreenWidth()];
            for (int32_t c = 0; c < ScreenWidth(); c++)
                subVector[c] = '.';
            buff[r] = subVector;
        }
    }

    void Set(int row, int col, char val)
    {
        if ( IsInBound(row, col) )
        {
#ifdef DEBUG_
            std::cout << "Setting in target: [" << row << ", " << col << "]  : " << val << std::endl;
#endif
            buff[row][col] = val;
        }

    }

    char Get(int row, int col)
    {
        return buff[row][col];
    }

    void Print()
    {
        std::ofstream myfile;

        myfile.open("../output.txt");

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
                myfile << Get(r, c); // VALUE
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


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


class MyEngine : public rpl::Engine<char>
{
public:
    MyEngine(MyTarget *target, std::function<char(float)> shader) : Engine(target, shader) {}

public:
    bool OnCreate() final
    {
        rpl::Math::Vector3D v1 = {1, -1, 1.5};
        rpl::Math::Vector3D v2 = {1, 1, 1.1};
        rpl::Math::Vector3D v3 = {-1, 1, 1.5};
        rpl::Math::Vector3D v4 = {-1, -1, 1.9};

        rpl::Mesh::Mesh mesh;
        mesh.triangles.push_back({{v1, v2, v3}});
        mesh.triangles.push_back({{v1, v3, v4}});

        ApplyProjection     (rpl::Transformations::Perspective(1, -1, 1, -1, .1, 2));
        ApplyTransformation (rpl::Transformations::Scaling({ 150, 50, 1}));

        DrawMesh(mesh);

        return true;
    }

    void OnDestroy() final { }
};


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


int main(int, char **)
{
    MyTarget myTarget(150, 50);

    auto ShaderFunc = [&](float z)
    {
        int i = std::abs((int)std::round(z) % 10);
#ifdef DEBUG_
        std::cout << z << " - " << i << " - " << std::to_string(i) << std::endl;
#endif
        return std::to_string(i)[0];
    };

    MyEngine myEngine( &myTarget, ShaderFunc);

    myEngine.Run();

    myTarget.Print();

    return 0;
}
