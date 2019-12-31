/******************************************************************************

Online C++ Compiler.
Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream> 
#include <fstream> 
#include <vector>
#include <stack>
#include <list>
#include <ctime>    // For time()
#include <cstdlib>
using namespace std;
struct vertex
{
	int d, f, color, parent;
};
void addEdge(vector<int> adj[], int u, int v)
{
	bool found = false;
	for (int elem : adj[u])
	{
		if (elem == v)
		{
			found = true;
			break;
		}
	}
	if (!found)
		adj[u].push_back(v);

}
bool IsReachable(vector<int> adj[], vertex*vertices, int V, int source, int destination)
{
	for (int i = 0; i < V; i++)
	{
		vertices[i].color = 0; // 0 for unvisited
	}
	list<int> queue;
	vertices[source].color = 1; // 1 for visited
	queue.push_back(source);

	if (source == destination)
	{
		return true;
	}

	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it 
		source = queue.front();
		queue.pop_front();
		if (source == destination)
		{
			return true;
		}
		int n = adj[source].size();
		for (int i = 0; i < n;)
		{
			int x = adj[source][i];
			i++;
			if (vertices[x].color == 0)
			{
				vertices[x].parent = source;
				vertices[x].color = 1; //1 for visited
				queue.push_back(x);
				i = 0;
				n = adj[source].size();
			}
		}
	}
	return false;
}

int main()
{
	ifstream fin;
	fin.open("epinions-network.txt");
	ofstream fout("outputQ2graph.txt");

	const long long int V = 75889; //size+1
	vector<int> adj[V];

	int tail, head;
	while (!fin.eof())
	{
		fin >> tail;
		fin >> head;
		addEdge(adj, tail, head);
	}

	vertex*vertices = new vertex[V];
	int Total = 10240, Reachablecounter = 0;
	srand(time(0));
	for (int i = 0; i<11; i++)
	{

		for (int j = 0; j<Total; j++)
		{
			
 			int source = (rand() % 75888);
			int destination = (rand() % 75888);
			bool r = IsReachable(adj, vertices, V, source, destination);
			if (r == true)
			{
				Reachablecounter++;
			}
		}
		fout << "(" << Total << "," << Reachablecounter << ")\n\n";
		Total = Total + Total;
		Reachablecounter = 0;
	}

	fout.close();
	system("pause");
	return 0;
}
