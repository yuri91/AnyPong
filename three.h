#include <cheerp/client.h>

namespace [[cheerp::genericjs]] client
{

class Vector2: public Object {
public:
	Vector2(double x, double y);
	double get_x();
	double get_y();
	void set_x(double x);
	void set_y(double y);
	void set(double x, double y);
};

class Vector3: public Object {
public:
	Vector3(double x, double y, double z);
	double get_x();
	double get_y();
	double get_z();
	void set_x(double x);
	void set_y(double y);
	void set_z(double z);
	void set(double x, double y, double z);
};

class Object3D: public Object {
public:
	Vector3* get_position();
	Vector3* get_rotation();
	void add(Object3D* child);
};

class PerspectiveCamera: public Object3D {
public:
	PerspectiveCamera(double view_angle, double aspect, double near, double far);
	void lookAt(Vector3* pos);
};

class Geometry: public Object {
};

class BoxGeometry: public Geometry {
public:
	BoxGeometry(double width, double height, double depth, int widthSegments, int heightSegments, int depthSegments);
};

class SphereGeometry: public Geometry {
public:
	SphereGeometry(double radius, int widthSegments, int heightSegments);
};


class MeshLambertMaterialOptions: public Object {
public:
	MeshLambertMaterialOptions(int color): Object()
	{
		this->set_color(color);
	}
	void set_color(int color);
};

class MeshLambertMaterial: public Object {
public:
	MeshLambertMaterial(MeshLambertMaterialOptions* options);
};

class Mesh: public Object3D {
public:
	Mesh(Geometry* geo, MeshLambertMaterial* mat);
};

class HemisphereLight: public Object3D {
public:
	HemisphereLight(int skyColor = 0xFFFFFF, int groundColor = 0xFFFFFF, double intensity = 1);
};

class DirectionalLight: public Object3D {
public:
	DirectionalLight(int color = 0xFFFFFF, double intensity = 1);
	void set_castShadow(bool shadow);
};

class Scene: public Object3D {
public:
	Scene();
};

class WebGLRendererOptions: public Object {
public:
	WebGLRendererOptions(HTMLCanvasElement* canvas): Object()
	{
		this->set_canvas(canvas);
	}
	void set_canvas(HTMLCanvasElement* canvas);
};

class WebGLRenderer: public Object {
public:
	WebGLRenderer();
	WebGLRenderer(WebGLRendererOptions* opts);
	void setSize(int w, int h);
	void setClearColor(unsigned color, double alpha);
	Element* get_domElement();
	void render(Scene* scene, PerspectiveCamera* camera);
};


}

