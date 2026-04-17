#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>

// would it be more appropriate for these constants to live in the SquareMazeData struct?
const size_t MINIMUM_MAZE_SIZE = 2;
const size_t MAXIMUM_MAZE_SIZE = 100; // might adjust later

enum class Direction : std::uint8_t
{
    NORTH = 1u << 3, // 8  (1000)
    SOUTH = 1u << 2, // 4  (0100)
    EAST = 1u << 1, // 2  (0010)
    WEST = 1u << 0, // 1  (0001)
};

using CellMask = std::uint8_t; // only needs 4 bits; uint8_t is ideal

constexpr CellMask ToMask(Direction d) noexcept
{
    return static_cast<CellMask>(d);
}

constexpr bool HasOpen(CellMask cell, Direction d) noexcept
{
    return (cell & ToMask(d)) != 0;
}

constexpr void SetOpen(CellMask& cell, Direction d, bool open) noexcept
{
    if (open) cell |= ToMask(d);
    else      cell &= static_cast<CellMask>(~ToMask(d));
}

constexpr std::size_t IndexOf(std::size_t mazeSize, std::size_t row, std::size_t col) noexcept
{
    return (mazeSize * row) + col;
}

struct SquareMazeData
{
    std::size_t mazeSize = 0;             // width == height
    std::vector<CellMask> cellData;       // flat array of CellMasks

    std::size_t CellCount() const noexcept { return mazeSize * mazeSize; }

    bool InBounds(std::size_t row, std::size_t col) const noexcept
    {
        return row < mazeSize && col < mazeSize;
    }

    CellMask& Cell(std::size_t row, std::size_t col)
    {
        return cellData[IndexOf(mazeSize, row, col)];
    }

    const CellMask& Cell(std::size_t row, std::size_t col) const
    {
        return cellData[IndexOf(mazeSize, row, col)];
    }

    // Having Walls() will make rendering the maze in Raylib easier (I think?)
    static constexpr CellMask AllDirsMask = static_cast<CellMask>(0b1111);
    CellMask Walls(const CellMask& cell) const noexcept
    {
        return static_cast<CellMask>((~cell) & AllDirsMask);
    }

};

// an unweighted, directed graph data structure
// key: the index of a node
// value: a list node indexes that key is adjacent (i.e. open) to
using MazeNodeId = std::size_t;
using MazeGraph = std::unordered_map<MazeNodeId, std::vector<MazeNodeId>>;
using MazeGraph = std::unordered_map<MazeNodeId, std::vector<MazeNodeId>>;

struct MazeGraphData
{
    MazeGraph graph;
    MazeNodeId start;
    MazeNodeId goal;
};

class SquareMaze
{
public:
    SquareMaze(size_t mazeSize);
    ~SquareMaze() = default;

    MazeGraphData AsGraph() const;
    void DebugMaze();

    // Todo public function declarations
private:
    SquareMazeData data;
};

inline SquareMaze::SquareMaze(size_t mazeSize)
{
    // - Create a new random square maze of size mazeSize (both width and height) as the private variable 'data'

    // initialize the data struct
    // edge cases maybe we should enforce a minimum mazeSize of 2, and a max for good measure
    size_t clamped = std::clamp(mazeSize, MINIMUM_MAZE_SIZE, MAXIMUM_MAZE_SIZE);
    data.mazeSize = clamped;
    data.cellData.assign(clamped * clamped, CellMask{ 0 });
    const size_t n = data.CellCount();

    auto popcount4 = [](CellMask m) -> int
        {
            // Counts how many directions are currently open in this cell (how many of the 4 bits are set).
            // Used to enforce the "1 to 3 open directions per cell" rule.
            m &= SquareMazeData::AllDirsMask;
            int c = 0;
            for (; m; m = static_cast<CellMask>(m & (m - 1))) ++c;
            return c;
        };

    auto opposite = [](Direction d) -> Direction
        {
            // Returns the opposite direction (needed because when we open a passage from A->B,
            // we must also open the matching passage from B->A).
            switch (d) {
            case Direction::NORTH: return Direction::SOUTH;
            case Direction::SOUTH: return Direction::NORTH;
            case Direction::EAST:  return Direction::WEST;
            case Direction::WEST:  return Direction::EAST;
            default:               return Direction::NORTH; // unreachable
            }
        };

    auto carve = [&](MazeNodeId a, MazeNodeId b, Direction dirAB)
        {
            // "Carves" a corridor between two adjacent cells by opening dirAB on cell a,
            // and the opposite direction on cell b (so the maze is consistent both ways).
            SetOpen(data.cellData[a], dirAB, true);
            SetOpen(data.cellData[b], opposite(dirAB), true);
        };

    // simple RNG setup
    std::mt19937 rng{
        std::random_device{}()
    };


    std::vector<bool> visited(n, false);
    std::vector<MazeNodeId> stack;
    stack.push_back(0);
    visited[0] = true;
    while (!stack.empty())
    {
        MazeNodeId cur = stack.back();
        size_t row = cur / data.mazeSize;
        size_t col = cur % data.mazeSize;

        // STEP A
        // - create candidate list
        struct Candidate { MazeNodeId next; Direction dir; };
        std::vector<Candidate> candidates;
        candidates.reserve(4); // there will never be more candidates than there are directions

        // - add each neighbor (bounds & visited checks)
        //   - remember mazeSize is both the width of each row, and the height of each column

        // NORTH neighbor
        if (row > 0) {
            MazeNodeId next = cur - data.mazeSize;
            if (!visited[next]) {
                candidates.push_back(
                    { next, Direction::NORTH }
                );
            }
        }

        // SOUTH neighbor
        if (row + 1 < data.mazeSize) {
            MazeNodeId next = cur + data.mazeSize;
            if (!visited[next]) {
                candidates.push_back(
                    { next, Direction::SOUTH }
                );
            }
        }

        // EAST neighbor
        if (col + 1 < data.mazeSize) {
            MazeNodeId next = cur + 1;
            if (!visited[next]) {
                candidates.push_back(
                    { next, Direction::EAST }
                );
            }
        }

        // WEST neighbor
        if (col > 0) {
            MazeNodeId next = cur - 1;
            if (!visited[next]) {
                candidates.push_back(
                    { next, Direction::WEST }
                );
            }
        }

        // STEP B
        // - backtrack if no candidates
        if (candidates.empty()) {
            stack.pop_back();
            continue;
        }

        // - otherwise...
        //   - pick one at random
        std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
        const Candidate pick = candidates[dist(rng)];

        //   - and carve forward
        carve(cur, pick.next, pick.dir);
        visited[pick.next] = true;
        stack.push_back(pick.next);
    }
}

inline MazeGraphData SquareMaze::AsGraph() const
{
    MazeGraphData out = {};

    const size_t size = data.mazeSize;
    const size_t n = data.CellCount();

    out.start = 0;                     // first cell (upper-left)
    out.goal = (n == 0) ? 0 : (n - 1); // last cell (lower-right)

    // Ensure even dead ends exist as keys
    for (MazeNodeId id = 0; id < n; ++id) {
        out.graph[id]; // default constructs empty vector even if missing
    }

    for (MazeNodeId id = 0; id < n; ++id) {
        const CellMask cell = data.cellData[id];
        const size_t row = id / size;
        const size_t col = id % size;

        if (HasOpen(cell, Direction::NORTH) && row > 0) {
            out.graph[id].push_back(id - size);
        }
        if (HasOpen(cell, Direction::SOUTH) && (row + 1) < size) {
            out.graph[id].push_back(id + size);
        }
        if (HasOpen(cell, Direction::EAST) && (col + 1) < size) {
            out.graph[id].push_back(id + 1);
        }
        if (HasOpen(cell, Direction::WEST) && col > 0) {
            out.graph[id].push_back(id - 1);
        }
    }
    return out;
}

inline void SquareMaze::DebugMaze() {
    // Prints the maze as a grid of 2-digit hex CellMask values (00..0F).
    // Note: cast CellMask to int so uint8_t doesn't print as a character.
    for (std::size_t r = 0; r < data.mazeSize; ++r) {
        for (std::size_t c = 0; c < data.mazeSize; ++c) {
            const CellMask cell = data.Cell(r, c);
            std::cout
                << std::uppercase << std::hex
                << std::setw(2) << std::setfill('0')
                << static_cast<int>(cell)
                << ' ';
        }
        std::cout << '\n';
    }

    // Reset stream state so later prints aren’t accidentally hex.
    std::cout << std::dec << std::nouppercase;
}



