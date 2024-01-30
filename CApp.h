#ifndef CAPP_H
#define CAPP_H

#include "SDL2-2.28.3/include/SDL.h"
#include "Image.h"

class CApp {
public:
	CApp();

	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRender();
	void OnExit();

private:
	// An instance of qbImage to store the image.
	Image m_image;

	// SDL2 stuff.
	bool isRunning;
	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;
};

#endif // !CAPP_H