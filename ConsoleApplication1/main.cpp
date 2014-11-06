#include <iostream>
#include <GL/glew.h>
//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <OpenGL/glu.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#elif WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <gl/GLU.h>
#endif

#include <vector>

#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "../assets/";
const std::string SHADER_PATH = "shaders/";
const std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";
#elif __APPLE__
const std::string ASSET_PATH;
const std::string SHADER_PATH;
const std::string TEXTURE_PATH;
const std::string FONT_PATH;
#else
const std::string ASSET_PATH="/assets/";
const std::string SHADER_PATH="shaders/";
#endif

//Our headers
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"


//SDL Window
SDL_Window * window = NULL;
//SDL GL Context
SDL_GLContext glcontext = NULL;

//Window Width
const int WINDOW_WIDTH = 640;
//Window Height
const int WINDOW_HEIGHT = 480;

bool running = true;

std::vector<GameObject*> displayList;
GameObject * mainCamera;

Vertex triangleData[] = {
		{ vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Left
		{ vec3(-0.5f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },// Bottom Left
		{ vec3(0.5f, -0.5f, 0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Bottom Right
		{ vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Top Right
		
		
		//back
		{ vec3(-0.5f, 0.5f, -0.5f), vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },// Top Left
		{ vec3(-0.5f, -0.5f, -0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) },// Bottom Left
		{ vec3(0.5f, -0.5f, -0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f) }, //Bottom Right
		{ vec3(0.5f, 0.5f, -0.5f), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f)}// Top Right
};


GLuint indices[]={
    //front
    0,1,2,
    0,3,2,
    
    //left
    4,5,1,
    4,1,0,
    
    //right
    3,7,2,
    7,6,2,
    
    //bottom
    1,5,2,
    6,2,1,
    
    //top
    5,0,7,
    5,7,3,
    
    //back
    4,5,6,
    4,7,6
};

void CheckForErrors()
{
    GLenum error;
    do{
        error=glGetError();
    }while(error!=GL_NO_ERROR);
}

//Global functions
void InitWindow(int width, int height, bool fullscreen)
{
	//Create a window
	window = SDL_CreateWindow(
		"Lab 6",             // window title
		SDL_WINDOWPOS_CENTERED,     // x position, centered
		SDL_WINDOWPOS_CENTERED,     // y position, centered
		width,                        // width, in pixels
		height,                        // height, in pixels
		SDL_WINDOW_OPENGL           // flags
		);
}



void CleanUp()
{
    auto iter=displayList.begin();
	while(iter!=displayList.end())
    {
        (*iter)->destroy();
        if ((*iter))
        {
            delete (*iter);
            (*iter)=NULL;
            iter=displayList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    displayList.clear();
    
	// clean up, reverse order!!!
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}



//Function to initialise OpenGL
void initOpenGL()
{
    //Ask for version 3.2 of OpenGL
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
	//Create OpenGL Context
	glcontext = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
    
    //Smooth shading
    glShadeModel( GL_SMOOTH );
    
    //clear the background to black
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    
    //Clear the depth buffer
    glClearDepth( 1.0f );
    
    //Enable depth testing
    glEnable( GL_DEPTH_TEST );
    
    //The depth test to go
    glDepthFunc( GL_LEQUAL );
    
    //Turn on best perspective correction
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

//Function to set/reset viewport
void setViewport( int width, int height )
{
    
    //make sure height is always above 1
    if ( height == 0 ) {
        height = 1;
    }

    
    //Setup viewport
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
}

void Initialise()
{
    mainCamera=new GameObject();
    mainCamera->setName("MainCamera");
    
    Transform *t=new Transform();
    t->setPosition(0.0f,0.0f,10.0f);
    mainCamera->setTransform(t);
    
    Camera * c=new Camera();
    c->setAspectRatio((float)(WINDOW_WIDTH/WINDOW_HEIGHT));
    c->setFOV(45.0f);
    c->setNearClip(0.1f);
    c->setFarClip(1000.0f);
    
    mainCamera->setCamera(c);
    displayList.push_back(mainCamera);
    
    
    GameObject * cube=new GameObject();
    cube->setName("Cube");
    Transform *transform=new Transform();
    transform ->setPosition(0.0f,0.0f,0.0f);
    cube ->setTransform(transform);
    
    Material * material=new Material();
    std::string vsPath = ASSET_PATH + SHADER_PATH+"/simpleVS.glsl";
    std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
    material -> loadShader(vsPath,fsPath);
    cube->setMaterial(material);
    
    Mesh * mesh=new Mesh();
    cube->setMesh(mesh);
    displayList.push_back(cube);

    
    //alternative sytanx
    for(auto iter=displayList.begin();iter!=displayList.end();iter++)
    {
        (*iter)->init();
    }
    
    mesh->copyVertexData(8,sizeof(Vertex), (void**)triangleData);
    mesh->copyIndexData(36,sizeof(int), (void**)indices);
}


//Function to update the game state
void update()
{
    //alternative sytanx
    for(auto iter=displayList.begin();iter!=displayList.end();iter++)
    {
        (*iter)->update();
    }
}

//Function to render(aka draw)
void render()
{
    //old imediate mode!
    //Set the clear colour(background)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);
    //clear the colour and depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //alternative sytanx
    for(auto iter=displayList.begin();iter!=displayList.end();iter++)
    {
        (*iter)->render();
        
        Mesh * currentMesh=(*iter)->getMesh();
        Transform * currentTransform=(*iter)->getTransform();
        Material * currentMaterial=(*iter)->getMaterial();
        
        if (currentMesh && currentMaterial && currentTransform)
        {
            currentMaterial->bind();
            currentMesh->bind();
            
            GLint MVPLocation = currentMaterial->getUniformLocation("MVP");
            
            Camera * cam=mainCamera->getCamera();
            mat4 MVP=cam->getProjection()*cam->getView()*currentTransform->getModel();
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
            
            glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(),GL_UNSIGNED_INT,0);
        }
    }
    
    SDL_GL_SwapWindow(window);
}



//Main Method
int main(int argc, char * arg[])
{
    // Setup asset path, no real work required for Windows. Mac needs to load assets from a bundle
    // ----------------------------------------------------------------------------
    // http://stackoverflow.com/questions/516200/relative-paths-not-working-in-xcode-c
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    

    
    // init everyting - SDL, if it is nonzero we have a problem
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init " <<SDL_GetError()<< std::endl;
        
        return -1;
    }
    
	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	if (((returnInitFlags) & (imageInitFlags)) != imageInitFlags) {
		std::cout << "ERROR SDL_Image Init " << IMG_GetError() << std::endl;
		// handle error
	}

	if (TTF_Init() == -1) {
		std::cout << "TTF_Init: " << TTF_GetError();
	}
    
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);
    //Call our InitOpenGL Function
    initOpenGL();
    CheckForErrors();
    //Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

    Initialise();
    
    //Value to hold the event generated by SDL
    SDL_Event event;
    //Game Loop
	while (running)
    {
        //While we still have events in the queue
        while (SDL_PollEvent(&event)) {
            //Get event type
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                //set our boolean which controls the loop to false
                running = false;
            }
        }
		update();
        //render
		render();
       
        
    }
    

	CleanUp();
    
    return 0;
}
