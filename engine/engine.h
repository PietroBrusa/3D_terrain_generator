/**
 * @file		engine.h
 * @brief	Graphics engine main include file
 *
 * @author	Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/////////////
// #DEFINE //
/////////////

/** @brief Enables static linking for FreeGLUT. */
#define FREEGLUT_STATIC

// Quick define for the extension:
/** @brief OpenGL constant for setting the maximum degree of anisotropic filtering. */
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
/** @brief OpenGL constant for querying the maximum supported anisotropic filtering degree. */
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
// See https://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt

// Silly Microsoft GL.h stuck at version 1.1...:
#ifndef GL_CLAMP_TO_EDGE
/** @brief OpenGL constant for texture wrapping mode (clamping coordinates to the texture edge). */
#define GL_CLAMP_TO_EDGE 0x812F
#endif

//////////////
// #INCLUDE //
//////////////

// C/C++:
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>

// GLM:
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/////////////
// VERSION //
/////////////

// Generic info:
#ifdef _DEBUG
/** @brief Library credits and name for the debug build. */
#define LIB_NAME "My Graphics Engine v0.1a (debug)" ///< Library credits
#else
/** @brief Library credits and name for the release build. */
#define LIB_NAME "My Graphics Engine v0.1a" ///< Library credits
#endif
/** @brief Library version number. */
#define LIB_VERSION 10 ///< Library version (divide by 10)

// Export API:
#ifdef _WINDOWS
// Specifies i/o linkage (VC++ spec):
#ifdef ENGINE_EXPORTS
/** @brief Macro for exporting symbols from the DLL/shared library (Windows). */
#define ENG_API __declspec(dllexport)
#else
/** @brief Macro for importing symbols from the DLL/shared library (Windows). */
#define ENG_API __declspec(dllimport)
#endif

// Get rid of annoying warnings:
#pragma warning(disable : 4251)
#else // Under linux
/** @brief Empty macro for API visibility (Linux/other). */
#define ENG_API
#endif

///////////////
// NAMESPACE //
///////////////

/**
 * @brief The main namespace for the Graphics Engine.
 *
 * All core engine classes, utilities, and components reside in this namespace.
 */
namespace Eng
{

	//////////////
	// #INCLUDE //
	//////////////

#include "definitions.h"


#include "object.h"
#include "guiobjects.h"

#include "shader.h"

#include "node.h"
#include "texture.h"
#include "material.h"

// Scene objesct
#include "camera.h"
#include "mesh.h"
#include "light.h"
#include "list.h"

// Cameras
#include "perspetivecamera.h"
#include "orthocamera.h"

// Lights
#include "infinitelight.h"
#include "omnilight.h"
#include "spotlight.h"

// Ovoreader
#include "ovoreader.h"

	///////////////////////
	// MAIN ENGINE CLASS //
	///////////////////////

	/**
	 * @brief Base engine main class. This class is a singleton.
	 *
	 * It manages engine initialization, shutdown, the main loop,
	 * camera state, scene loading, and callback registration.
	 */
	class ENG_API Base final
	{

	public:
		// Const/dest:
		/**
		 * @brief Deleted copy constructor.
		 * @param Base const & prevents copying of the Base engine instance.
		 */
		Base(Base const &) = delete;

		/** @brief Destructor for the Base engine class. */
		~Base();

		// Operators:
		/**
		 * @brief Deleted assignment operator.
		 * @param Base const & prevents assignment of the Base engine instance.
		 */
		void operator=(Base const &) = delete;

		// Singleton:
		/**
		 * @brief Gets the single instance of the Base engine class.
		 * @return A reference to the Base engine instance.
		 */
		static Base &getInstance();

		// Init/free/start:
		/**
		 * @brief Initializes the graphics engine and the rendering window.
		 *
		 * This function sets up the OpenGL/FreeGLUT context and initializes internal engine structures.
		 * @param argc Pointer to the main argument count.
		 * @param argv Array of argument strings.
		 * @param winName The title of the rendering window.
		 * @param width The initial width of the rendering window (default: 1066).
		 * @param height The initial height of the rendering window (default: 600).
		 * @return \c true on successful initialization, \c false otherwise.
		 */
		bool init(int *argc, char *argv[], const char *winName, int width = 1066, int height = 600);
		
		/**
		 * @brief Shuts down and frees all resources used by the engine.
		 * @return \c true on successful cleanup, \c false otherwise.
		 */
		bool free();

		/**
		 * @brief Starts the main rendering loop of the engine.
		 *
		 * This is a blocking call that begins the FreeGLUT main loop.
		 * @param callback The user-defined function to be executed every frame before rendering (the scene update logic).
		 * @return \c true if the loop started successfully, \c false otherwise.
		 */
		bool start(void (*callback)(Node *root));


		/**
		* @brief Stop the main rendering loop of the engine.
		*/
		void stop();
		
		// Engine loader:
        /**
		 * @biref Give the current scene graph root node or create a new, then you can attach any node child.
		 * @return A pointer to the root Node of the loaded scene graph.
		 */
		Eng::Node* getSceneGraphInstance();

		/**
		 * @brief Loads a scene from a specified file path using an internal loader (e.g., OVOReader).
		 * @param path The file path to the scene definition.
		 * @return A pointer to the root Node of the loaded scene graph, or \c nullptr on failure.
		 */
		Eng::Node *loadScene(std::string path);

		// Camera management
		/**
		 * @brief Sets the currently active camera for rendering.
		 * @param camera A pointer to the Camera object to be used for the next frame.
		 */
		void setActiveCamera(Camera *camera);

		/**
		* @brief Gets the currently active camera used for rendering.
		* @return A pointer to the active Camera object, or \c nullptr if no camera is set.
		*/
		Camera* getActiveCamera() const;

		/**
		 * @brief Forces a camera to recalculate its viewport and projection matrix.
		 * @param camera A pointer to the Camera object to be updated.
		 * @param width The new width of the viewport.
		 * @param height The new height of the viewport.
		 */
		void fixCameraViewport(Camera *camera, int width, int height);

		// FPS:
		/**
		 * @brief Gets the current frame rate (Frames Per Second) of the engine.
		 * @return The current FPS as an integer.
		 */
		int getCurrentFPS();

		float getDeltaTime();

		bool getClickedNode(int x, int y, glm::vec3& coord);

		// Engine external callbacks:
		/**
		 * @brief Sets the callback function executed when the rendering window is resized.
		 * @param callback The function pointer: \c void \c (*)(int \c width, \c int \c height).
		 */
		void setOnResizeCallback(void (*callback)(int width, int height));

		/**
		 * @brief Sets the callback function executed when a special key (e.g., F1, arrow keys) is pressed.
		 * @param callback The function pointer: \c void \c (*)(int \c key, \c int \c mouseX, \c int \c mouseY).
		 */
		void setOnSpecialPressedCallback(void (*callback)(int key, int mouseX, int mouseY));

		/**
		 * @brief Sets the callback function executed when a special key (e.g., F1, arrow keys) is released.
		 * @param callback The function pointer: \c void \c (*)(int \c key, \c int \c mouseX, \c int \c mouseY).
		 */
		void setOnSpecialReleasedCallback(void (*callback)(int key, int mouseX, int mouseY));

		/**
		 * @brief Sets the callback function executed when a regular keyboard key is pressed.
		 * @param callback The function pointer: \c void \c (*)(unsigned \c char \c key, \c int \c mouseX, \c int \c mouseY).
		 */
		void setOnKeyboardPressedCallback(void (*callback)(unsigned char key, int mouseX, int mouseY));

		/**
		 * @brief Sets the callback function executed when a mouse button event occurs.
		 * @param callback The function pointer: \c void \c (*)(int \c buttonId, \c int \c buttonState, \c int \c mouseX, \c int \c mouseY).
		 */
		void setOnMouseCallback(void (*callback)(int buttonId, int buttonState, int mouseX, int mouseY));

		/**
		 * @brief Sets the callback function executed when a mouse motion event occurs (button press and mouse move).
		 * @param callback The function pointer: \c void \c (*)(int \c int \c x, \c int \c y).
		 */
		void setOnMouseMotionCallback(void (*callback)(int x, int y));

		/**
		 * @brief Sets the callback function executed when a mouse passive motion event occurs (mouse hover).
		 * @param callback The function pointer: \c void \c (*)(int \c int \c x, \c int \c y).
		 */
		void setOnPassiveMouseMotionCallback(void (*callback)(int x, int y));

		/**
		* @brief Sets the callback function executed when a mouse wheel event occurs.
		 * @param callback The function pointer: \c void \c (*)(int \c wheelId, \c int \c direction, \c int \c mouseX, \c int \c mouseY).
		*/
		void setOnMouseWheelCallback(void(*callback)(int wheelId, int direction, int x, int y));

		/**
		 * @brief Sets the callback function executed when the engine is idle (between frames).
		 * @param callback The function pointer: \c void \c (*)().
		 */
		void setOnIdleCallback(void (*callback)());

		/**
		 * @brief Sets the callback function executed when the rendering window is closed.
		 * @param callback The function pointer: \c void \c (*)().
		 */
		void setOnCloseCallback(void (*callback)());

		/**
		 * @brief Sets the callback function for drawing GUI text elements.
		 * @param callback The function pointer: \c void \c (*)(GUIObjects \c gui).
		 */
		void setOnTextDrawCallback(void (*callback)(GUIObjects gui));

		/**
		 * @brief Toggles the rendering mode between solid and wireframe.
		 * @param isWireFrame If \c true, switch to wireframe mode; otherwise, switch to solid mode.
		 */
		void changeWireFrame(bool isWireFrame);

		/**
		 * @brief Toggles the v-sync mode.
		 * @param isVsync If \c true, v-sync is enabled; otherwise, is disabled (fps fixed).
		 */
		void changeVsync(bool isVsync);

	private:
		// Reserved:
		/**
		 * @brief Forward declaration of an internal structure to hide implementation details (PIMPL idiom).
		 * @internal
		 */
		struct Reserved;

		/** @brief Unique pointer to the internal reserved structure holding engine state and variables. */
		std::unique_ptr<Reserved> reserved;

		/** @brief Static pointer to the user-defined update function executed every frame. */
		static void (*userUpdateCallback)();

		int lastFrameTime = 0;
		float currentDeltaTime = 0.0f;

		// Const/dest:
		/**
		 * @brief Private default constructor (part of the Singleton pattern).
		 */
		Base();

		// Engine init:
		/**
		 * @brief Prints initial information about the engine and environment to the console.
		 * @private
		 */
		void printInitInfo();

		/**
		 * @brief Internal engine initialization routine (sets up rendering context, etc.).
		 * @param argc Pointer to the main argument count.
		 * @param argv Array of argument strings.
		 * @param winName The title of the rendering window.
		 * @param width The initial width of the rendering window (default: 1600).
		 * @param height The initial height of the rendering window (default: 600).
		 * @private
		 */
		void initEngine(int *argc, char *argv[], const char *winName, int width = 1600, int height = 600);

		/**
		* @bried Initialize the engine shaders
		*/
		void initShaders();

}; // end of class Base
} // end of namespace Eng
