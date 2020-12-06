#ifndef RASTERIZATION_H
#define RASTERIZATION_H
#include "mesh.h"

#include<cmath>
#include<vector>
#include<array>

namespace rpl {

	template <class Target_t>
	class Engine {
	public:
    	Engine() {}

		~Engine() { }

    	void set_target(int w, int h, std::vector<Target_t> t) {
        	width=w;
        	height=h;
			screen=t;
        	screen.shrink_to_fit();
        	z_buffer.clear();
        	z_buffer.resize(w*h*3,1.0e8f);
    	}

    	void set_perspective_projection(float left, float right, float top, float bottom, float near, float far) {
        	const float w=right-left;
        	const float h=bottom-top;
        	const float d=far-near;
	
        	//row-major
        	projection_matrix[0] = 2.0f*near/w;
        	projection_matrix[1] = 0;
        	projection_matrix[2] = -(right+left)/w;
        	projection_matrix[3] = 0;
        	projection_matrix[4*1+0] = 0;
        	projection_matrix[4*1+1] = 2.0f*near/h;
        	projection_matrix[4*1+2] = -(bottom+top)/h;
        	projection_matrix[4*1+3] = 0;
        	projection_matrix[4*2+0] = 0;
        	projection_matrix[4*2+1] = 0;
        	projection_matrix[4*2+2] = (far+near)/d;
        	projection_matrix[4*2+3] = -2.0f*far*near/d;
        	projection_matrix[4*3+0] = 0;
        	projection_matrix[4*3+1] = 0;
        	projection_matrix[4*3+2] = 1;
        	projection_matrix[4*3+3] = 0;
    	}
	
    	void set_orthographic_projection(float left, float right, float top, float bottom, float near, float far) {
        	const float w=right-left;
        	const float h=bottom-top;
        	const float d=far-near;
	
        	//row-major
        	projection_matrix[0] = 2.0f/w;
        	projection_matrix[1] = 0;
        	projection_matrix[2] = 0;
        	projection_matrix[3] = -(right+left)/w;
        	projection_matrix[4*1+0] = 0;
        	projection_matrix[4*1+1] = 2.0f/h;
        	projection_matrix[4*1+2] = 0;
        	projection_matrix[4*1+3] = -(bottom+top)/h;
        	projection_matrix[4*2+0] = 0;
        	projection_matrix[4*2+1] = 0;
        	projection_matrix[4*2+2] = 2.0f/d;
        	projection_matrix[4*2+3] = -(far+near)/d;
        	projection_matrix[4*3+0] = 0;
        	projection_matrix[4*3+1] = 0;
        	projection_matrix[4*3+2] = 0;
        	projection_matrix[4*3+3] = 1;
    	}

		void render_mesh(const Mesh& mesh) {

			std::vector<Triangle> tris = mesh.get_triangles();

			// render all triangles of the mesh
			for (Triangle &t: tris) {
				// needs to pass mesh becaus it has the shader
				render_triangle(t, mesh);
			}

		}

		void save_to_file(const char* filename) {
			Texture::write(filename, width, height, 3, screen);
		}

    	std::array<float,16> projection_matrix;
	    
	private:

    	void render_triangle(const Triangle& T, const Mesh& M) {
        	Vertex v1=T.points[0];
        	Vertex v2=T.points[1];
        	Vertex v3=T.points[2];

        	//project view coordinates to ndc;
        	std::array<float,3> ndc1;
        	std::array<float,3> ndc2;
        	std::array<float,3> ndc3;
        	project(v1,ndc1);
        	project(v2,ndc2);
        	project(v3,ndc3);

	
        	// at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice
        	if(ndc1[1] > ndc2[1]) {
            	std::swap(v1,v2);
            	std::swap(ndc1,ndc2);
        	}
        	if(ndc1[1] > ndc3[1]) {
            	std::swap(v1,v3);
            	std::swap(ndc1,ndc3);
        	}
        	if(ndc2[1] > ndc3[1]) {
            	std::swap(v2,v3);
            	std::swap(ndc2,ndc3);
        	}	
	
        	//pre-divide vertex attributes by depth
        	perspective_correct(v1);
        	perspective_correct(v2);
        	perspective_correct(v3);
	
	
	
        	// convert normalized device coordinates into pixel coordinates
        	const float x1f=ndc2idxf(ndc1[0],width);
        	const int x1=static_cast<int>(x1f+0.5f);
        	const float y1f=ndc2idxf(ndc1[1],height);
        	const int y1=static_cast<int>(y1f);
        	const float x2f=ndc2idxf(ndc2[0],width);
        	const int x2=static_cast<int>(x2f);
        	const float y2f=ndc2idxf(ndc2[1],height);
        	const int y2=static_cast<int>(y2f+0.5f);
        	const float x3f=ndc2idxf(ndc3[0],width);
        	const int x3=static_cast<int>(x3f+0.5f);
        	const float y3f=ndc2idxf(ndc3[1],height);
        	const int y3=static_cast<int>(y3f);
	
        	const float idy12 = 1.0f/(y2f-y1f);
        	const float idy13 = 1.0f/(y3f-y1f);
        	const float idy23 = 1.0f/(y3f-y2f);
	
        	const float m12=(x2f-x1f)*idy12;
        	const float m13=(x3f-x1f)*idy13;
        	const float m23=(x3f-x2f)*idy23;
	
        	const float q12=(x1f*y2f - x2f*y1f)*idy12;
        	const float q13=(x1f*y3f - x3f*y1f)*idy13;
        	const float q23=(x2f*y3f - x3f*y2f)*idy23;
	
	
	
        	bool horizontal12=std::abs(m12)>1.0f;
        	bool horizontal13=std::abs(m13)>1.0f;
        	bool horizontal23=std::abs(m23)>1.0f;
	
	
	
        	if (m13>m12) { // v2 is on the left of the line v1-v3
                	int y=y1;
                	float w1f = (y2f-y)*idy12;
                	float w1l = (y3f-y)*idy13;
                	float xf = m12*y+q12;
                	float xl = m13*y+q13;
                	while (y!=y2){
                    	const int first = horizontal12?static_cast<int>(xf+0.5*m12):static_cast<int>(xf);
                    	const int last = horizontal13?static_cast<int>(xl+0.5*m13)+1:static_cast<int>(xl)+1;
	
                    	const float step = 1.0f/(xl-xf);
                    	const float w0 = 1.0f + (xf-first)*step;
	
                    	render_scanline(y,first,last,interpolate(v1,v2,w1f),interpolate(v1,v3,w1l),
                                    	interpolate(ndc1[2],ndc2[2],w1f),interpolate(ndc1[2],ndc3[2],w1l), w0, step, M);
                    	++y;
                    	w1f -= idy12;
                    	w1l -= idy13;
                    	xf += m12;
                    	xl += m13;
                	}
                	if (y2==y3) {
                    	const float step = 1.0f/(xl-xf);
                    	const float w0 = 1.0f + (xf-x2)*step;
                    	render_scanline(y,x2,x3,v2,v3,ndc2[2],ndc3[2],w0,step, M);
                    	return;
                	} else {
                    	if (std::abs(m12)>std::abs(m23)) xf=m23*y+q23;
	
                    	const float step = 1.0f/(xl-xf);
                    	const float w0 = 1.0f + (xf-x2)*step;
                    	const int last = horizontal13?static_cast<int>(xl+0.5*m13)+1:static_cast<int>(xl)+1;
	
                    	render_scanline(y,x2,last,v2,interpolate(v1,v3,w1l),ndc2[2],interpolate(ndc1[2],ndc3[2],w1l),
                            	w0,step, M);
                	}
                	++y;
                	w1l -= idy13;
                	float w2f = (y3f-y)*idy23;
                	xf = m23*y+q23;
                	xl += m13;
	
                	while (y!=y3){
                    	const int first = horizontal23?static_cast<int>(xf+0.5*m23):static_cast<int>(xf);
                    	const int last = horizontal13?static_cast<int>(xl+0.5*m13)+1:static_cast<int>(xl)+1;
	
                    	const float step = 1.0f/(xl-xf);
                    	const float w0 = 1.0f + (xf-first)*step;
	
                    	render_scanline(y,first,last,interpolate(v2,v3,w2f),interpolate(v1,v3,w1l),
                                    	interpolate(ndc2[2],ndc3[2],w2f),interpolate(ndc1[2],ndc3[2],w1l),
                                    	w0,step, M);
                    	++y;
                    	w1l -= idy13;
                    	w2f -= idy23;
                    	xf += m23;
                    	xl += m13;
                	}
	
                	const int first = horizontal23?static_cast<int>(xf+0.5*m23):static_cast<int>(xf);
                	const float step = 1.0f/(xl-xf);
                	const float w0 = 1.0f + (xf-first)*step;
                	render_scanline(y,first,x3,interpolate(v2,v3,w2f),v3,interpolate(ndc2[2],ndc3[2],w2f),ndc3[2],
                        	w0,step, M);
	
        	} else { // v2 is on the right of the line v1-v3
            	int y=y1;
            	float w1f = (y3f-y)*idy13;
            	float w1l = (y2f-y)*idy12;
            	float xf = m13*y+q13;
            	float xl = m12*y+q12;
            	while (y!=y2){
                	const int first = horizontal13?static_cast<int>(xf+0.5*m13):static_cast<int>(xf);
                	const int last = horizontal12?static_cast<int>(xl+0.5*m12)+1:static_cast<int>(xl)+1;
	
                	const float step = 1.0f/(xl-xf);
                	const float w0 = 1.0f + (xf-first)*step;
	
                	render_scanline(y,first,last,interpolate(v1,v3,w1f),interpolate(v1,v2,w1l),
                                	interpolate(ndc1[2],ndc3[2],w1f),interpolate(ndc1[2],ndc2[2],w1l),
                        	w0,step, M);
                	++y;
                	w1f -= idy13;
                	w1l -= idy12;
                	xf += m13;
                	xl += m12;
            	}
            	if (y2==y3) {
                	const float step = 1.0f/(xl-xf);
                	const float w0 = 1.0f + (xf-x3)*step;
	
                	render_scanline(y,x3,x2+1,v3,v2,ndc3[2],ndc2[2],w0,step, M);
                	return;
            	} else {
                	const int first = horizontal13?static_cast<int>(xf+0.5*m13):static_cast<int>(xf);
                	const float step = 1.0f/(x2+1-xf);
                	const float w0 = 1.0f + (xf-first)*step;
	
                	render_scanline(y,first,x2+1,interpolate(v1,v3,w1f),v2,interpolate(ndc1[2],ndc3[2],w1f),ndc2[2],w0,step, M);
            	}
            	++y;
            	w1f -= idy13;
            	float w2l = (y3f-y)*idy23;
            	xl = m23*y+q23;
            	xf += m13;
	
            	while (y!=y3){
                	const int first = horizontal13?static_cast<int>(xf+0.5*m13):static_cast<int>(xf);
                	const int last = horizontal23?static_cast<int>(xl+0.5*m23)+1:static_cast<int>(xl)+1;
	
                	const float step = 1.0f/(xl-xf);
                	const float w0 = 1.0f + (xf-first)*step;
	
                	render_scanline(y,first,last,interpolate(v1,v3,w1f),interpolate(v2,v3,w2l),
                                	interpolate(ndc1[2],ndc3[2],w1f),interpolate(ndc2[2],ndc3[2],w2l),
                                	w0,step, M);
                	++y;
                	w1f -= idy13;
                	w2l -= idy23;
                	xl += m23;
                	xf += m13;
            	}
            	const float step = 1.0f/(xl-xf);
            	const float w0 = 1.0f + (xf-x3)*step;
            	const int last = horizontal23?static_cast<int>(xl+0.5*m23)+1:static_cast<int>(xl)+1;
	
            	render_scanline(y,x3,last,v3,interpolate(v2,v3,w2l),ndc3[2],interpolate(ndc2[2],ndc3[2],w2l),w0,step, M);

			}
    	}
	
    	float ndc2idxf(float ndc, int range) { return (ndc+1.0f)*(range-1)/2.0f; }
	
    	void project(const Vertex &v, std::array<float,3> &ndc) {
        	const float w=v.p.x*projection_matrix[4*3+0] + v.p.y*projection_matrix[4*3+1] + v.p.z*projection_matrix[4*3+2] + projection_matrix[4*3+3];
        	ndc[0] = (v.p.x*projection_matrix[4*0+0] + v.p.y*projection_matrix[4*0+1] + v.p.z*projection_matrix[4*0+2] + projection_matrix[4*0+3])/w;
        	ndc[1] = (v.p.x*projection_matrix[4*1+0] + v.p.y*projection_matrix[4*1+1] + v.p.z*projection_matrix[4*1+2] + projection_matrix[4*1+3])/w;
        	ndc[2] = (v.p.x*projection_matrix[4*2+0] + v.p.y*projection_matrix[4*2+1] + v.p.z*projection_matrix[4*2+2] + projection_matrix[4*2+3])/w;
    	}
	
    	float interpolate(float v1, float v2, float w) {
        	return v1*w + v2*(1.0f-w);
    	}
		
    	Vertex interpolate(const Vertex& v1, const Vertex& v2, float w) {
        	const float w2 = (1.0f-w);
        	Vertex v;
        	v.p.x = (w*v1.p.x + w2*v2.p.x);
        	v.p.y = (w*v1.p.y + w2*v2.p.y);
        	v.p.z = (w*v1.p.z + w2*v2.p.z);
        	v.np.x = (w*v1.np.x + w2*v2.np.x);
        	v.np.y = (w*v1.np.y + w2*v2.np.y);
        	v.np.z = (w*v1.np.z + w2*v2.np.z);
        	v.t.x = (w*v1.t.x + w2*v2.t.x);
        	v.t.y = (w*v1.t.y + w2*v2.t.y);

        	return v;
    	}
    	void perspective_correct(Vertex& v) {
        	v.p.z = 1.0f/v.p.z;
        	v.p.x *= v.p.z;
        	v.p.y *= v.p.z;
        	v.np.x *= v.p.z;
        	v.np.y *= v.p.z;
        	v.np.z *= v.p.z;
        	v.t.x *= v.p.z;
        	v.t.y *= v.p.z;
    	}
	
    	void render_scanline(int y, int xl, int xr, const Vertex& vl, const Vertex& vr, float ndczl, float ndczr, float w, float step, const Mesh& mesh) {
        	constexpr float epsilon = 1.0e-8f;
        	if (y<0 || y>=height) return;
	
        	Vertex p;
        	int x=std::max(xl,0);
        	w += (xl-x)*step;
        	for (; x<std::min(width,xr+1); ++x) {
            	const float ndcz=interpolate(ndczl,ndczr,w);
            	if ((z_buffer[y*width+x]+epsilon)<ndcz) continue;
            	Vertex p=interpolate(vl,vr,w);
            	perspective_correct(p);

				screen[y*width+x] = mesh.sample_pixel(p);

            	z_buffer[y*width+x]=ndcz;
            	w -= step;
        	}
	
    	}
	
    	int width;
    	int height;
	
    	std::vector<Target_t> screen;
    	std::vector<float> z_buffer;
	};
	
}//pipeline3D


#endif //RASTERIZATION_H
