# Algorithms and Data Structures — Visualizations (Raylib + C++)

This project is a school assignment for the course unit **“Algorithms and Data Structures.”**

It contains two interactive visualizations:

1. **Sorting algorithm visualization** (required for **G / passing**)
2. **Pathfinding visualization** (for **VG / pass with distinction**)

## Documentation

- Sorting (G): **[SORTING_DOCUMENTATION.md](./SORTING_DOCUMENTATION.md)**
- Pathfinding (VG): **[PATHFINDING_DOCUMENTATION.md](./PATHFINDING_DOCUMENTATION.md)**

## How to navigate the code

- Entry point / main loop: `main.cpp`
- Shared systems:
  - Input → events: `Tools/InputHandler.h`, `Tools/EventSystem.h`
  - Mode switching: `Tools/MainMgr.h`
  - Rendering: `RenderSystem/RenderSystem.h`
- Sorting:
  - Scene manager: `Sorting/SortSceneManager.h`
  - Algorithms: `Sorting/*SortScene.h`
- Pathfinding:
  - Scene manager: `Pathfinding/PathfindingSceneMgr.h`
  - Maze generation + maze→graph conversion: `Pathfinding/MazeTools.h`

## High-level runtime flow

Both visualizations share the same update/render loop:

- `main.cpp` initializes systems and both scene managers.
- Each frame:
  - `EventSystem::FrameUpdate` is invoked with `dT`.
  - The active scene manager (sorting or pathfinding) updates its internal state.
  - `RenderSystem` draws based on `MainMgr::currentVizMode`.

```mermaid
flowchart TD
    A[main.cpp] --> B[InitWindow / main loop]
    B --> C[InputHandler]
    C --> ES[EventSystem]

    ES -->|FrameUpdate| SM[SortSceneManager]
    ES -->|FrameUpdate| PM[PathfindingSceneMgr]

    MM[MainMgr\ncurrentVizMode] --> R{Which mode?}
    R -->|Sorting| SDD[SortSceneDrawData]
    R -->|Pathfinding| PDD[PathfindingSceneDrawData]

    SDD --> RS[RenderSystem]
    PDD --> RS[RenderSystem]
```

## Controls (global)

Controls are read in `Tools/InputHandler.h` and forwarded as events via `Tools/EventSystem.h`.

- `ESC`: quit
- `S`: switch to sorting mode
- `P`: switch to pathfinding mode
- `R`: reset current visualization (scene-specific)

Sorting-only:

- `Up Arrow`: faster (decrease step interval)
- `Down Arrow`: slower (increase step interval)
- `N`: next sorting algorithm
