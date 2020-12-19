///////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Matrix2d.h"
#include "Matrix3d.h"
#include "MatrixFactory.h"
#include "Vector4d.h"
#include "ShaderProgram.h"
#include "GLStatics.h"
#include "Camera.h"
#include "util.h"
#include "SceneGraph.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "ErrorHandler.h"
#include "Texture.h"
#include "TextureInfo.h"
#include "Texture2D.h"
#include "Texture3D.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

/*	MODIFIED BY
*	Group: 11
*	Bernardo Pinto - 98734
*	Daniel Correia - 98745
*/

bool tcoords = true;
bool normals = true;

const char vertexShaderPath[] = "../Resources/GraniteVS.glsl";
const char fragmentShaderPath[] = "../Resources/GraniteFS_Normal.glsl";
const char fragmentShaderPath_Texture[] = "../Resources/GraniteFS_3D.glsl";

const string SLIDING_PUZZLE_SCENE_GRAPH = "SlidingPuzzle";
const string MAIN_SHADER = "main";
const string CUBE_MESH = "cube";
const string COLOR_UNIFORM = "Color";

Camera camera = Camera(Vector3d(0, 0, -10), Vector3d(0, 0, -1), Vector3d(0, 1, 0));

#define DEGREES_TO_RADIANS 0.01745329251994329547

/*#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	// params: source, type, severity, count, ids, enabled
}

#else // ERROR_CALLBACK

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch(error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif // ERROR_CALLBACK*/

///////////////////////////////////////////////////////////////////// PreDrawFunctions

void setCyanColor() {
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 1, 1, 0);
	}
}

void setRedColor() {
	//glUniform4f(shaders.getUniformColorId(), 1, 0, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 1, 0, 0, 0);
	}
}

void setGreenColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 1, 0, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 1, 0, 0);
	}
}

void setBlueColor() {
	//glUniform4f(shaders.getUniformColorId(), 0, 0, 1, 0);
	std::map<std::string, ShaderProgram::UniformInfo>::iterator it = ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.find(COLOR_UNIFORM);
	if (it != ShaderProgramManager::getInstance()->get(MAIN_SHADER)->uniforms.end()) {
		glUniform4f(it->second.index, 0, 0, 1, 0);
	}
}

///////////////////////////////////////////////////////////////////// SCENE


SceneNode* backboard;
SceneNode* frame;
SceneNode* pieces;

void createTextures() {

	Texture3D* texture_0 = new Texture3D();
	texture_0->createPerlinNoise(256, 5, 5, 5, 2, 2, 8);
	TextureManager::getInstance()->add("number_0", (Texture*)texture_0);

	Texture2D* texture_1 = new Texture2D();
	texture_1->load("../number_1.png");
	//texture_1->createPerlinNoise(256, 5, 5, 2, 2, 8);
	TextureManager::getInstance()->add("number_1", (Texture*)texture_1);

	Texture2D* texture_2 = new Texture2D();
	texture_2->load("../number_2.png");
	//texture_2->createPerlinNoise(128, 10, 10, 2, 2, 1);
	TextureManager::getInstance()->add("number_2", (Texture*)texture_2);

	Texture2D* texture_3 = new Texture2D();
	texture_3->load("../number_3.png");
	//texture_3->createPerlinNoise(256, 10, 10, 2, 2, 8);
	TextureManager::getInstance()->add("number_3", (Texture*)texture_3);
}

void createEnvironmentSceneGraph()
{
	Mesh* cubeMesh = MeshManager::getInstance()->get(CUBE_MESH);

	#pragma region backboard
	backboard = new SceneNode();
	backboard->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	backboard->setPreDrawFun(setBlueColor);
	backboard->setMesh(cubeMesh);
	backboard->setMatrix(
		MatrixFactory::scalingMatrix(Vector3d(11.0f,11.0f,1.0f))
	);


	TextureInfo* tinfo_0 = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("number_0"));
	backboard->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	backboard->addTexture(tinfo_0);

#pragma endregion

	#pragma region frame
	frame = new SceneNode();
	frame->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	//frame->setPreDrawFun(setRedColor);
	frame->setPreDrawFun(setRedColor);
	frame->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
	);

	SceneNode* frameUp = new SceneNode();
	frameUp->setParent(frame);
	frameUp->setPreDrawFun(setRedColor);
	frameUp->setMesh(cubeMesh);
	//Define shape and set Z
	//UP Frame
	frameUp->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

	TextureInfo* tinfo_3 = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("number_3"));
	//piece1->setShaderProgram(ShaderProgramManager::getInstance()->get(MAIN_SHADER));
	frameUp->addTexture(tinfo_3);

	//Define positions and rotations of other frame components
	//Down
	SceneNode* frameDown = new SceneNode();
	frameDown->setParent(frame);
	frameDown->setPreDrawFun(setRedColor);
	frameDown->setMesh(cubeMesh);
	frameDown->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -2.0f, 0.0f))
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

	//Right
	SceneNode* frameRight = new SceneNode();
	frameRight->setParent(frame);
	frameRight->setPreDrawFun(setRedColor);
	frameRight->setMesh(cubeMesh);
	frameRight->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);
	//Left
	SceneNode* frameLeft = new SceneNode();
	frameLeft->setParent(frame);
	frameLeft->setPreDrawFun(setRedColor);
	frameLeft->setMesh(cubeMesh);
	frameLeft->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-2.0f, 0.0f, 0.0f))
		* MatrixFactory::rotateZMatrix(90)
		* MatrixFactory::scalingMatrix(Vector3d(11.0f, 1.0f, 3.0f))
	);

#pragma endregion

	#pragma region pieces

	pieces = new SceneNode();
	pieces->setParent(SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->getRoot());
	//pieces->setPreDrawFun(setGreenColor);
	pieces->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.8f))
		* MatrixFactory::scalingMatrix(3.0f)
	);

	SceneNode* piece1 = new SceneNode();
	piece1->setParent(pieces);
	piece1->setPreDrawFun(setGreenColor);
	piece1->setMesh(cubeMesh);
	piece1->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f,0.4f,0.0f))
	);

	SceneNode* piece2 = new SceneNode();
	piece2->setParent(pieces);
	piece2->setPreDrawFun(setGreenColor);
	piece2->setMesh(cubeMesh);
	piece2->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.4f, 0.0f))
	);

	SceneNode* piece3 = new SceneNode();
	piece3->setParent(pieces);
	piece3->setPreDrawFun(setGreenColor);
	piece3->setMesh(cubeMesh);
	piece3->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.4f, 0.0f))
	);

	SceneNode* piece4 = new SceneNode();
	piece4->setParent(pieces);
	piece4->setPreDrawFun(setGreenColor);
	piece4->setMesh(cubeMesh);
	piece4->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, 0.0f, 0.0f))
	);

	SceneNode* piece5 = new SceneNode();
	piece5->setParent(pieces);
	piece5->setPreDrawFun(setGreenColor);
	piece5->setMesh(cubeMesh);
	piece5->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, 0.0f, 0.0f))
	);

	SceneNode* piece6 = new SceneNode();
	piece6->setParent(pieces);
	piece6->setPreDrawFun(setGreenColor);
	piece6->setMesh(cubeMesh);
	piece6->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.4f, 0.0f, 0.0f))
	);

	SceneNode* piece7 = new SceneNode();
	piece7->setParent(pieces);
	piece7->setPreDrawFun(setGreenColor);
	piece7->setMesh(cubeMesh);
	piece7->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(-0.4f, -0.4f, 0.0f))
	);

	SceneNode* piece8 = new SceneNode();
	piece8->setParent(pieces);
	piece8->setPreDrawFun(setGreenColor);
	piece8->setMesh(cubeMesh);
	piece8->setMatrix(
		MatrixFactory::translationMatrix(Vector3d(0.0f, -0.4f, 0.0f))
	);

	TextureInfo* tinfo_1 = new TextureInfo(GL_TEXTURE0, 0, "Texture_1", TextureManager::getInstance()->get("number_1"));
	TextureInfo* tinfo_2 = new TextureInfo(GL_TEXTURE0, 0, "NoiseTexture", TextureManager::getInstance()->get("number_0"));
	piece8->setShaderProgram(ShaderProgramManager::getInstance()->get("Granite"));
	piece8->addTexture(tinfo_1);
	piece8->addTexture(tinfo_2);



#pragma endregion
}


void createSceneGraph()
{
	Mesh* mesh = new Mesh();
	MeshManager::getInstance()->add(CUBE_MESH, mesh);

	string s = string("../objs/cube5.obj");

	//mesh->createMesh(s);
	//mesh->createMeshBufferObjects();
	mesh->init(s);


	SceneGraph* slidingPuzzleScenegraph = new SceneGraph();
	slidingPuzzleScenegraph->setCamera(&camera);
	SceneGraphManager::getInstance()->add(SLIDING_PUZZLE_SCENE_GRAPH, slidingPuzzleScenegraph);

	SceneNode* n = slidingPuzzleScenegraph->getRoot();
	n->setShaderProgram(ShaderProgramManager::getInstance()->get(MAIN_SHADER));


	createEnvironmentSceneGraph();

	slidingPuzzleScenegraph->init(*(ShaderProgramManager::getInstance()->get(MAIN_SHADER)));
}

void drawScene()
{
	SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->draw();

#ifndef ERROR_CALLBACK
	ErrorHandler::checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	ShaderProgramManager::getInstance()->get(MAIN_SHADER)->destroy();
	SceneGraphManager::getInstance()->get(SLIDING_PUZZLE_SCENE_GRAPH)->destroy();
	//TODO maybe "delete mesh"
	//mesh->destroyMeshBufferObjects();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}

	if (action == GLFW_PRESS || action == GLFW_RELEASE) {
		camera.updateCameraPos(key, action);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		camera.changeProjectionType();
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		camera.changeRotationType();
	}

}

int mouse_pressed = 0;

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	camera.look(xpos, ypos, mouse_pressed);

}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == 0) {
		mouse_pressed = action;
	}
}

///////////////////////////////////////////////////////////////////////// SETUP

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);

	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);

	ShaderProgram* shaders = new ShaderProgram();
	shaders->addUniform(COLOR_UNIFORM);
	shaders->addUniform("Texture_1");
	shaders->addUniform("NoiseTexture");
	shaders->init(vertexShaderPath, fragmentShaderPath, tcoords, normals);
	ShaderProgramManager::getInstance()->add(MAIN_SHADER, shaders);


	ShaderProgram* g_shaders = new ShaderProgram();
	g_shaders->addAttribute("inVertex", VERTICES);
	g_shaders->addAttribute("inTexcoords", TEXCOORDS);
	g_shaders->addAttribute("inNormal", NORMALS);
	g_shaders->addUniform(COLOR_UNIFORM);
	g_shaders->addUniform("ModelMatrix");
	g_shaders->addUniform("Texture_1");
	g_shaders->addUniform("NoiseTexture");
	g_shaders->addUniform("Levers");

	g_shaders->init(vertexShaderPath, fragmentShaderPath_Texture, tcoords, normals);
	ShaderProgramManager::getInstance()->add("Granite", g_shaders);

	createTextures();
	createSceneGraph();

	return win;

#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	drawScene();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();

	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//updates camera position
		camera.updateCamera();
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		ErrorHandler::checkOpenGLError("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;

	GLFWwindow* win = setup(gl_major, gl_minor,
		640, 640, "Team project", is_fullscreen, is_vsync);
	run(win);

	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END