// iliyacod@gmail.com
#pragma once 

#include <iostream>
#include <stdexcept>
#include "Graph.hpp"

// a fixed size for the graph. this is a part of the assignment requirements.
// this size cannot be changed after the creation of the graph.
#define MAX_SIZE 10

namespace graph {

class Algorithms {
public:
    // if every function in this class will return a graph or a tree,
    // i might as well make all the functions static.
    // this way i can call them without creating an instance of the class.
    // this will act as a utility class for the algorithms.

    static Graph bfs(const Graph& g, int source);
    static Graph dfs(const Graph& g, int source);
    static Graph dijkstra(const Graph& g, int source);
    static Graph kruskal(const Graph& g);
    static Graph prim(const Graph& g);

private:
    /// helper function for DFS to recursively visit nodes.
    static void dfsHelper(const Graph& g, Graph& tree, bool* visited, int u);
};

// Queue implementation for bfs
class Queue {
private:
    int front; // first current index of the queue
    int rear; // last current index of the queue
    int arr[MAX_SIZE];
    int current_size;
public:
    // constructor
    Queue() : front(-1), rear(-1), current_size(0) {}

    bool isEmpty() const 
    { 
        return current_size == 0; 
    }

    bool isFull() const 
    { 
        return current_size == MAX_SIZE; 
    }

    int show_top() const 
    {
        if (isEmpty()) 
        {
            throw std::underflow_error("Queue is empty!");
            return -1;
        }
        return arr[front];
    }

    void pop() 
    {
        if (isEmpty()) 
        {
            throw std::underflow_error("Queue is empty!");
            return;
        }
        front = (front + 1) % MAX_SIZE;
        current_size--;

        if (current_size == 0) 
        {
            front = rear = -1;
        }
    }

    void push(int data) 
    {
        if (isFull()) 
        {
            throw std::overflow_error("Queue is full!");
            return;
        }

        if (isEmpty()) front = 0;
        rear = (rear + 1) % MAX_SIZE;
        arr[rear] = data;
        current_size++;
    }
};

// PriorityQueue implementation.
class PriorityQueue {
private:
    Edge arr[MAX_SIZE];
    int currentSize;
public:
    PriorityQueue() : currentSize(0) {}
    bool isEmpty() const 
    { 
        return currentSize == 0; 
    }

    void insert(const Edge& e) 
    {
        if (currentSize >= MAX_SIZE) 
        {
            throw std::overflow_error("PriorityQueue is full!");
            return;
        }
        arr[currentSize++] = e;
    }

    Edge extract_min() 
    {
        if (isEmpty()) 
        {
            throw std::underflow_error("PriorityQueue is empty!");
            return Edge(-1, -1, -1);
        }
        int minIndex = 0;
        for (int i = 1; i < currentSize; ++i) 
        {
            if (arr[i].weight < arr[minIndex].weight) 
            {
                minIndex = i;
            }
        }
        Edge minEdge = arr[minIndex];
        for (int i = minIndex; i < currentSize - 1; ++i) 
        {
            arr[i] = arr[i + 1];
        }
        currentSize--;
        return minEdge;
    }
};

// manual union-find data structure.
class union_find {
private:
// a fixed sized array to track the parent
// for each vertex in the graph.
    int parent[MAX_SIZE];
public:
    // constructor (no need for destructor since im not using any dynamic memory...)
    union_find(int size) {
        for (int i = 0; i < size; ++i)
        {
            // we set the parent of each vertex to himself
            parent[i] = i;
        }
    }
    int find(int x) 
    // we find the parent of the vertex x.
    // i have to check if x is its own parent
    // if not, i have to keep going up the array
    // until i find the parent.
    {
        while (x != parent[x]) 
        {
            x = parent[x];
        }
        // return the parent
        return x; 
    }
    void unite(int x, int y) 
    // function to unite two vertices to the same set.
    {   // we find the parent of each vertex.
        // if they are not in the same set, we set the parent of x to y.
        int rootX = find(x); 
        int rootY = find(y);
        if (rootX != rootY) 
        {
            parent[rootX] = rootY;
        }
    }
};

}