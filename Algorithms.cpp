// iliyacod@gmail.com 
#include <iostream>
#include "Algorithms.hpp"
#include <stdexcept>
#include <limits.h>
#include "Graph.hpp"

namespace graph {

Graph Algorithms::bfs(const Graph& g, int source) {
    // get the total number of vxs for this graph.
    int V = g.getNumVertices();

    // check if the source(starting point) is indeed valid.
    if (!g.vertex_in_graph(source)) {
        throw std::out_of_range("Source vertex is out of range.");
    }
    // creating a new graph so that it will hold the bfs tree result.
    Graph tree(V);
    // a boolean visited array. this array will keep track of the vertices that
    // we have already visited.
    bool* visited = new bool[V]();

    // a queue(this has the implementation in my graph.hpp file) 
    // it is known from Algorithms 1 that a queue is useful for the bfs algo.
    Queue q;
    visited[source] = true;
    q.push(source);
    // while loop to process all the reachable vxs.
    while (!q.isEmpty()) 
    {
        // get the next vx.
        int u = q.show_top();
        q.pop();
        AdjListNode* temp = g.getAdjListHead(u);

        while (temp) {
            if (!visited[temp->dest]) 
            {
                visited[temp->dest] = true;
                tree.addEdge(u, temp->dest, temp->weight);

                q.push(temp->dest);
            }
            temp = temp->next;
        }
    }
    delete[] visited;
    return tree;
}

Graph Algorithms::dfs(const Graph& g, int source) {
    int V = g.getNumVertices();
    if (!g.vertex_in_graph(source)) 
    {
        throw std::out_of_range("Source vertex is out of range.");
    }
    Graph tree(V);
    bool* visited = new bool[V]();
    dfsHelper(g, tree, visited, source);

    delete[] visited;
    return tree;
}

// Helper function for DFS.
void Algorithms::dfsHelper(const Graph& g, Graph& tree, bool* visited, int u) {
    // mark the current vx true as visited.
    visited[u] = true;
    // get the head of the adjacency list for this vx.
    AdjListNode* temp = g.getAdjListHead(u);
    while (temp) 
    {
        // if the current dest vx is not visited, we will add it to the tree and recursively call dfsHelper on it.
        if (!visited[temp->dest]) 
        {
            tree.addEdge(u, temp->dest, temp->weight);
            dfsHelper(g, tree, visited, temp->dest);
        }
        temp = temp->next; 
    }
}

Graph Algorithms::dijkstra(const Graph& g, int source) {
    // a check to see whether the graph has any negative edges, as dijkstra doesn't allow negative edges and therefore, we retuns an 
    // invalid argument exception.
    if (g.check_negative_edge()) 
    {
        throw std::invalid_argument("Graph contains negative edges, which is not allowed for Dijkstra's algorithm!");
    }
    // i will also check if the source vertex is even valid at first, if so then we can contiune else who throw an out of range exception.
    if (!g.vertex_in_graph(source)) 
    {
        throw std::out_of_range("Source vertex is out of range.");
    }
    // a final check to see if the graph is empty.
    if (g.isEmpty()) {
        throw std::runtime_error("Graph is empty.");
    }

    int V = g.getNumVertices();
    // array to store the shortest distances to each place.
    int* dist = new int[V];
    // boolean array that keeps track of already visited vxs.
    bool* visited = new bool[V]();
    // empty graph to store the results.
    Graph tree(V);
    // pq to make sure we do visit the closest vertex(unvisited) each time.
    PriorityQueue pq;

    // setting the distance to every vertex to infinity.
    for (int i = 0; i < V; ++i)
    {
        dist[i] = INT_MAX;
    }
    // dist from where we start is 0 trivially.
    dist[source] = 0;
    // starting point with dis 0, 
    pq.insert(Edge(-1, source, 0));

    // while loop to process all the reachable vxs.
    while (!pq.isEmpty()) {

        // pq gets the closest (unvisited) vx.
        Edge e = pq.extract_min();
        int u = e.dest_index;
        // we have to make sure this vx isnt already visited, if so we have to skip it.
        if (visited[u])
        {
             continue;
        }
        // after we have found the shortest path to a specific vertex we mark it in the visited arr.
        visited[u] = true;

        if (e.source_index != -1)
        {

            tree.addEdge(e.source_index, e.dest_index, e.weight);
        }

        AdjListNode* n = g.getAdjListHead(u);
        while (n) 
        {

            if (!visited[n->dest]) 
            {
                // relaxation part. as learned in Algorithms 1.
                // since we are using an adjancey list, i got really confused
                // on how you exactly print out the shortest paths in the graph.
                // for example if i have a shortest path from 0 -> 1 -> 3 
                // because adjancey list head is linked with an edge to its neighbours..
                // what i did was adding the weights of the shortest path to the dest vertex
                // so when we reach the destination of some vertex, we look at its 
                // neighbours total edge weight and find the total shortest path from 
                // the source to the destination.
                if (dist[u] + n->weight < dist[n->dest]) 
                {
                    dist[n->dest] = dist[u] + n->weight;
                    pq.insert(Edge(u, n->dest, dist[n->dest]));
                }
            }
            n = n->next;
        }
       
    }
    // freeing the memory to avoid memory leaks.
    // at the start we have created 2 arrays using "new" and 
    // therefore we MUST free it after we finish with it.
    delete[] dist;
    delete[] visited;
    return tree;
}

// kruskal algorithm.
Graph Algorithms::kruskal(const Graph& g) {
    if (g.isEmpty()) {
        throw std::runtime_error("Graph is empty.");
    }

    int V = g.getNumVertices();
    // union find(implementation is located at my Algorithms.hpp file), this 
    // has use as we need to detect cycles, keeping track of the connected groups of vxs.
    union_find uf(V);
    // empty graph for our mst result.
    Graph mst(V);
    // since we need mst we have a priority queue to sort the edges by their weight.
    // this priority queue is implemented at my Algorithms.hpp file.
    PriorityQueue pq;

    for (int u = 0; u < V; ++u) 
    { AdjListNode* n = g.getAdjListHead(u);
        while (n) {
            // we ensure that we dont add the edge multiple times to avoid duplications.
            // since the graph is undirected, we have to keep track of every vertex's
            // destination to ensure we are not adding the same edge...
            if (u < n->dest)
            pq.insert(Edge(u, n->dest, n->weight));
            n = n->next;
        }
    }

    // mst buildup, we get the next shortest edge in the pq, we also have to check whether this edge
    // will create a ctyle and for that we have to check the endpoints. since the idea is
    // as If the endpoints are already in the same connected component, adding this edge would create a cycle
    while (!pq.isEmpty()) {
        Edge e = pq.extract_min();

        if (uf.find(e.source_index) != uf.find(e.dest_index)) 
        {
            uf.unite(e.source_index, e.dest_index);
            mst.addEdge(e.source_index, e.dest_index, e.weight);
        }
    }
    return mst;
}

// Prim algorithm. 
Graph Algorithms::prim(const Graph& g) {
    if (g.isEmpty()) {
        throw std::runtime_error("Graph is empty.");
    }

    int V = g.getNumVertices();

    // a boolean array to keep track of the previously added vertices.
    // we have to make sure to delete its memory afterwards to ensure no memory leaks.
    bool* inMST = new bool[V]();

    // a new empty graph for the mst.
    Graph mst(V);
    // pq to choose the next shortest edge
    PriorityQueue pq;
    // i start with vertex 0 for this mst
    inMST[0] = true;

    // i add all the edges from the starting vertex to the pq.
    AdjListNode* n = g.getAdjListHead(0);
    while (n) {
        pq.insert(Edge(0, n->dest, n->weight));
        n = n->next;
    }

    // here i am adding edges to the mst 
    while (!pq.isEmpty()) 
    {
        // i extarct the shortest edge from the pq to connect with the current mst with a new vertex.
        Edge e = pq.extract_min();

        // i have to check if the endpoints are aldreadfy in the current mst, if so i skip this edge.
        if (inMST[e.dest_index]) 
        {
            continue;
        }
        // edge added to the mst and we have to mark it as  an "added vertex in the tree"
        mst.addEdge(e.source_index, e.dest_index, e.weight);
        inMST[e.dest_index] = true;

            // here we add the edges from the recent added vertex to the pq,
            // this finds new vertices for the next edge to add.
        AdjListNode* n = g.getAdjListHead(e.dest_index);
        while (n) {
            // a check to see if this edge is already in the mst.
            if (!inMST[n->dest]) 
            {
            pq.insert(Edge(e.dest_index, n->dest, n->weight));
            }
            n = n->next;
        }
    }
    // free the memory to avoid memory leaks.
    delete[] inMST;
    return mst;
}

}