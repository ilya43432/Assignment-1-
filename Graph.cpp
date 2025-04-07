// iliyacod@gmail.com
#include <iostream>
#include <stdexcept>
#include "Graph.hpp"
#include "Algorithms.hpp"
    
namespace graph {

    // The following functions are the required functions I had to implement for this Assignement, as well some helper functions
    // that would be useful for several algorithms.

    // required function - edge addition to a graph that is based on our adjancey list struct implementation i have made.
    // we check if the src and dest vertices are valid, if not we throw an out_of_range exception.
    // if they are valid we call the adjancey list struct function "addEdge" for both the dest and src vertices.
    // since the graph is undirected we have to add the edge to both vertices.

    void Graph::addEdge(int src, int dest, int weight)
    {
        if (!vertex_in_graph(src) || !vertex_in_graph(dest))
        {
            throw std::out_of_range("Invalid vertex index!");
            return;
        }
        // since the graph is undirected we have to add the edge to both vertices.
        // adding the edge to the src vertex.
        adjLists[src].addEdge(dest, weight);
        // adding the edge to the dest vertex.
        adjLists[dest].addEdge(src, weight);
    }

    // required function - a remove edge option that is based on my adjancey list struct i have made.
    // we call the adjancey list struct function "removeEdge" for both the dest and src vertices.
    // this is because the graph is undirected and therefore we have to remove the edge from both vertices.
    // if the edge does not exist or one of the vertices (src/dest) isnt valid we throw a runtime error.
    void Graph::removeEdge(int src, int dest)
    {
        if (!vertex_in_graph(src) || !vertex_in_graph(dest))
        {
            throw std::out_of_range("Invalid vertex index!");
            return;
        }

        // if both edges are removed and return true then we 
        // know that the edge was removed.
        bool removed_vertexSrc = adjLists[src].removeEdge(dest);
        bool removed_vertexDest = adjLists[dest].removeEdge(src);

        if (!removed_vertexSrc && !removed_vertexDest)
        {
            throw std::runtime_error("This edge does not exist!");
        }
    }

    // required function - Graph printing, nothing complex to explain here, a simple and logical way of
    // showcasing how the graph looks. of course the graph is an adjancey list graph.
    void Graph::print_graph() const
    {
        for (int i = 0; i < numVertices; i++)
        {
            // head of the list
            std::cout << "Head vertex No. " << i;
            // print the list of edges for this vertex.
            adjLists[i].printList();
        }
    }

    // returns the number of vertices that was allocated to the graph. this size cannot be changed after the creation of the graph.
    // adhereing to the assignment requirements.
    int Graph::getNumVertices() const
    {
        return numVertices;
    }

    // func that returns a boolean that states whether the graph is empty or not, we check if the adjancey list has any head vertices,
    // if not then we return false, else true.
    bool Graph::isEmpty() const
    {
        for (int i = 0; i < numVertices; ++i)
        {
            // if any of the lists are not empty, return false
            // otherwise, return true.
            if (adjLists[i].getHead() != nullptr)
            {
                return false;
            }
        }
        return true;
    }
    // this function returns the head of the adjancey list of a vertex.
    AdjListNode *Graph::getAdjListHead(int vertex) const
    {
        if (!vertex_in_graph(vertex))
        {
            throw std::out_of_range("Invalid vertex index!");
            return nullptr;
        }
        return adjLists[vertex].getHead();
    }

    // this function returns the adjancey list of a vertex.
    const AdjList &Graph::getAdjList(int vertex) const
    {
        if (!vertex_in_graph(vertex))
        {
            throw std::out_of_range("Invalid vertex index!");
        }
        return adjLists[vertex];
    }

    // this function checks whether the graph is connected or not.
    // of course we will use this check in our kruskal and prim algorithms,
    // since they require the graph to be connected.
    bool Graph::check_connected() const
    {
        AdjListNode *head = getAdjListHead(0);
        if (head == nullptr)
        {
            return false; // No edges in the graph
        }
        bool *visited = new bool[getNumVertices()]();
        Queue q;
        q.push(0);
        visited[0] = true;

        while (!q.isEmpty())
        {
            int u = q.show_top();
            q.pop();
            AdjListNode *temp = getAdjListHead(u);
            while (temp)
            {
                if (!visited[temp->dest])
                {
                    visited[temp->dest] = true;
                    q.push(temp->dest);
                }
                temp = temp->next;
            }
        }

        for (int i = 0; i < getNumVertices(); i++)
        {
            if (!visited[i])
            {
                delete[] visited; // free the memory is we find an unreachable vertex, avoiding memory leak.
                return false;     // Not all vertices are reachable
            }
        }
        // if we reach here, we know the graph is connected and we safely free the memory.
        delete[] visited;
        return true;
    }

    // this function checks if the graph has any negative edges (for dijkstra).
    bool Graph::check_negative_edge() const
    {
        // used a bfs variation for this
        int V = getNumVertices();
        bool *visited = new bool[V]();
        bool ans = false;
        Queue q;
        visited[0] = true;
        q.push(0);

        while (!q.isEmpty() && !ans)
        {
            int u = q.show_top();
            q.pop();
            AdjListNode *temp = getAdjListHead(u);
            while (temp && !ans)
            {
                if (temp->weight < 0) // if we find an edge that is below weight 0
                // then we can break and return true
                {
                    ans = true;
                    break;
                }

                if (!visited[temp->dest])
                {
                    visited[temp->dest] = true;
                    q.push(temp->dest);
                }
                temp = temp->next;
            }
        }
        delete[] visited;
        return ans;
    }

    // this function counts the total edges of the graph. since its undirected, every edge will be counter twice and
    // therefore to get the precise amount we will divide it by 2.

    int Graph::count_edges() const
    {
        int count = 0;
        for (int i = 0; i < numVertices; ++i)
        {
            AdjListNode *temp = adjLists[i].getHead();
            while (temp)
            {
                count++;
                temp = temp->next;
            }
        }
        return count / 2;
    }
    // this short function checks if a vertex is valid.
    bool Graph::vertex_in_graph(int vertex) const
    {
        return vertex >= 0 && vertex < numVertices;
    }

    // this function calculates the total weight edges of a tree. this function is of course to test the total weight of mst's created
    // by my kruskal and prim algorithms.
    int Graph::calculate_tree_weight(const Graph &tree_Graph)
    {
        int total_weight = 0;
        for (int i = 0; i < tree_Graph.getNumVertices(); i++)
        {
            AdjListNode *temp = tree_Graph.getAdjListHead(i);

            while (temp)
            {
            // i have to check if the edge is already counted or not.
            // since its undirected, graph has 2 edges for every edge in the graph.
            // so i will check if i < temp_node->dest, this way we will only count
            // one edge and not both.
                if (i < temp->dest)
                {
                    total_weight += temp->weight;
                }
                temp = temp->next;
            }
        }
        return total_weight;
    };

}