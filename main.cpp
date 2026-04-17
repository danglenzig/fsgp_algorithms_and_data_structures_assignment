// fsgp_algorithms_and_data_structures_assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Tools/EventSystem.h"
#include "Tools/Event.h"
#include "Tools/InputHandler.h"
#include "Sorting/SortSceneManager.h"
#include "Pathfinding/PathfindingSceneMgr.h"
#include "RenderSystem/RenderSystem.h"
#include "Data/structs.h"
#include "Tools/MainMgr.h"
#include "raylib.h"



int main()
{
    MainMgr& mainMgr = MainMgr::Instance();
    mainMgr.SetVizMode(MainMgr::VizMode::SORTING);

    EventSystem& eventSystem = EventSystem::Instance();
    InputHandler& inputHandler = InputHandler::Instance();
    RenderSystem& renderSystem = RenderSystem::Instance();    

    const int WIDTH = 1280;
    const int HEIGHT = 720;
    const float STEP_INTERVAL = 0.02f;

    SortSceneManager sortSceneMgr = SortSceneManager(STEP_INTERVAL);
    sortSceneMgr.InitializeSceneData();
    sortSceneMgr.SetIsActive(true); // for now just automatically turn it on

    PathfindingSceneMgr pfSceneMgr = PathfindingSceneMgr(STEP_INTERVAL);
    pfSceneMgr.InitializeSceneData();
    pfSceneMgr.SetIsActive(false);


    size_t sHandle = eventSystem.SortingPressed.Subscribe(
        [&sortSceneMgr, &pfSceneMgr]() {
            if (MainMgr::Instance().currentVizMode == MainMgr::VizMode::SORTING) { return; }
            pfSceneMgr.SetIsActive(false);
            sortSceneMgr.InitializeSceneData();
            sortSceneMgr.SetIsActive(true);
        }
    );
    size_t pHandle = eventSystem.PathfindingPressed.Subscribe(
        [&sortSceneMgr, &pfSceneMgr]() {
            if (MainMgr::Instance().currentVizMode == MainMgr::VizMode::PATHFINDING) { return; }
            
            // temp
            sortSceneMgr.Test();
            pfSceneMgr.Test();

            /*sortSceneMgr.SetIsActive(false);
            pfSceneMgr.InitializeSceneData();
            pfSceneMgr.SetIsActive(true);*/
        }
    );


    InitWindow(WIDTH, HEIGHT, "Algorithms and Data Structures");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        // Update
        float dT = GetFrameTime();
        eventSystem.FrameUpdate.Invoke(dT);
        inputHandler.GetInputEvents();

        // Draw
        BeginDrawing();
        ClearBackground({ 137, 207, 240, 255 }); // baby blue

        // Tell the RenderSystem to draw everything
        switch (mainMgr.currentVizMode) {
        case MainMgr::VizMode::SORTING:
            // get draw data from sortSceneMgr and tel the RenderSystem to draw it
            renderSystem.RenderSortScene(sortSceneMgr.GetDrawData());
            break;
        case MainMgr::VizMode::PATHFINDING:
            // get draw data from pathSceneMgr and tell the RenderSystem to draw it
            break;
        default:
            break;
        }


        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    return 0;

}