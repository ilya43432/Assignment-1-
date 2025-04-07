// iliyacod@gmail.com
#include <iostream>
#include "Graph.hpp"
#include "Algorithms.hpp"

int main() 
{
    graph::Graph g(5);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);
    g.addEdge(4, 1, 6);
    g.addEdge(4, 0, 7);
    g.print_graph();
    std::cout << "Graph is connected: " << g.check_connected()<< std::endl;
    std::cout << "Graph has no negative edges " << g.check_negative_edge() << std::endl;
    std::cout << "Graph has " << g.count_edges() << " edges." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "BFS tree:" << std::endl;
    graph::Graph bfs_tree = graph::Algorithms::bfs(g, 0);
    bfs_tree.print_graph();
    std::cout << '\n' << std::endl;
    std::cout << " BFS has " << bfs_tree.count_edges() << " edges." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "DFS tree:" << std::endl;
    graph::Graph dfs_tree = graph::Algorithms::dfs(g, 0);
    dfs_tree.print_graph();
    std::cout << '\n' << std::endl;
    std::cout << " DFS has " << dfs_tree.count_edges() << " edges." << std::endl;

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Dijkstra shortest paths graph:" << std::endl;
    graph::Graph dijkstra_tree = graph::Algorithms::dijkstra(g, 0);
    dijkstra_tree.print_graph();
    std::cout << '\n' << std::endl;
    std::cout << " Dijkstra has " << dijkstra_tree.count_edges() << " edges." << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    std::cout << "Kruskal tree graph:" << std::endl;
    graph::Graph kruskal_tree = graph::Algorithms::kruskal(g);
    kruskal_tree.print_graph();
    std::cout << '\n' << std::endl;
    std::cout << " Kruskal has " << kruskal_tree.count_edges() << " edges." << std::endl;
    std::cout << " Kruskal MST has " << kruskal_tree.calculate_tree_weight(kruskal_tree) << " weight." << std::endl;

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Prim tree graph:" << std::endl;
    graph::Graph prim_tree = graph::Algorithms::prim(g);
    prim_tree.print_graph();
    std::cout << '\n' << std::endl;
    std::cout << " Prim has " << prim_tree.count_edges() << " edges." << std::endl;
    std::cout << " Prim MST has " << prim_tree.calculate_tree_weight(prim_tree) << " weight." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    // here i remove all the edges to demonstrate the graph's functionalities.
    std::cout << "Removing edges..." << std::endl;
    std::cout << "Empty graph:" << std::endl;
    g.removeEdge(0, 1);
    g.removeEdge(0, 2);
    g.removeEdge(1, 2);
    g.removeEdge(2, 3);
    g.removeEdge(3, 4);
    g.removeEdge(4, 1);
    g.removeEdge(4, 0);
    g.print_graph();
    std::cout << " Checking current graph's functionality..." << std::endl;
    std::cout << "current Graph is empty: " << g.isEmpty() << std::endl;
    std::cout << "Graph is connected: " << g.check_connected()<< std::endl;

    std::cout << "adding 1 negative edge to the graph." << std::endl;
    g.addEdge(0, 1, -4);
    std::cout << "The Graph has negative edges: " << g.check_negative_edge() << std::endl;
    std::cout << "The Graph has " << g.count_edges() << " edges." << std::endl;

    std::cout << "Testing graph creation with (new) to test the destructor." << std::endl;
    graph::Graph* g2 = new graph::Graph(5);
    g2->addEdge(0, 1, 4);
    g2->addEdge(0, 2, 1);
    g2->addEdge(1, 2, 2);
    g2->addEdge(2, 3, 5);
    std::cout << "freeing memory..." << std::endl;
    delete g2;
    std::cout << "Memory freed." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    return 0;
}