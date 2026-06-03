/**
 * @file    guiobjects.h
 * @brief   GUIObjects class
 *
 * @author  Giona Valsecchi (C) SUPSI [giona.valsecchi@supsi.ch] , Pietro Brusadelli (C) SUPSI [pietro.brusadelli@supsi.ch], Filippo De Simoni (C) SUPSI [filippo.desimoni@supsi.ch]
 */

#pragma once

/**
 * @brief Provides simple drawing utilities for 2D graphical user interface (GUI) elements and overlays.
 *
 * This class is typically used to draw debug information, text labels, or simple colored shapes directly
 * onto the screen, bypassing the 3D scene graph structure.
 */
class ENG_API GUIObjects
{
public:
	///////////////
	// Lifecycle //
	///////////////

	/**
	 * @brief Initializes the 2D rendering state.
	 *
	 * This typically switches the rendering pipeline to an orthographic projection (2D mode)
	 * and sets up the base modelview matrix.
	 * @param matrix The base modelview matrix to use for 2D drawing (usually identity).
	 */
	void start(glm::mat4 matrix);

	/**
	 * @brief Restores the rendering pipeline to the previous 3D state.
	 *
	 * This function should be called after all 2D drawing is complete.
	 */
	void stop();

	/////////////
	// Objects //
	/////////////

	/**
	 * @brief Draws a text string at a specified screen position.
	 *
	 * @param pos The 2D screen coordinate (x, y) for the start of the text.
	 * @param text A pointer to the null-terminated string of characters to draw.
	 * @param color The \c glm::vec4 color (R, G, B, A) of the text.
	 */
	void drawText(glm::vec2 pos, const unsigned char* text, glm::vec4 color);

	/**
	 * @brief Draws a solid color rectangle on the screen.
	 *
	 * @param pos The 2D screen coordinate (x, y) for the top-left corner of the rectangle.
	 * @param width The width of the rectangle in pixels.
	 * @param height The height of the rectangle in pixels.
	 * @param color The \c glm::vec4 color (R, G, B, A) of the rectangle.
	 */
	void drawRect(glm::vec2 pos, float width, float height, const glm::vec4 color);

	/////////////////////
	// Drawing windows //
	/////////////////////

	/**
	 * @brief Gets the current width of the drawable window.
	 * @return The width of the window in pixels.
	 */
	int getWidth();

	/**
	 * @brief Gets the current height of the drawable window.
	 * @return The height of the window in pixels.
	 */
	int getHeight();

private:
	/** @brief The base transformation matrix used for 2D drawing. */
	glm::mat4 drawingView = glm::mat4(1.0f);
};
