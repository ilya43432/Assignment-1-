// iliyacod@gmail.com
#pragma once
#include <iostream>
#include <stdexcept>

#define DEFAULT_WEIGHT 1 // default weight in case it's not specified.
namespace graph
{

    // This represents a node in our adjacency list
    struct AdjListNode
    {
        int dest;
        int weight;
        AdjListNode *next;

        AdjListNode(int dest, int weight = DEFAULT_WEIGHT)
            : dest(dest), weight(weight), next(nullptr) {}
    };

    struct Edge
    {
        // this struct is used for the priority queue in the algorithms.
        public: // struct fields are public by default... but i will still specify it.
            int weight;
            int source_index;
            int dest_index;
        // default cons
        public:
            Edge() : weight(1), source_index(-1), dest_index(-1) {}

        // Cons
            Edge(int source, int destination, int weight = DEFAULT_WEIGHT) : weight(weight), source_index(source),
            dest_index(destination) {}
    };

    // LinkedList class to represent the adjacency list for a vertex
    class AdjList
    {
    private:
        // ptr to the head of the list
        AdjListNode *head;

    public:
        // constructor for this adjancey list
        AdjList() : head(nullptr) {}

        // destructor for this adjancey list
        ~AdjList()
        {
            // a deletion process to free the memory of the list.
            // we have to traverse the list and delete each node.
            AdjListNode *temp = nullptr;
            while (head)
            {
                temp = head;
                head = head->next;
                delete temp;
            }
        }

        // Insert a new edge into the adjacency list
        void addEdge(int dest, int weight = DEFAULT_WEIGHT)
        {
            // Create new node
            AdjListNode *newNode = new AdjListNode(dest, weight);
            newNode->next = head;
            head = newNode;
        }

        // a remove edge function
        // returns true if the edge is removed successfully.
        // else, returns false
        bool removeEdge(int dest)
        {
            // if the list is empty
            if (!head)
            {
                return false;
            }
            // if the head is the node to remove
            if (head->dest == dest)
            {
                // tmp node will is used to delete the head node.
                AdjListNode *temp = head;
                // head list will be the next node.
                head = head->next;
                delete temp;
                return true;
            }

            // Otherwise, find the node
            AdjListNode *current = head;
            while (current->next && current->next->dest != dest)
            {
                current = current->next;
            }

            // if a node is found
            if (current->next)
            {
                AdjListNode *temp = current->next;
                current->next = temp->next;
                delete temp;
                return true;
            }
            // node isnt found
            return false;
        }

        // prints the adj list
        void printList() const
        {
            AdjListNode *temp = head;
            while (temp)
            {
                std::cout << " -> [" << "Vertex: " << temp->dest << " | Edge weight: " << temp->weight << "]";
                temp = temp->next;
            }
            std::cout << std::endl;
        }

        // getter
        AdjListNode *getHead() const
        {
            return head;
        }
    };

    // Graph class to represent the entire graph
    //  i only imeplemnted the constructor and destructor in here. the rest of the mthods 
    // are in the cpp file...
    class Graph
    {
    private:
        AdjList *adjLists; // Array of adjacency lists
        int numVertices;   // Number of vertices in the graph

    public:
        // Constructor
        Graph(int numVertices) : numVertices(numVertices)
        {
            adjLists = new AdjList[numVertices];
        }

        // Destructor
        // since we are using dynamic allocation for the adjacency lists, we have to free the memory.
        ~Graph()
        {
            delete[] adjLists;
        }

        // method declarations (required & helper functions), all of these are implemented in the Graph.cpp file.
        bool isEmpty() const;
        void addEdge(int src, int dest, int weight = DEFAULT_WEIGHT);
        void removeEdge(int src, int dest);
        void print_graph() const;
        int getNumVertices() const;
        const AdjList &getAdjList(int vertex) const;
        AdjListNode *getAdjListHead(int vertex) const;
        bool check_connected() const;
        bool check_negative_edge() const;
        int count_edges() const;
        bool vertex_in_graph(int vertex) const;
        static int calculate_tree_weight(const Graph &tree_Graph);
    };
};