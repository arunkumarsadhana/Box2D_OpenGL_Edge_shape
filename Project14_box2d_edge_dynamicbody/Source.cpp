#include <Box2D/Box2D.h>
#include <glfw/glfw3.h>
#include <math.h>
#include <iostream>
using namespace std;
#define M_PI 3.14159265358979323846
GLFWwindow* window;
const int WIDTH=840;
const int HEIGHT=680;
const float WORLD_TO_BOX=0.01f;
const float BOX_TO_WORLD=100.0f;
b2World* world;
b2Body* body;
b2Vec2 points[4];
static void mouse_callback(GLFWwindow* window,int button,int action,int mods);
static float ConvertToBox(float x){
	return x*WORLD_TO_BOX;
}
static float ConvertToWorld(float x){
	return x*BOX_TO_WORLD;
}
void MakeEdgeShape(b2Body* body,float len,b2BodyType bodyType,float density,float restitution,float friction);
b2Body* addRect(int x,int y,int w,int h,bool dyns=true)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(ConvertToBox(x),ConvertToBox(y));
	if(dyns)
		bodyDef.type=b2_dynamicBody;	
	body=world->CreateBody(&bodyDef);
	b2PolygonShape shape;
	shape.SetAsBox(ConvertToBox(w/2),ConvertToBox(h/2));
	b2FixtureDef fixtureDef;
	fixtureDef.shape=&shape;
	fixtureDef.density=1.0;
	fixtureDef.friction=0.0;//step-1 try giving 0.9 they dont fly usually this is given
	fixtureDef.restitution=0.0;//now it is like a ball it can bounce
	body->CreateFixture(&fixtureDef);
	return body;
}


void drawSquare(b2Vec2 points[],b2Vec2 center,float angle)
{
	glColor3f(1,1,1);
	glPushMatrix();
	glTranslatef(ConvertToWorld(center.x),ConvertToWorld(center.y),0);
	glRotatef(angle*180.0/M_PI,0,0,1);
	
	
		glBegin(GL_QUADS);
		for(int i=0;i<4;i++)
			glVertex2f(ConvertToWorld(points[i].x),ConvertToWorld(points[i].y));
		glEnd();
	

	glPopMatrix();
}


b2Body* CreateEdgeBody(b2BodyType bodyType,float v1x,float v1y,float v2x,float v2y)
{
	b2BodyDef bodyDef;
	bodyDef.type=bodyType;
	float posX=(v1x + v2x)/2.0f;
	float posY=(v1y + v2y)/2.0f;
	float len=(float)sqrt((v1x-v2x)*(v1x-v2x)+(v1y-v2y)*(v1y-v2y));
	float bx=ConvertToBox(posX);
	float by=ConvertToBox(posY);
	bodyDef.position.Set(bx,by);
	bodyDef.angle=0;
	body=world->CreateBody(&bodyDef);
	MakeEdgeShape(body,len,bodyType,1,0,1);	
	body->SetTransform(b2Vec2(bx,by),atan2(v2y-v1y,v2x-v1x));
	return body;
}
void MakeEdgeShape(b2Body* body,float len,b2BodyType bodyType,float density,float restitution,float friction){
	b2FixtureDef fixtureDef;
	fixtureDef.density=density;
	fixtureDef.restitution=restitution;
	fixtureDef.friction=friction;
	b2EdgeShape es;
	float boxLen=ConvertToBox(len);	
	es.Set(b2Vec2(-boxLen/2.0f,0),b2Vec2(boxLen/2.0f,0));
	//glBegin(GL_LINE);
	//glVertex2f(-boxLen/2.0f,boxLen/2.0f);
	//glEnd();
	fixtureDef.shape=&es;
	body->CreateFixture(&fixtureDef);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();	
	//edge = CreateEdgeBody(b2_staticBody,100,290,260,280);
	
	b2Body* tmp=world->GetBodyList();

	
	
	while(tmp){
			if((tmp->GetFixtureList()->GetShape()->GetType())==1){
				glBegin(GL_LINES);
	glVertex3f(100.0f, 290.0f, 0.0f); // origin of the FIRST line
	glVertex3f(260.0f, 280.0f, 0.0f); // ending point of the FIRST line
	glVertex3f(65.0,255.0,0.0);
	glVertex3f(260.0,215.0,0.0);
	glEnd( );
			}
				else{
		for(int i=0;i<4;i++)
			points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
			
		drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
				
			}
		tmp=tmp->GetNext();
	}
	//edge = CreateEdgeBody(b2_staticBody,330,280,180,250);
	//edge = CreateEdgeBody(b2_staticBody,65,255,260,215);
	//edge =	CreateEdgeBody(b2_staticBody,400,240,170,170);
	//edge =	CreateEdgeBody(b2_staticBody,5,190,290,95);
	//edge =	CreateEdgeBody(b2_staticBody,290,95,290,70);
	//edge =	CreateEdgeBody(b2_staticBody,315,95,315,70);
	//edge =	CreateEdgeBody(b2_staticBody,290,70,315,70);	
}
void init()
{
	b2Body* edge;	
	glMatrixMode(GL_PROJECTION);	
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1,0,0,1);
	world=new b2World(b2Vec2(0.0,9.8));	
	edge = CreateEdgeBody(b2_staticBody,100,290,260,280);
	edge = CreateEdgeBody(b2_staticBody,65,255,260,215);
}
int main(int argc,char **argv)
{
	glfwInit();
	window=glfwCreateWindow(WIDTH,HEIGHT,"HELLO WORLD",NULL,NULL);
	glfwMakeContextCurrent(window);
	glViewport(0,0,WIDTH,HEIGHT);	
	glfwSetMouseButtonCallback(window,mouse_callback);
	init();	
	do{	
		display();
		world->Step(1.0/30.0,8,3);
		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwSwapInterval(5);
	}while(!glfwWindowShouldClose(window));
	glfwTerminate();
	return 0;
}

static void mouse_callback(GLFWwindow* window,int button,int action,int mods)
{
	
	if(button==GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS)
	{
		double x;
		double y;
		glfwGetCursorPos(window,&x,&y);
		addRect(x,y,20,20,true);
	}
	
}