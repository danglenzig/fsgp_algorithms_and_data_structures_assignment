#pragma once
#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <limits>
#include <cstdio>

// WGraph is an unordered map, where the keys represent nodes,
// and the values are each a std::pair<int, float> indicating 
// the adjacent node (the int), // and the edge length (the 
// float)
using WGraph = std::unordered_map<int, std::vector<std::pair<int, float>>>;

// use this weighted graph for testing
WGraph GetTestGraph()
{
    WGraph graph;
    graph[0] = { {1, 1.0f}, {3, 8.0f} };
    graph[1] = { {0, 1.0f}, {2, 1.0f} };
    graph[2] = { {1, 1.0f}, {5, 1.0f} };
    graph[3] = { {0, 8.0f}, {5, 8.0f} };
    graph[5] = { {2, 1.0f}, {3, 8.0f} };
    return graph;
}

struct PathResult
{
    std::vector<int> path;
    float totalCost;
    int hops;
};


void PrintPathResult(const std::string& label, const PathResult& r)
{
    std::cout << label << "\n";
    std::cout << "  Path:  ";
    for (int i = 0; i < (int)r.path.size(); i++)
        std::cout << r.path[i] << (i < (int)r.path.size() - 1 ? " -> " : "");
    std::cout << "\n";
    printf("  Hops:  %d\n", r.hops);
    printf("  Cost:  %.1f\n\n", r.totalCost);
}

// TODO: Implement Djikstra on a weighted graph
// Use std::priority_queue with std::greater<std::pair<float,int>> for min-heap.
// Track costSoFar and cameFrom. Reconstruct path on goal found.
PathResult Dijkstra(const WGraph& graph, int start, int goal)
{
    // a new, empty path result
    PathResult result = {
        {}, 0.0f, 0
    };

    // edge cases
    if (start == goal) {
        result.path = { start };
        result.totalCost = 0.0f;
        result.hops = 0;
        return result;
    }
    if (graph.find(start) == graph.end() || graph.find(goal) == graph.end()) {
        return result;  // start and/or goal are not present in the graph
    }

    using PQItem = std::pair<float, int>; // "Priority Queue Item" (cost, node)
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> frontier; // our priority queue
    std::unordered_map<int, float> costSoFar;
    std::unordered_map<int, int> cameFrom;

    frontier.push({ 0.0f, start });
    costSoFar[start] = 0.0f;
    cameFrom[start] = start;

    while (!frontier.empty()) {
        auto [currentCost, current] = frontier.top();
        frontier.pop();

        // Skip stale heap entries
        auto itCost = costSoFar.find(current);
        if (itCost != costSoFar.end() && currentCost > itCost->second)
            continue;

        if (current == goal)
            break;

        auto itAdj = graph.find(current);
        if (itAdj == graph.end())
            continue;

        for (const auto& [next, edgeCost] : itAdj->second)
        {
            float newCost = currentCost + edgeCost;

            auto itNextCost = costSoFar.find(next);
            if (itNextCost == costSoFar.end() || newCost < itNextCost->second)
            {
                costSoFar[next] = newCost;
                cameFrom[next] = current;
                frontier.push({ newCost, next });
            }
        }
    }

    // If goal was never reached
    auto itGoalCost = costSoFar.find(goal);
    if (itGoalCost == costSoFar.end())
        return result;

    // Reconstruct path: goal -> ... -> start
    std::vector<int> reversed;
    for (int cur = goal;; cur = cameFrom[cur])
    {
        reversed.push_back(cur);
        if (cur == start) break;
    }
    std::reverse(reversed.begin(), reversed.end());

    result.path = std::move(reversed);
    result.totalCost = itGoalCost->second;
    result.hops = (result.path.empty() ? 0 : (int)result.path.size() - 1);
    return result;
}

PathResult FewestHops(const WGraph& graph, int start, int goal)
{
    // FewestHops ignores weights and finds the fewest-hops path between start and goal
    // It should accumulate the actual edge cost though (in result.totalCost), so this 
    // can be compared with other algorithms

    PathResult result = {
        {}, 0.0f, 0
    };

    // Edge cases
    if (start == goal)
    {
        result.path = { start };
        result.totalCost = 0.0f;
        result.hops = 0;
        return result;
    }

    if (graph.find(start) == graph.end() || graph.find(goal) == graph.end())
        return result;

    // Standard BFS for unweighted shortest path (fewest edges)
    std::queue<int> q;
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int> cameFrom;

    visited[start] = true;
    cameFrom[start] = start;
    q.push(start);

    bool found = false;

    while (!q.empty() && !found)
    {
        int current = q.front();
        q.pop();

        auto itAdj = graph.find(current);
        if (itAdj == graph.end())
            continue;

        for (const auto& [next, edgeCost] : itAdj->second)
        {
            (void)edgeCost; // ignored for BFS hop-count decision

            if (visited[next])
                continue;

            visited[next] = true;
            cameFrom[next] = current;

            if (next == goal)
            {
                found = true;
                break;
            }

            q.push(next);
        }
    }

    if (!found)
        return result;

    // Reconstruct path: goal -> ... -> start
    std::vector<int> reversed;
    for (int cur = goal;; cur = cameFrom[cur])
    {
        reversed.push_back(cur);
        if (cur == start) break;
    }
    std::reverse(reversed.begin(), reversed.end());
    result.path = std::move(reversed);

    // Hops = number of edges in path
    result.hops = (result.path.empty() ? 0 : (int)result.path.size() - 1);

    // Compute actual cost along that path using real edge weights
    float total = 0.0f;
    for (int i = 0; i + 1 < (int)result.path.size(); i++)
    {
        int a = result.path[i];
        int b = result.path[i + 1];

        auto itA = graph.find(a);
        if (itA == graph.end())
        {
            total = 0.0f;
            break;
        }

        bool edgeFound = false;
        for (const auto& [to, w] : itA->second)
        {
            if (to == b)
            {
                total += w;
                edgeFound = true;
                break;
            }
        }

        if (!edgeFound)
        {
            total = 0.0f;
            break;
        }
    }

    result.totalCost = total;
    return result;
}