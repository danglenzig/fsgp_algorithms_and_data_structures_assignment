# Sorting Visualizer (Raylib + C++) — Documentation

This project visualizes sorting algorithms as animated bar charts using Raylib. Each algorithm runs incrementally: every call to `Advance()` performs a small piece of work so you can see comparisons, swaps, and sorted regions evolve over time.

Implemented sorting scenes:
- Bubble sort (`Sorting/BubbleSortScene.h`)
- Selection sort (`Sorting/SelectionSortScene.h`)
- Insertion sort (`Sorting/InsertionSortScene.h`)
- Heap sort (`Sorting/HeapSortScene.h`) — incremental heapify + extract

---

## Grading rubric checklist

- [x] Sorting visualizer similar to planning-session example
- [x] At least 3 sorting algorithms from course material
  - Implemented: Bubble, Selection, Insertion, Heap
- [x] Swap between the scenes with a button press
  - `N` cycles to the next sorting scene
- [x] Bonus: comparisons and swaps shown when the algorithm runs / completes
  - UI shows `comparisons` and `swaps`
  - Heap sort also shows build vs extract comparisons via `additionalInformation`

---

## Controls (sorting mode)

From `Tools/InputHandler.h`:

- `Up Arrow`: decrease step time (faster)
- `Down Arrow`: increase step time (slower)
- `R`: reset current algorithm (regenerate + shuffle + restart)
- `N`: next sorting algorithm
- `ESC`: quit (close Raylib window)

---

## Key design: incremental sorting with `Advance()`

All algorithms are implemented for visualization, not for fastest execution.

- Each sort scene derives from `SortScene` and overrides `Advance()`.
- The manager calls `Advance()` at a fixed interval (`stepInterval`).
- Each `Advance()` performs a small, repeatable unit of work.

### About `steps`

`steps` increments once per `Advance()` call.

That means **`steps` equals “visualization ticks”**, not a fixed algorithm-theory step count. `comparisons` and `swaps` are the more comparable metrics across algorithms.

---

## High-level architecture

- `Sorting/SortScene.h`
  - Base class for all algorithms
  - Holds stats (`steps`, `comparisons`, `swaps`), `barsList`, and `sorted`
  - Provides `Start()` + virtual hooks:
    - `AdditionalResetOps()`
    - `AdditionalStartOps()`

- `Sorting/SortSceneManager.h`
  - Owns scenes via `std::vector<std::unique_ptr<SortScene>>`
  - Tracks current scene via `currentSortSceneIdx`
  - Handles input events (Next, Reset, Speed)
  - Produces `SortSceneDrawData` for the renderer

- `RenderSystem/RenderSystem.h`
  - Draws UI + bars using `SortSceneDrawData`

- `Data/DataFactory.h`
  - Generates the bars list

---

## Program flow (update + render)

```mermaid
flowchart TD
    A["main.cpp"] --> B["InitWindow + SetTargetFPS"]
    B --> C["Create SortSceneManager"]
    C --> D["SortSceneManager::InitializeSceneData"]
    D --> E["currentSortScene->Start()"][" "]
    E --> F{"Main loop"}
    F --> G["EventSystem::FrameUpdate.Invoke(dT)"]
    G --> H["SortSceneManager::OnFrameUpdate(dT)"]
    H --> I{"tA >= stepInterval?"}
    I -->|No| F
    I -->|Yes| J["SortSceneManager::UpdateDrawData"]
    J --> K["currentSortScene->Advance()"]
    J --> L["Copy stats + barsList + additionalInfo into SortSceneDrawData"]
    F --> M["RenderSystem::RenderSortScene(drawData)"]
    M --> F
```

---

## Input/event flow (Next + Reset)

```mermaid
flowchart TD
    A["KEY_N pressed"] --> B["EventSystem::NextPressed.Invoke()"]
    B --> C["SortSceneManager::OnNextPressed"]
    C --> D["currentSortSceneIdx = (currentSortSceneIdx + 1) % sortScenes.size()"]
    D --> E["currentSortScene = sortScenes[currentSortSceneIdx].get()"]
    E --> F["currentSortScene->Start()"]

    G["KEY_R pressed"] --> H["EventSystem::ResetPressed.Invoke()"]
    H --> I["SortSceneManager::OnResetPressed"]
    I --> F
```

---

## `SortScene::Start()` lifecycle

```mermaid
flowchart TD
    A["SortScene::Start"] --> B["ResetStats()"]
    B --> C["AdditionalResetOps()  (virtual)"]
    C --> D["barsList = DataFactory::GetBarsList(120)"]
    D --> E["listSize = barsList.size()"]
    E --> F["Shuffle()"]
    F --> G["AdditionalStartOps() (virtual)"]
```

---

## Metrics

- `comparisons`: increments when bar heights are compared
- `swaps`: increments when `std::swap` is called on bars
- `steps`: increments once per tick (`Advance()` call)

Note: Heap sort can do up to **two height comparisons per tick** (left child and right child), so `comparisons` can be larger than `steps`.

---

## Algorithm flow diagrams (per `Advance()`)

### Bubble sort

```mermaid
flowchart TD
    A["Advance()"] --> B{"sorted?"}
    B -->|Yes| Z["return"]
    B -->|No| C{"outerIndex >= listSize?"}
    C -->|Yes| D["color all bars sorted; sorted=true; return"]
    C -->|No| E["steps++"]

    E --> F{"innerIndex >= listSize - outerIndex - 1?"}
    F -->|Yes| G["mark last unsorted bar sorted"]
    G --> H["reset colors of unsorted region"]
    H --> I["outerIndex++; innerIndex=0"]
    F -->|No| J["comparisons++"]

    I --> J
    J --> K["highlight bars innerIndex and innerIndex+1"]
    K --> L{"height[i] > height[i+1]?"}
    L -->|Yes| M["swap; swaps++"]
    L -->|No| N["no swap"]
    M --> O["innerIndex++"]
    N --> O
```

### Selection sort

```mermaid
flowchart TD
    A["Advance()"] --> B{"sorted?"}
    B -->|Yes| Z["return"]
    B -->|No| C{"outerIndex >= listSize?"}
    C -->|Yes| D["color all bars sorted; sorted=true; return"]
    C -->|No| E["steps++"]

    E --> F["color sorted prefix green, rest gray"]
    F --> G{"scanIndex >= listSize?"}
    G -->|Yes| H{"minIndex != outerIndex?"}
    H -->|Yes| I["swap; swaps++"]
    H -->|No| J["no swap"]
    I --> K["outerIndex++; minIndex=outerIndex; scanIndex=outerIndex+1; return"]
    J --> K

    G -->|No| L["highlight minIndex (RED) and scanIndex (GOLD)"]
    L --> M["comparisons++"]
    M --> N{"height[scan] < height[min]?"}
    N -->|Yes| O["minIndex = scanIndex"]
    N -->|No| P["no change"]
    O --> Q["scanIndex++"]
    P --> Q
```

### Insertion sort

```mermaid
flowchart TD
    A["Advance()"] --> B{"sorted?"}
    B -->|Yes| Z["return"]
    B -->|No| C{"outerIndex >= listSize?"}
    C -->|Yes| D["color all bars sorted; sorted=true; return"]
    C -->|No| E["steps++"]

    E --> F["color sorted prefix green; rest gray"]
    F --> G{"passActive?"}
    G -->|No| H["innerIndex=outerIndex; passActive=true"]
    G -->|Yes| I["continue"]

    H --> I
    I --> J["highlight key (RED) at innerIndex"]
    J --> K{"innerIndex <= 0?"}
    K -->|Yes| L["outerIndex++; passActive=false; return"]
    K -->|No| M["comparisons++"]
    M --> N["highlight left neighbor (GOLD)"]
    N --> O{"height[left] > height[key]?"}
    O -->|Yes| P["swap adjacent; swaps++; innerIndex--; return"]
    O -->|No| Q["outerIndex++; passActive=false; return"]
```

### Heap sort (two phases)

Heap sort uses persistent state so `sift-down` can be incremental across ticks:
- `siftInProgress`
- `siftIdx`
- `siftTargetEnd`

```mermaid
flowchart TD
    A["Advance()"] --> B{"sorted?"}
    B -->|Yes| Z["return"]
    B -->|No| C["steps++"]
    C --> D{"phase == BUILD_HEAP?"}

    D -->|Yes| E{"idx < 0 AND !siftInProgress?"}
    E -->|Yes| F["phase=EXTRACT; recolor default; return"]
    E -->|No| G{"!siftInProgress?"}
    G -->|Yes| H["start build sift: siftIdx = idx; siftInProgress=true"]
    G -->|No| I["continue build sift"]
    H --> J["SiftDownBuild() (one step)"]
    I --> J
    J --> K{"sift finished?"}
    K -->|Yes| L["idx--"]
    K -->|No| M["keep idx"]
    L --> N["BuildHeapInfoString(); return"]
    M --> N

    D -->|No| O{"phase == EXTRACT"}
    O --> P{"end <= 0 AND !siftInProgress?"}
    P -->|Yes| Q["color bars[0] sorted; sorted=true; return"]
    P -->|No| R{"!siftInProgress?"}
    R -->|Yes| S["swap(0,end); swaps++; mark end green; start extract sift"]
    R -->|No| T["continue extract sift"]
    S --> U["SiftDownExtract() (one step)"]
    T --> U
    U --> V{"sift finished?"}
    V -->|Yes| W["end--"]
    V -->|No| X["keep end"]
    W --> Y["BuildHeapInfoString(); return"]
    X --> Y
```

---

## Class relationships

```mermaid
classDiagram
    class SortScene {
        # string sortSceneName
        # string additionalInformation
        # int steps
        # int swaps
        # int comparisons
        # vector~SortBarData~ barsList
        # int listSize
        # bool sorted
        + void Advance()
        + void Start()
        + void ResetStats()
        + void Shuffle()
        + void AdditionalResetOps()
        + void AdditionalStartOps()
        + SortSceneStats GetStats()
        + vector~SortBarData~ GetBarsList()
        + string GetName()
        + string GetAdditionalInfo()
    }

    class BubbleSortScene
    class SelectionSortScene
    class InsertionSortScene
    class HeapSortScene

    SortScene <|-- BubbleSortScene
    SortScene <|-- SelectionSortScene
    SortScene <|-- InsertionSortScene
    SortScene <|-- HeapSortScene

    class SortSceneManager {
        - vector~unique_ptr~SortScene~~ sortScenes
        - int currentSortSceneIdx
        - SortScene* currentSortScene
        + void InitializeSceneData()
        + SortSceneDrawData GetDrawData()
    }

    SortSceneManager o--> SortScene : owns (polymorphic)
```
