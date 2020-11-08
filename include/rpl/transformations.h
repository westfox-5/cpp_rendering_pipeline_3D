#pragma once // include only one time at compile-time

#include "math.h"

using namespace rpl::Math;
namespace rpl::Transformations {

    static Matrix4D Translation(const rpl::Math::Vector3D &t) {
        Matrix4D m;
        m.m[0][0] = 1;
        m.m[0][3] = t.x;
        m.m[1][1] = 1;
        m.m[1][3] = t.y;
        m.m[2][2] = 1;
        m.m[2][3] = t.z;
        m.m[3][3] = 1;
        return m;
    }

    static Matrix4D Scaling(const rpl::Math::Vector3D &scale) {
        Matrix4D m;
        m.m[0][0] = scale.x;
        m.m[1][1] = scale.y;
        m.m[2][2] = scale.z;
        m.m[3][3] = 1;
        return m;
    }

    static rpl::Math::Matrix4D Projection(const float fov, const float z_near, const float z_far) {
        float scale = 1 / tan(fov * 0.5 * M_PI / 180);
        Matrix4D m;
        m.m[0][0] = scale;
        m.m[1][1] = scale;
        m.m[2][2] = -z_far / (z_far - z_near);
        m.m[2][3] = -z_far * z_near / (z_far - z_near);
        m.m[3][2] = -1;
        return m;
    }

    static rpl::Math::Matrix4D Orthographic(const float right, const float left, const float bottom, const float top, const float zNear, const float zFar) {
        Matrix4D m;
        m.m[0][0] = 2 / (right-left);
        m.m[0][3] =  -(right+left) / (right-left);
        m.m[1][1] = 2 / (bottom-top);
        m.m[1][3] = -(bottom+top) / (bottom-top);
        m.m[2][2] = 2 / (zFar - zNear);
        m.m[2][3] = -(zFar + zNear) / (zFar - zNear);
        m.m[3][3] = 1;
        return m;
    }

    static rpl::Math::Matrix4D Perspective(const float right, const float left, const float bottom, const float top, const float zNear, const float zFar) {
        Matrix4D m;
        // m.m[0][0] = (2 * zNear) / (right-left);
        // m.m[0][2] =  (right+left) / (right-left);
        // m.m[1][1] = (2 * zNear) / (top-bottom);
        // m.m[1][2] = (top+bottom) / (top-bottom);
        // m.m[2][2] = -(zFar + zNear) / (zFar-zNear);
        // m.m[2][3] = (-2 * zNear * zFar ) / (zFar - zNear);
        // m.m[3][2] = -1;

        m.m[0][0] = (2 * zNear) / (right-left);
        m.m[0][2] = -(right+left) / (right-left);
        m.m[1][1] = (2 * zNear) / (bottom-top);
        m.m[1][2] = -(bottom+top) / (bottom-top);
        m.m[2][2] = (zFar + zNear) / (zFar-zNear);
        m.m[2][3] = (-2 * zNear * zFar ) / (zFar - zNear);
        m.m[3][2] = 1;
        return m;
    }
}