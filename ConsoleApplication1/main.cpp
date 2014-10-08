//Header Files
#include <iostream>
//header for SDL2 functionality
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>


//Global variables go here
//Pointer to our SDL Windows
SDL_Window*window;
//SDL GL Context
SDL_GLContext glcontext = NULL;
/*//Vertex data of triangle
float triangleData[] = { 0.0f, 1.0f, 0.0f,	//Top
			- 1.0f, -1.0f, 0.0f,	//BottomLeft
			1.0f, -1.0f, 0.0f };	//Bottom Right
*/
//Vertex Data of triangles for cube
Vertex triangleData[] = {
	//Front
		//Bottom Left Triangle of front
		{-0.5f,0.5f,0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top left
		
		{-0.5f,-0.5f,0.5f,
		1.0f,1.0f,0.0f,1.0f},	//Bottom Left
		
		{0.5f,-0.5f,0.5f,
		0.0f,1.0f,1.0f,1.0f},	//Bottom Right
		
		//Top Right Triangle of front
		{0.5f,0.5f,0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top Right
		
		{-0.5f,0.5f,0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top Left
		
		{0.5f,-0.5f,0.5f,
		0.0f,1.0f,1.0f,1.0f},	//Bottom Right
	
	//Back
		//Bottom Left Triangle of Back
		{-0.5f,0.5f,-0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top Left
		
		{-0.5f,-0.5f,-0.5f,
		1.0f,1.0f,0.0f,1.0f},	//Bottom Left
		
		{0.5f,-0.5f,-0.5f
		0.0f,1.0f,1.0f,1.0f},	//Bottom Right
		
		//Top Right Triangle of Back
		{0.5f,0.5f,-0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top Right
		
		{-0.5f,0.5f,-0.5f,
		1.0f,0.0f,1.0f,1.0f},	//Top Left
		
		{0.5f,-0.5f,-0.5f,
		0.0f,1.0f,1.0f,1.0f},	//Bottom Right
		
};
//Triangle VBO variable
GLuint triangleVBO;

//Constants to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;


//Global functions
void InitWindow(int width, int height, bool fullscreen){
	//Create a window
	window = SDL_CreateWindow(
		"Lab 1",		//Window title
		SDL_WINDOWPOS_CENTERED,	//x position, centered
		SDL_WINDOWPOS_CENTERED, //y position, centered
		width,			//width, in pixels
		height,			//height, in pixels
		SDL_WINDOW_OPENGL	//flags
		);

}

//Used to cleanup once we exit
void CleanUp(){
	glDeleteBuffers(1, &triangleVBO);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//Function to initialise OpenGL
void initOpenGL(){
	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);
	
	//Something went wrong in creating the context, if it is still NULL
	if(!glcontext){
		std::cout<<"Error Creating OpenGL Context"<<SDL_GetError()<<
		std::endl;
	}
	
	//Smooth shading
	glShadeModel(GL_SMOOTH);
	
	//clear the background to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	//Clear the depth buffer to 1.0
	glClearDepth(1.0f);
	
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	//The depth test to use
	glDepthFunc(GL_LEQUAL);
	
	//Turn on best perspective correction
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	GLenum err = glewInit();
	if (GLEW_OK != err){
		/*Problem: glewInit failed, something is seriously wrong.*/
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	
}

//Function to st/reset viewport
void setViewport(int width, int height){
	//screen ratio
	GLfloat ratio;
	
	//make sure height is always above 0
	if(height == 0){
		height = 1;
	}
	
	//calculate screen ratio
	ratio = (GLfloat)width/(GLfloat)height;
	
	//Setup viewport
	glViewport(0,0,(GLsizei)width, (GLsizei)height);
	
	//Change to project matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//Calculate perspetive matrix, using glu library functions
	gluPerspective(45.0f,ratio,0.1f,100.0f);
	
	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);
	
	//Reset using the identity matrix
	glLoadIdentity();
	
}

//Function to draw
void render(){
	//Set the clear color(background
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	
	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active. Repeat here as a sanity check(may have changed since initialization)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//Establish its 3 coordinates per vertex with zero stride(space between elements) in 
	//array and contain floating point numbers
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	//Establish array contains vertices(not normals, colors, texture coords etc)
	glEnableClientState(GL_VERTEX_ARRAY);

	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);
	//Reset using the Identity Matrix
	glLoadIdentity();
	
	//Set up the 3D space from the 2D space
	//First 3 params set camera position, second 3 are center(look at point), last 3 are up axis
	gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0f,0.0,1.0,0.0);
	
	//translate
	glTranslatef(0.0f, 0.0f, -6.0f);
	//Actually draw the triangle, giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / (3 * sizeof(float)));

	////Second triangle
	//glLoadIdentity();
	//glTranslatef(1.0f, 0.0f, -5.0f);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / (3 * sizeof(float)));

	////Switch to ModelView
	//glMatrixMode(GL_MODELVIEW);
	////Reset using the identitiy matrix
	//glLoadIdentity();
	////Translate to -5.0f on z-axis
	//glTranslatef(0.0f,0.0f,-5.0f);
	////Begin drawing triangles
	//glBegin(GL_TRIANGLES);
	//	glColor3f(1.0f,0.0f,0.0f);//Color of the vertice
	//	glVertex3f(0.0f,1.0f,0.0f);//Top
	//	glVertex3f(-1.0f,-1.0f,0.0f);//Bottom Left
	//	glVertex3f(1.0f,-1.0f,0.0f);//BottomRight
	//glEnd();
	
	//require to swap the back and front buffer
	SDL_GL_SwapWindow(window);
}

//Function to update game state
void update(){
	
}

//Will create and fill a VBO with some vertex data
void initGeomoetry(){
	//Create buffer, specifies number of buffers to generate
	glGenBuffers(1, &triangleVBO);
	//make the new VBO, binds the buffer to a specified state
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//Copy vertex data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

}

//Main Method - Entry Point
int main(int argc, char*arg[]){
	//init everything - SDL, if it is nonzero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return -1;
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
	
	//Call our InitOpenGL Function
	initOpenGL();
	//Call to initGeometry
	initGeomoetry();

	//Set our viewport
	setViewport(WINDOW_WIDTH,WINDOW_HEIGHT);

	SDL_Event event;
	while (running){
		while (SDL_PollEvent(&event)){
			//Get event type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
				//set our boolean which controls the game loop to false
				running = false;
			}
		}
		
		update();
		render();

	}

	CleanUp();

	return 0;
}
