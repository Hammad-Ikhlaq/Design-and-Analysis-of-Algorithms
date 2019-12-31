#include <iostream> 
#include <fstream> 
#include <vector>
using namespace std; 

int time = 0,ComponentCounter=0, ElementsInComponentCounter = 0;
struct vertex 
{
	int d,f,color,parent;
};
void addEdge(vector<int> adj[], int u, int v)
{
	adj[u].push_back(v);
}

void DFS_Visit(vector<int> adj[], int v, vertex*vertices)
{
	vertices[v].d = time++;
	vertices[v].color = 1; //1 for grey

	for (int x : adj[v])
	{
		if (vertices[x].color == 0)
		{
			vertices[x].parent = v;
			DFS_Visit(adj, x, vertices);
		}
	}
	vertices[v].color = 2;//2 for black
	vertices[v].f = time++;
}

void DFS(vector<int> adj[],vertex*vertices, int V)
{

	for (int i = 1; i < V; i++)
	{
		vertices[i].color = 0; //0 for white
		vertices[i].parent = -1;
	}
	for (int i = 1; i < V; i++)
	{
		if (vertices[i].color == 0)
		{
			DFS_Visit(adj,i,vertices);
		}

	}
}
void SCC_DFS_Visit(vector<int> adj[], int v, vertex*vertices)
{
	vertices[v].d = time++;
	vertices[v].color = 1; //1 for grey

	for (int x : adj[v])
	{
		if (vertices[x].color == 0)
		{
			vertices[x].parent = v;
			SCC_DFS_Visit(adj, x, vertices);
		}
	}
	vertices[v].color = 2;//2 for black
	vertices[v].f = time++;
	cout << v <<",";
	ElementsInComponentCounter++;
}

int maxf(vertex*vertices,int V)
{
	int maxInd = 0;
	vertices[0].f = -1;
	for (int i = 1; i < V; i++)
	{
		if ((vertices[maxInd].f < vertices[i].f)&& (vertices[i].color==0))
		{
			maxInd = i;
		}
	}
	return maxInd;
}

void SCC_DFS(vector<int> adj[], vertex*vertices, int V)
{

	for (int i = 0; i < V; i++)
	{
		vertices[i].color = 0; //0 for white
		vertices[i].parent = -1;
	}

	while (true)
	{
		int indexOfMax = maxf(vertices, V);
		if (indexOfMax == 0)
			return;
		SCC_DFS_Visit(adj, indexOfMax, vertices);
		cout << "count=" << ElementsInComponentCounter;
		ElementsInComponentCounter = 0;
		cout << endl;
		ComponentCounter++;
	}
}
int main()
{ 

	ifstream fin;
	fin.open("sample.txt");

	const int V = 9;
	vector<int> adj[V];
	vector<int> adjComplement[V];

	int tail,head;
	while (fin) 
	{
		fin >> tail;
		fin >> head;
		addEdge(adj, tail, head);
		addEdge(adjComplement, head, tail);
	}

	vertex*vertices = new vertex[V];

	DFS(adj, vertices,V);
//Now d and f values are calculated

	for (int i = 1; i < V; i++)
	{
		cout << vertices[i].d<<" ";
		cout << vertices[i].f<<endl;
	}

	SCC_DFS(adjComplement,vertices, V);
	cout <<"Total number of components "<< ComponentCounter<<endl;

	fin.close();
	system("pause");
	return 0;
}