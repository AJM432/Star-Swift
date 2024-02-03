#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <stdio.h>
#include <random>
#include <cmath>

#include "QuadTree.hpp"

const int NUM_STARS = 5;
const int RADIUS = 20;

int main( int argc, char* argv[] )
{

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

  int WINDOW_GAP = 50;
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  auto SCREEN_HEIGHT = DM.h-WINDOW_GAP;
  auto SCREEN_WIDTH = SCREEN_HEIGHT;
  // auto SCREEN_WIDTH = DM.w;

    SDL_Window* window = SDL_CreateWindow(
        "Galaxy Modeller",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  // setup random number generator
  double width_middle = SCREEN_WIDTH/2;
  double height_middle = SCREEN_HEIGHT/2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist_pos_x(width_middle-RADIUS, width_middle+RADIUS);
  std::uniform_real_distribution<double> dist_vel(-1000.0, 1000.0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Our state
    ImVec4 galaxy_color = ImVec4(0.25f, 0.35f, 0.90f, 1.00f);
    float gravity_strength = 100.f;
    float max_speed = 200.f;
    float theta = 0.5f;
    int soft_power = 2;
    bool show_velocity_vectors = false;
    bool show_gravity_vectors = false;

    SDL_Event e; 
    bool quit = false; 
    double oldTime = SDL_GetTicks();
    
    Point* stars[NUM_STARS]; //need to free memory when resized and points move to child quadrants
    for(int i=0; i < NUM_STARS; i++) {
      double x = dist_pos_x(mt);
      double y_variance = sqrt(RADIUS*RADIUS - (x-width_middle)*(x-width_middle));
      std::uniform_real_distribution<double> dist_pos_y(-y_variance+height_middle, y_variance+height_middle);
      double y = dist_pos_y(mt);
      double vx = (RADIUS-x)/(abs(RADIUS-x))/sqrt(x*x + y*y)*5000;
      double vy = (RADIUS-y)/(abs(RADIUS-y))/sqrt(x*x + y*y)*5000;
      stars[i] = new Point(x, y, vy, -vx, 0, 0);
  }

    while(!quit){
      while(SDL_PollEvent( &e ) != 0){ 
        ImGui_ImplSDL2_ProcessEvent(&e);
        if( e.type == SDL_QUIT ){
          quit = true; 
        }
      } 
      // rebuild quadtree
      QuadTree* root = new QuadTree(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, gravity_strength, max_speed, theta, soft_power);
      for (int i=0; i < NUM_STARS; i++) {
        root->insert(stars[i]);
      }

      // Start the Dear ImGui frame
      ImGui_ImplSDLRenderer2_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();

      double newTime = SDL_GetTicks();
      double deltaTime = (newTime - oldTime) / 1000.0f;

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      root->update_galaxy(root, deltaTime);

      for (int i=0; i < NUM_STARS; i++) {
        Point *p = stars[i];
        p->update_star_color(root->center_of_mass_x, root->center_of_mass_y, RADIUS, galaxy_color.x, galaxy_color.y, galaxy_color.z);
        SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);
        // SDL_SetRenderDrawColor(renderer, galaxy_color.x*255, galaxy_color.y*255, galaxy_color.z*255, galaxy_color.z*255);
        SDL_RenderDrawPoint(renderer, p->x, p->y);
        if(show_velocity_vectors) {
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          SDL_RenderDrawLine(renderer, p->x, p->y, p->x + p->vx/5.0, p->y + p->vy/5.0);
        }
        if(show_gravity_vectors) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer,
                       p->x, p->y, p->x + p->ax, p->y + p->ay);
        }
      }
      delete root;
      root = nullptr;

      static int counter = 0;

      ImGui::Begin("Settings");
      ImGui::Text("Galaxy Modeller Parameters");
      ImGui::SliderFloat("Gravity Strength", &gravity_strength, 0.0f, 10000.f);
      ImGui::SliderFloat("Max Star Velocity", &max_speed, 0.0f, 10000.f);
      ImGui::SliderFloat("Theta Threshold", &theta, 0.0f, 5.0f);
      ImGui::SliderInt("Softening Power", &soft_power, -5, 5);
      ImGui::ColorEdit3("Galaxy Color", (float*)&galaxy_color); // Edit 3 floats representing a color
      ImGui::Checkbox("Show Velocity Vectors", &show_velocity_vectors);
      ImGui::Checkbox("Show Gravity Vectors", &show_gravity_vectors);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();

    // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(galaxy_color.x * 255), (Uint8)(galaxy_color.y * 255), (Uint8)(galaxy_color.z * 255), (Uint8)(galaxy_color.w * 255));
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

      SDL_RenderPresent(renderer);

      SDL_Delay(1);
      oldTime = newTime;
    }

  // Cleanup
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( window );
	SDL_Quit();

  for (int i=0; i < NUM_STARS; i++) {
    delete stars[i];
  }


	return 0;
}