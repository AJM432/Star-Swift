#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "implot.h"

#include <stdio.h>
#include <random>
#include <cmath>

#include "QuadTree.hpp"
#include "helper.h"

const int NUM_STARS = 5000;
const int RADIUS = 300;

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
  auto ACTUAL_WIDTH = DM.w;

    SDL_Window* window = SDL_CreateWindow(
        "Galaxy Modeller",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        ACTUAL_WIDTH,
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
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // State Variables
    ImVec4 galaxy_color = ImVec4(0.25f, 0.35f, 0.90f, 1.00f);
    float gravity_strength = 100.f; // also the point mass
    float max_speed = 10000.f;
    float theta = 1.5f;
    int soft_power = 2;
    bool show_velocity_vectors = false;
    bool show_gravity_vectors = false;
    double total_kinetic_energy = 0;
    double total_gravitational_potential_energy = 0;
    const int plot_size = 1001;
    static float kinetic_energies[plot_size];
    static float gravitational_energies[plot_size];
    static float xs1[plot_size];
    for (int i = 0; i < plot_size; ++i) {
        xs1[i] = i * 1.0f;
    }
    enum ColorMode { Color_Solid, Color_Radial, Color_Velocity, Color_COUNT };
    static int color_mode = Color_Radial;
    const char* color_mode_names[Color_COUNT] = {"Radial", "Solid", "Velocity"};
    bool update = true;

    SDL_Event e; 
    bool quit = false; 
    double oldTime = SDL_GetTicks();
    
    Point* stars[NUM_STARS]; //need to free memory when resized and points move to child quadrants
    for(int i=0; i < NUM_STARS; i++) {
      double x = dist_pos_x(mt);
      double y_variance = sqrt(RADIUS*RADIUS - (x-width_middle)*(x-width_middle));
      std::uniform_real_distribution<double> dist_pos_y(-y_variance+height_middle, y_variance+height_middle);
      double y = dist_pos_y(mt);
      double central_x = x-SCREEN_WIDTH/2.0;
      double central_y = y-SCREEN_HEIGHT/2.0;
      double vx = central_x;
      double vy = central_y;
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

      if(update) {
        root->update_galaxy(root, deltaTime);
      }

      // reset state variables
      total_gravitational_potential_energy = 0;
      total_kinetic_energy = 0;
      for (int i=0; i < NUM_STARS; i++) {
        Point *p = stars[i];
        total_kinetic_energy += gravity_strength*distance(p->x, p->y, p->x+p->vx, p->y+p->vy); // 1/2mv^2
        total_gravitational_potential_energy -= gravity_strength*distance(p->x, p->y, p->x+p->ax, p->y+p->ay);


        p->update_star_color(root->center_of_mass_x, root->center_of_mass_y, RADIUS, max_speed, galaxy_color.x, galaxy_color.y, galaxy_color.z, color_mode);
        SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);
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

      ImGuiWindowFlags window_flags = 0;
      window_flags |= ImGuiWindowFlags_NoBackground;
      bool p_open = true;

      ImGui::Begin("Galaxy Settings", &p_open, window_flags);
      ImGui::SeparatorText("General");
      if(ImGui::Button("Pause/Start", ImVec2(ImGui::GetWindowSize().x*1.0f, 0.0f))){
        update = !update;
        }
      if (ImGui::Button("Reset Galaxy", ImVec2(ImGui::GetWindowSize().x*1.0f, 0.0f))) {
        for(int i=0; i < NUM_STARS; i++) {
          double x = dist_pos_x(mt);
          double y_variance = sqrt(RADIUS*RADIUS - (x-width_middle)*(x-width_middle));
          std::uniform_real_distribution<double> dist_pos_y(-y_variance+height_middle, y_variance+height_middle);
          double y = dist_pos_y(mt);
          double vx = (RADIUS-x)/(abs(RADIUS-x))/sqrt(x*x + y*y)*5000;
          double vy = (RADIUS-y)/(abs(RADIUS-y))/sqrt(x*x + y*y)*5000;
          *stars[i] = Point(x, y, vy, -vx, 0, 0);
    }

      }
      ImGui::SliderFloat("Gravitational Strength", &gravity_strength, 0.0f, 10000.f);
      ImGui::SliderFloat("Max Star Velocity", &max_speed, 0.0f, 10000.f);
      ImGui::SliderFloat("Theta Threshold", &theta, 0.0f, 5.0f);
      ImGui::SliderInt("Collision Softening", &soft_power, -5, 5);
      ImGui::ColorEdit3("Galaxy Color", (float*)&galaxy_color); // Edit 3 floats representing a color
      const char* get_color_mode = (color_mode >= 0 && color_mode < Color_COUNT) ? color_mode_names[color_mode] : "Unknown";
      ImGui::SliderInt("Color Modes", &color_mode, 0, Color_COUNT - 1, get_color_mode); // Use ImGuiSliderFlags_NoInp

      ImGui::SeparatorText("Vector Display");
      ImGui::Checkbox("Show Velocity Vectors", &show_velocity_vectors);
      ImGui::Checkbox("Show Gravity Vectors", &show_gravity_vectors);
      ImGui::SeparatorText("Stats");

    
    for (int i = 1; i < plot_size; ++i) {
        kinetic_energies[i-1] = kinetic_energies[i];
        gravitational_energies[i-1] = gravitational_energies[i];
    }
    kinetic_energies[plot_size-1] = total_kinetic_energy;
    gravitational_energies[plot_size-1] = total_gravitational_potential_energy;
    if (ImPlot::BeginPlot("System Energy")) {
        ImPlot::SetupAxes("time(ms)","Energy(J)");
        ImPlot::PlotLine("Kinetic Energy", xs1, kinetic_energies, plot_size);
        ImPlot::PlotLine("Gravitational Potential Energy", xs1, gravitational_energies, plot_size);
        ImPlot::EndPlot();
    }
      ////////////
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
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( window );
	SDL_Quit();

  for (int i=0; i < NUM_STARS; i++) {
    delete stars[i];
  }


	return 0;
}