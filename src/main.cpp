#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <stdio.h>
#include <random>

#include "QuadTree.hpp"

// const int SCREEN_WIDTH = 600;
// const int SCREEN_HEIGHT = 600;
const int NUM_STARS = 5000;
const int RADIUS = 200;

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
  auto SCREEN_WIDTH = DM.w;
  auto SCREEN_HEIGHT = DM.h-WINDOW_GAP;

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
  std::uniform_real_distribution<double> dist_vel(-5.0, 5.0);

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
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    SDL_Event e; 
    bool quit = false; 
    double oldTime = SDL_GetTicks();


    
    Point* stars[NUM_STARS]; //need to free memory when resized and points move to child quadrants
    for(int i=0; i < NUM_STARS; i++) {
      double x = dist_pos_x(mt);
      double y_variance = sqrt(RADIUS*RADIUS - (x-width_middle)*(x-width_middle));
      std::uniform_real_distribution<double> dist_pos_y(-y_variance+height_middle, y_variance+height_middle);
      double y = dist_pos_y(mt);
      double vx = (300-x)/(abs(300-x))/sqrt(x*x + y*y)*5000;
      double vy = (300-y)/(abs(300-y))/sqrt(x*x + y*y)*5000;
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
      QuadTree* root = new QuadTree(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr);
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
        // SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);
        SDL_SetRenderDrawColor(renderer, clear_color.x*255, clear_color.y*255, clear_color.z*255, clear_color.z*255);
        SDL_RenderDrawPoint(renderer, p->x, p->y);
        // SDL_RenderDrawLine(renderer,
        //                p->x, p->y, p->x + p->vx/5.0, p->y + p->vy/5.0);
      }
      delete root;
      root = nullptr;

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
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