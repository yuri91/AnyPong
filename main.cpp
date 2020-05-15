#include <cheerp/client.h>
#include <cmath>
#include "three.h"

int WIDTH = 800;
int HEIGHT = 600;
double VIEW_ANGLE = 45;
double ASPECT = double(WIDTH) / HEIGHT;
double NEAR = 0.1;
double FAR = 10000;
double FIELD_WIDTH = 1200;
double FIELD_LENGTH = 3000;
double FIELD_HEIGHT = 5;
double FIELD_SIDE = 300;
double BALL_RADIUS = 20;
double PADDLE_WIDTH = 200;
double PADDLE_HEIGHT = 30;
double PADDLE_DEPTH = 20;
double BALL_VELOCITY_Y = 10;
double BALL_VELOCITY_X_FACTOR = 3;
double PADDLE_VELOCITY_X = 20;

namespace [[cheerp::genericjs]] client {

class ThreeState: public Object {
public:
	void set_renderer(WebGLRenderer* renderer);
	WebGLRenderer* get_renderer();
	void set_paddle(Mesh* paddle);
	Mesh* get_paddle();
	void set_ball(Mesh* ball);
	Mesh* get_ball();
	void set_scene(Scene* scene);
	Scene* get_scene();
	void set_camera(PerspectiveCamera* camera);
	PerspectiveCamera* get_camera();
	void set_text(HTMLDivElement* div);
	HTMLDivElement* get_text();
};

extern ThreeState threeState;

}

using namespace client;

Mesh* createPaddle()
{
	BoxGeometry* geo = new BoxGeometry(PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_DEPTH, 1, 1, 1);
	MeshLambertMaterialOptions* opts = new MeshLambertMaterialOptions(0xCCCCCC);
	MeshLambertMaterial* mat = new MeshLambertMaterial(opts);
	Mesh* paddle = new Mesh(geo, mat);
	paddle->get_rotation()->set_x(M_PI/4);
	paddle->get_position()->set(0, PADDLE_HEIGHT/2, FIELD_LENGTH / 2);
	return paddle;
}
Mesh* createField()
{
	BoxGeometry* geo = new BoxGeometry(FIELD_WIDTH, FIELD_HEIGHT, FIELD_LENGTH, 1, 1, 1);
	MeshLambertMaterial* mat = new MeshLambertMaterial(new MeshLambertMaterialOptions(0xFFFFFF));
	Mesh* field = new Mesh(geo, mat);
	field->get_position()->set(0, 0, 0);

	BoxGeometry* borderTopGeo = new BoxGeometry(FIELD_WIDTH, FIELD_SIDE, FIELD_HEIGHT, 1, 1, 1);
	Mesh* borderTop = new Mesh(borderTopGeo, mat);
	borderTop->get_position()->set(0, FIELD_SIDE/2, -FIELD_LENGTH/2);
	field->add(borderTop);

	BoxGeometry* borderLeftGeo = new BoxGeometry(FIELD_HEIGHT, FIELD_SIDE, FIELD_LENGTH, 1, 1, 1);
	Mesh* borderLeft = new Mesh(borderLeftGeo, mat);
	borderLeft->get_position()->set(-FIELD_WIDTH/2, FIELD_SIDE/2, 0);
	field->add(borderLeft);

	BoxGeometry* borderRightGeo = new BoxGeometry(FIELD_HEIGHT, FIELD_SIDE, FIELD_LENGTH, 1, 1, 1);
	Mesh* borderRight = new Mesh(borderRightGeo, mat);
	borderRight->get_position()->set(FIELD_WIDTH/2, FIELD_SIDE/2, 0);
	field->add(borderRight);

	return field;
}

Mesh* createBall()
{
	SphereGeometry* geo = new SphereGeometry(BALL_RADIUS, 16, 16);
	MeshLambertMaterialOptions* opts = new MeshLambertMaterialOptions(0xCC0000);
	MeshLambertMaterial* mat = new MeshLambertMaterial(opts);
	Mesh* ball = new Mesh(geo, mat);

	return ball;
}


void initGraphics()
{
	HTMLCanvasElement* canvas = (HTMLCanvasElement*)document.getElementById("canvas");
	WebGLRenderer* renderer = new WebGLRenderer(new WebGLRendererOptions(canvas));
	renderer->setSize(WIDTH, HEIGHT);
	renderer->setClearColor(0x80C6E5, 1);

	Scene* scene = new Scene();
	PerspectiveCamera* camera = new PerspectiveCamera(VIEW_ANGLE, ASPECT, NEAR, FAR);
	camera->get_position()->set(0, 200, FIELD_LENGTH / 2 + 700);

	Mesh* field = createField();
	scene->add(field);

	Mesh* paddle = createPaddle();
	scene->add(paddle);

	Mesh* ball = createBall();
	scene->add(ball);

	camera->lookAt(ball->get_position());

	HemisphereLight* hlight = new HemisphereLight(0xFFFFBB, 0x080820);
	scene->add(hlight);
	
	DirectionalLight* dlight = new DirectionalLight(0xFFFFFF, 0.1);
	dlight->get_position()->set(0, 5000, FIELD_LENGTH);
	dlight->set_castShadow(true);
	scene->add(dlight);

	HTMLDivElement* text = (HTMLDivElement*) document.createElement("div");
	text->set_id("info");
	document.get_body()->appendChild(text);

	threeState.set_renderer(renderer);
	threeState.set_ball(ball);
	threeState.set_paddle(paddle);
	threeState.set_scene(scene);
	threeState.set_camera(camera);
	threeState.set_text(text);

	renderer->render(scene, camera);
}

void webMain()
{
	initGraphics();
}

struct Paddle
{
	double x{0};
	double y{FIELD_LENGTH/2-PADDLE_DEPTH};
	double width{PADDLE_WIDTH};
	double height{PADDLE_HEIGHT};
	void sync() const
	{
		threeState.get_paddle()->get_position()->set(x, PADDLE_HEIGHT/2, y);
		threeState.get_camera()->get_position()->set_x(x);
	}
	void moveLeft()
	{
		x -= PADDLE_VELOCITY_X;
	}
	void moveRight()
	{
		x += PADDLE_VELOCITY_X;
	}
};

struct Ball
{
	double x{0};
	double y{0};
	double vx{0};
	double vy{BALL_VELOCITY_Y};
	void update()
	{
		x += vx;
		y += vy;
	}
	bool collide(Paddle& paddle)
	{
		double bottom = FIELD_LENGTH / 2;
		double top = - FIELD_LENGTH / 2;
		double left = - FIELD_WIDTH / 2;
		double right = FIELD_WIDTH / 2;
		if(y >= bottom)
		{
			return true;
		}
		if(x <= left || x >= right)
		{
			vx = -vx;
			return false;
		}
		if(y <= top)
		{
			vy = -vy;
			return false;
		}
		double paddleTop = paddle.y - PADDLE_DEPTH / 2;
		double paddleBottom = paddle.y + PADDLE_DEPTH / 2;
		double paddleLeft = paddle.x - PADDLE_WIDTH / 2;
		double paddleRight = paddle.x + PADDLE_WIDTH / 2;
		if (y <= paddleBottom && y >= paddleTop && x <= paddleRight && x >= paddleLeft)
		{
			vy = -vy;
			vx = (x - paddle.x) / BALL_VELOCITY_X_FACTOR;
			return false;
		}
		return false;
	}
	void sync()
	{
		const double bounces = 4;
		const double alpha = 1.0/bounces;
		double yrel = (FIELD_LENGTH/2-y)/FIELD_LENGTH;
		double n = 2*fmod(yrel, alpha)-alpha;
		double fraction = fmod(alpha - n*n/alpha, alpha)/alpha;
		double z = FIELD_SIDE*fraction;
		threeState.get_ball()->get_position()->set(x, z+BALL_RADIUS/2+FIELD_HEIGHT/2, y);
	}
};

Paddle paddle;
Ball ball;

[[cheerp::jsexport]]
extern "C" void keyDownHandler(KeyboardEvent* e)
{
	if(e->get_keyCode() == 37)
		paddle.moveLeft();
	else if(e->get_keyCode() == 39)
		paddle.moveRight();
}

[[cheerp::jsexport]]
extern "C" void mainLoop()
{
	ball.update();
	bool hasLost = ball.collide(paddle);
	if(hasLost)
	{
		threeState.get_text()->set_textContent("you lost");
		return;
	}
	paddle.sync();
	ball.sync();
	threeState.get_renderer()->render(threeState.get_scene(), threeState.get_camera());
}
