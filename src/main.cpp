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
 * 
 */


class MyTarget : public rpl::Target<char>
{
public:
    MyTarget(int w, int h) : Target(w, h) {}
    ~MyTarget() {}

public:
    void Set(int row, int col, char val) const
    {
        //std::cout << "Setting in target: [" << row << ", " << col << "]  : " << val << std::endl;
        if (row >= 0 && row < ScreenHeight() && col >= 0 && col < ScreenWidth())
        {
            //std::cout << "Current val: " << buff[row][col] << std::endl;
            buff[row][col] = val;
        }

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
    MyEngine(const MyTarget *target, std::function<char(float)> shader) : Engine(target, shader) {}

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

       // ApplyProjection     (rpl::Transformations::Perspective(1, -1, 1, -1, .1, 2) );
        ApplyTransformation (rpl::Transformations::Scaling({ 150, 50, 1}) );

        DrawMesh(mesh);

        return true;
    }

    void OnDestroy() final { }
};


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


int main(int, char **)
{
    const MyTarget myTarget(150, 50);

    auto ShaderFunc = [&](float z)
    {
        int i = std::abs((int)std::round(z) % 10);
        //std::cout<< z << " - " << i << " - " << std::to_string(i) << std::endl;
        return std::to_string(i)[0];
    };

    MyEngine myEngine(&myTarget, ShaderFunc);

    myEngine.Run();

    myTarget.Print();

    return 0;
}
