//If I could rename this I would change it to main
//but I know I will break stuff doing so and don't want to :)
#include "CApp.h"

#include "rtweekend.h"

#include "bvh.h"
#include "camera.h"
#include "colour.h"
#include "constant_medium.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"


// The constructor (default)
CApp::CApp()
{
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;
}

bool CApp::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		return false;
	}

	pWindow = SDL_CreateWindow("qbRayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	if (pWindow != NULL)
	{
		// Initialise the renderer.
		pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

		// Initialise the Image instance.
		m_image.Initialize(1280, 720, pRenderer);

		// THIS IS THE MAIN PART
		// THIS IS WHAT GETS DISPLAYED ON THE SCREEN
        hittable_list boxes1;
        auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

        int boxes_per_side = 20;
        for (int i = 0; i < boxes_per_side; i++) {
            for (int j = 0; j < boxes_per_side; j++) {
                auto w = 100.0;
                auto x0 = -1000.0 + i * w;
                auto z0 = -1000.0 + j * w;
                auto y0 = 0.0;
                auto x1 = x0 + w;
                auto y1 = random_double(1, 101);
                auto z1 = z0 + w;

                boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
            }
        }

        hittable_list world;

        world.add(make_shared<bvh_node>(boxes1));

        auto light = make_shared<diffuse_light>(color(7, 7, 7));
        world.add(make_shared<quad>(point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

        auto center1 = point3(400, 400, 200);
        auto center2 = center1 + vec3(30, 0, 0);
        auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
        world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

        world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
        world.add(make_shared<sphere>(
            point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
            ));

        auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
        world.add(boundary);
        world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
        boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
        world.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

        /*auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
        world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
        auto pertext = make_shared<noise_texture>(0.1);
        world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));*/

        hittable_list boxes2;
        auto white = make_shared<lambertian>(color(.73, .73, .73));
        int ns = 1000;
        for (int j = 0; j < ns; j++) {
            boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
        }

        world.add(make_shared<translate>(
            make_shared<rotate_y>(
                make_shared<bvh_node>(boxes2), 15),
            vec3(-100, 270, 395)
            )
        );

        camera cam;

        //These variables can all be adjusted
        cam.aspect_ratio = 1.0;
        cam.image_width = 600;
        //For "optimization" reason you can decrease this number below
        cam.samples_per_pixel = 200;

        cam.max_depth = 50;
        cam.background = color(0, 0, 0);

        //Do not adjust these numbers
        cam.vfov = 40;
        cam.lookfrom = point3(478, 278, -600);
        cam.lookat = point3(278, 278, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocus_angle = 0;

        cam.render(world);

    }
	else
	{
		return false;
	}

	return true;
}

int CApp::OnExecute()
{
	SDL_Event event;

	if (OnInit() == false)
	{
		return -1;
	}

	while (isRunning)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			OnEvent(&event);
		}

		OnLoop();
		OnRender();
	}

	OnExit();
	return 0;
}

void CApp::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_QUIT)
	{
		isRunning = false;
	}
}

void CApp::OnLoop()
{

}

void CApp::OnRender()
{
	// Set the background colour to white.
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderClear(pRenderer);

	// Display the image.
	//m_image.Display();

	// Show the result.
	SDL_RenderPresent(pRenderer);
}

void CApp::OnExit()
{
	// Tidy up SDL2 stuff.
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
}