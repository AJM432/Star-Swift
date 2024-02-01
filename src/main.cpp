// #include <SDL2/SDL.h>
// #include "QuadTree.hpp"
// #include <random>


// const int SCREEN_WIDTH = 600;
// const int SCREEN_HEIGHT = 600;
// const int NUM_STARS = 20000;
// const int RADIUS = 100;

// int main( int argc, char* argv[] )
// {
//     std::random_device rd;
//     std::mt19937 mt(rd());
//     std::uniform_real_distribution<double> dist_pos_x(100.0, 500.0);
//     std::uniform_real_distribution<double> dist_vel(-5.0, 5.0);

//     SDL_Window* window = SDL_CreateWindow(
//         "Galaxy Modeller",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         SCREEN_WIDTH,
//         SCREEN_HEIGHT,
//         SDL_WINDOW_SHOWN
//     );

//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     SDL_Renderer* GUI_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     SDL_Event e; 
//     bool quit = false; 
//     double oldTime = SDL_GetTicks();


    
//     Point* stars[NUM_STARS]; //need to free memory when resized and points move to child quadrants
//     for(int i=0; i < NUM_STARS; i++) {
//       double x = dist_pos_x(mt);
//       std::uniform_real_distribution<double> dist_pos_y(-sqrt(200*200-(x-300)*(x-300))+300, sqrt(200*200-(x-300)*(x-300))+300);
//       double y = dist_pos_y(mt);
//       double vx = (300-x)/(abs(300-x))/sqrt(x*x + y*y)*5000;
//       double vy = (300-y)/(abs(300-y))/sqrt(x*x + y*y)*5000;
//       stars[i] = new Point(x, y, vy, -vx, 0, 0);
//   }

//     while(!quit){
//       while(SDL_PollEvent( &e ) != 0){ 
//         if( e.type == SDL_QUIT ){
//           quit = true; 
//         }
//       } 
//       // rebuild quadtree
//       QuadTree* root = new QuadTree(0, 0, 600, 600);
//       for (int i=0; i < NUM_STARS; i++) {
//         root->insert(stars[i]);
//       }

//       double newTime = SDL_GetTicks();
//       double deltaTime = (newTime - oldTime) / 1000.0f;

//       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//       SDL_RenderClear(renderer);

//       root->update_galaxy(root, deltaTime);

//       for (int i=0; i < NUM_STARS; i++) {
//         Point *p = stars[i];
//         SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);
//         SDL_RenderDrawPoint(renderer, p->x, p->y);
//         SDL_RenderDrawLine(renderer,
//                        p->x, p->y, p->x + p->vx/5.0, p->y + p->vy/5.0);
//       }
//       delete root;
//       root = nullptr;


//       SDL_RenderPresent(renderer);

//       SDL_Delay(1);
//       oldTime = newTime;
//     }


//   SDL_DestroyRenderer(renderer);
// 	SDL_DestroyWindow( window );
// 	SDL_Quit();

//   for (int i=0; i < NUM_STARS; i++) {
//     delete stars[i];
//   }


// 	return 0;
// }


#include <SDL2/SDL.h>
#include "QuadTree.hpp"
#include <random>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int NUM_STARS = 20000;
const int RADIUS = 100;

int main(int argc, char* argv[]) {
    // ... (your existing code)

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    while (!quit) {
        // Poll events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // ImGui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(guiWindow);
        ImGui::NewFrame();

        // Your ImGui GUI code
        ImGui::Begin("Parameters");
        ImGui::SliderInt("Parameter 1", &parameter1, 0, 100);
        ImGui::SliderFloat("Parameter 2", &parameter2, 0.0f, 1.0f);
        ImGui::End();

        // ImGui Rendering
        ImGui::Render();
        glViewport(0, 0, static_cast<int>(ImGui::GetIO().DisplaySize.x), static_cast<int>(ImGui::GetIO().DisplaySize.y));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Your existing simulation rendering code
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        QuadTree* root = new QuadTree(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        for (int i = 0; i < NUM_STARS; i++) {
            root->insert(stars[i]);
        }

        double newTime = SDL_GetTicks();
        double deltaTime = (newTime - oldTime) / 1000.0f;

        root->update_galaxy(root, deltaTime);

        for (int i = 0; i < NUM_STARS; i++) {
            Point* p = stars[i];
            SDL_SetRenderDrawColor(renderer, p->r, p->g, p->b, 255);
            SDL_RenderDrawPoint(renderer, p->x, p->y);
            SDL_RenderDrawLine(renderer, p->x, p->y, p->x + p->vx / 5.0, p->y + p->vy / 5.0);
        }
        delete root;
        root = nullptr;

        SDL_RenderPresent(renderer);

        SDL_Delay(1);
        oldTime = newTime;
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    // ... (your existing cleanup code)
    
    return 0;
}