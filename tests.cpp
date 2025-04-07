// iliyacod@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "Graph.hpp"
#include "Algorithms.hpp"
#include "doctest.h"

TEST_CASE("Basic graph creation tests.")
{
    graph::Graph g(5);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);

    CHECK_FALSE(g.isEmpty());       // this should return false.
    CHECK(g.getNumVertices() == 5); // we got 5 vertices.
    CHECK_EQ(g.getNumVertices(), 5);

    CHECK_EQ(g.getAdjList(0).getHead()->dest, 2);
    CHECK_EQ(g.getAdjList(0).getHead()->weight, 1);

    CHECK(g.check_connected()); // should return true.

    // Remove edge (0, 1) and check the new head of the adjacency list.
    g.removeEdge(0, 1);
    CHECK_EQ(g.getAdjList(0).getHead()->dest, 2);   // should now return 2.
    CHECK_EQ(g.getAdjList(0).getHead()->weight, 1); // should now return 1.
    CHECK(g.check_connected()); // graph is still connected.
}

TEST_CASE("Graph edge cases tests.")
{
    graph::Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 5, 6);
    g.addEdge(5, 0, 7);

    // in here i will be showing the graph's capabillities to handle edge cases.

    // adding an edge that is beyond the graph's range.
    CHECK_THROWS_AS(g.addEdge(0, 6, 4), std::out_of_range);

    // removing an edge that doesn't exit in the graph.
    CHECK_THROWS_AS(g.removeEdge(4, 8), std::out_of_range);
    CHECK_THROWS_AS(g.removeEdge(2, 5), std::runtime_error);

    // attempting to get the list of vertex that is out of range.
    CHECK_THROWS_AS(g.getAdjList(7), std::out_of_range);

    // head list of the vertex that isnt in range.
    CHECK_THROWS_AS(g.getAdjListHead(9), std::out_of_range);
}

TEST_CASE("BFS and DFS tests.")
{
    graph::Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 5, 6);
    g.addEdge(5, 0, 7);

    CHECK(g.check_connected()); // should return true.
    CHECK_EQ(g.getNumVertices(), 6);

    graph::Graph bfs_tree = graph::Algorithms::bfs(g, 0);
    graph::Graph dfs_tree = graph::Algorithms::dfs(g, 0);

    CHECK(bfs_tree.check_connected());

    CHECK_EQ(bfs_tree.count_edges(), 5);

    CHECK(dfs_tree.check_connected());

    CHECK_EQ(dfs_tree.count_edges(), 5);

    // traverseing through head vertex one in the bfs tree
    // and checking the destination vertex and weight of the edge.
    graph::AdjListNode *bfs_head = bfs_tree.getAdjListHead(0);
    CHECK_EQ(bfs_head->dest, 1);
    CHECK_EQ(bfs_head->weight, 4);
    bfs_head = bfs_head->next;
    CHECK_EQ(bfs_head->dest, 2);
    CHECK_EQ(bfs_head->weight, 1);
    bfs_head = bfs_head->next;
    CHECK_EQ(bfs_head->dest, 5);
    CHECK_EQ(bfs_head->weight, 7);

    // same for dfs tree, this is head vertex 0.
    graph::AdjListNode *dfs_head = dfs_tree.getAdjListHead(0);
    CHECK_EQ(dfs_head->dest, 5);
    CHECK_EQ(dfs_head->weight, 7);

    // traverseing through head vertex 5 in the dfs tree
    graph::AdjListNode *dfs_head2 = dfs_tree.getAdjListHead(5);
    CHECK_EQ(dfs_head2->dest, 4);
    CHECK_EQ(dfs_head2->weight, 6);
    dfs_head2 = dfs_head2->next;
    CHECK_EQ(dfs_head2->dest, 0);
    CHECK_EQ(dfs_head2->weight, 7);
}

TEST_CASE("Dijkstra algorithm verification with adjacency lists")
{
    graph::Graph g(4);
    g.addEdge(0, 1, 2); // Edge from 0 to 1 with weight 2
    g.addEdge(0, 2, 4); // Edge from 0 to 2 with weight 4
    g.addEdge(1, 2, 1); // Edge from 1 to 2 with weight 1
    g.addEdge(1, 3, 5); // Edge from 1 to 3 with weight 5
    g.addEdge(2, 3, 3); // Edge from 2 to 3 with weight 3

    // here i want to check the shortest path from 0 to 3, and as you can tell the cost of the shortest path is 6.
    // the path will be from the source 0 to vx 1
    // 1 to 2 and 2 to 3. the total weight will be 2 + 1 + 3 = 6.

    // firsly we do some basic checks..
    // Verify the graph is properly connected
    CHECK(g.check_connected());
    CHECK_EQ(g.getNumVertices(), 4);
    CHECK_FALSE(g.check_negative_edge());// graph has no negative edges
    CHECK_FALSE(g.isEmpty()); // graph isnt empty
    CHECK_EQ(g.count_edges(), 5);// 5 edges so far

    // Test Dijkstra's algorithm
    graph::Graph dijkstra_tree = graph::Algorithms::dijkstra(g, 0);
    CHECK_EQ(dijkstra_tree.count_edges(), 3);
    CHECK(dijkstra_tree.check_connected());      // should return true.
    CHECK_EQ(dijkstra_tree.getNumVertices(), 4); // should return 4.

    // Calculate shortest path weights
    int total_weight_path = dijkstra_tree.getAdjListHead(3)->weight;

    // Expected weight: 0->1 (weight 2) + 1->2 (weight of 1) + 2->3 (weight of 3) = 6
    CHECK_EQ(total_weight_path, 6);

    graph::Graph g2(6);
    g2.addEdge(0, 1, 2);
    g2.addEdge(1, 2, -2);

    // Diksjtra's algorithm should throw an exception here since we have a negative edge.
    CHECK_THROWS_AS(graph::Algorithms::dijkstra(g2, 0), std::invalid_argument);

    g2.removeEdge(1, 2);
    g2.addEdge(1, 2, 3);
    g2.addEdge(3, 4, 2);

    graph::Graph dijkstra_tree2 = graph::Algorithms::dijkstra(g2, 0);

    CHECK_EQ(dijkstra_tree2.count_edges(), 2);

    CHECK_FALSE(dijkstra_tree2.check_connected()); // this should return false since 3 and 4 are not connected to the rest
    // of the graph.
    CHECK_EQ(dijkstra_tree2.getNumVertices(), 6);
    CHECK(dijkstra_tree2.getAdjListHead(3) == nullptr); // no path from 0 to 3 and therefore the
    // head of the adjacency list should be null. This should be the same for vertex 4.
    CHECK(dijkstra_tree2.getAdjListHead(4) == nullptr); // no path from 0 to 4 and therefore the

    g2.addEdge(0, 3, 1);
    // shortest path from 0 to 4 is 0 -> 3 -> 4 with weight 3.
    graph::Graph dijkstra_tree3 = graph::Algorithms::dijkstra(g2, 0);
}

TEST_CASE("Kruskal's algorithm functionallity and edge cases tests.")
{
    graph::Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 5, 6);
    g.addEdge(5, 0, 7);

    CHECK_FALSE(g.isEmpty());
    CHECK(g.check_connected());           // graph should be connected.
    CHECK_EQ(g.getNumVertices(), 6);      // graph created with 6 vertices.
    CHECK_FALSE(g.check_negative_edge()); // no negative edges in the graph.

    graph::Graph kruskal_tree = graph::Algorithms::kruskal(g);
    CHECK_EQ(kruskal_tree.count_edges(), ((2 * kruskal_tree.getNumVertices() - 1)) / 2);

    CHECK(kruskal_tree.check_connected());
    CHECK_EQ(graph::Graph::calculate_tree_weight(kruskal_tree), 17); // total mst weight should be 17.
}

TEST_CASE("Prim's algorithm functionallity and edge cases tests.")
{
    graph::Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 5, 6);
    g.addEdge(5, 0, 7);

    graph::Graph prim_tree = graph::Algorithms::prim(g); // a tree created by the prim algorithm.
    CHECK_FALSE(prim_tree.isEmpty());                    // should return false.
    // a tree has n - 1 edges. our graph is undirected and therfore we will have 2n - 1 edges, so we will need
    // to divide it by 2.
    CHECK_EQ(prim_tree.count_edges(), ((2 * prim_tree.getNumVertices() - 1) / 2));
    CHECK_EQ(graph::Graph::calculate_tree_weight(prim_tree), 17);
    
}

TEST_CASE("Union find functionallity tests")
{
    graph::union_find uf(5);
    // i will be checking if every parent is equal to itself.
    CHECK_EQ(uf.find(0), 0);
    CHECK_EQ(uf.find(1), 1);
    CHECK_EQ(uf.find(2), 2);
    CHECK_EQ(uf.find(3), 3);
    CHECK_EQ(uf.find(4), 4);

    uf.unite(0, 1);
    uf.unite(1, 2);
    uf.unite(3, 4);

    CHECK_EQ(uf.find(0), uf.find(1)); // both should be in the same union.
    CHECK_EQ(uf.find(1), uf.find(2)); // same as this one
    CHECK_EQ(uf.find(3), uf.find(4)); // and this one
}

TEST_CASE("Queue Functionallity tests.")
{
    graph::Queue q;
    CHECK(q.isEmpty());      // should return true.
    CHECK_FALSE(q.isFull()); // should return false.
    q.push(1);
    q.push(2);
    q.push(3);
    CHECK_EQ(q.show_top(), 1); // top should be 1.
    q.pop();
    CHECK_EQ(q.show_top(), 2); // top should be 2 now.
    q.pop();
    CHECK_EQ(q.show_top(), 3); // top should be 3 now
    q.pop();
    CHECK(q.isEmpty()); // queue should be empty now.

    CHECK_THROWS_AS(q.pop(), std::underflow_error);      // trying to pop when the queue is empty.
    CHECK_THROWS_AS(q.show_top(), std::underflow_error); // trying to show the top but its empty.

    // since MAX_SIZE is 10, we can only allocate upto 10 numbers.
    for (int i = 0; i < 10; i++)
    {
        q.push(i);
    }
    CHECK(q.isFull());
    CHECK_THROWS_AS(q.push(12), std::overflow_error);

    for (int j = 0; j < 5; j++)
    {
        CHECK_EQ(q.show_top(), j);
        q.pop();
    }
}

TEST_CASE("PriorityQueue Functionallity tests.")
{
    graph::PriorityQueue pq;
    CHECK(pq.isEmpty()); // should return true.

    pq.insert(graph::Edge(1, 2, 3));
    pq.insert(graph::Edge(2, 3, 1));
    pq.insert(graph::Edge(3, 4, 2));
    CHECK_EQ(pq.extract_min().weight, 1); // should return the edge with weight 1.
    CHECK_EQ(pq.extract_min().weight, 2); // should return the edge with weight 2.
    CHECK_EQ(pq.extract_min().weight, 3); // should return the edge with weight 3.
    CHECK(pq.isEmpty());                  // should return true.

    pq.insert(graph::Edge(1, 2, 3));
    pq.insert(graph::Edge(2, 3, 1));
    pq.insert(graph::Edge(3, 4, 2));

    CHECK_EQ(pq.extract_min().weight, 1); // should return the edge with weight 1.
    CHECK_EQ(pq.extract_min().weight, 2); // should return the edge with weight 2.
    CHECK_EQ(pq.extract_min().weight, 3); // should return the edge with weight 3.
    CHECK(pq.isEmpty());                  // should return true.
}

TEST_CASE("Adjancey List Functionallity")
{
    graph::AdjList adjList;
    adjList.addEdge(1, 2);
    adjList.addEdge(2, 3);
    adjList.addEdge(3, 4);

    adjList.printList();
    CHECK_EQ(adjList.getHead()->dest, 3);   // should return 3.
    CHECK_EQ(adjList.getHead()->weight, 4); // should return 4.

    adjList.removeEdge(2);
    CHECK_EQ(adjList.getHead()->dest, 3);   // should return 3.
    CHECK_EQ(adjList.getHead()->weight, 4); // should return 4.

    adjList.printList();
}