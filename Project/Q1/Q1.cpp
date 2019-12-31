#include <iostream> 
#include <fstream> 
#include <vector>
#include <stack>
using namespace std; 

ofstream fout("outputQ1.txt");

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
	stack <int> s;
	vertices[v].d = time++;
	vertices[v].color = 1; //1 for grey
	s.push(v);

	while (!s.empty())
	{
		v = s.top();
		int n = adj[v].size();
		for (int i=0;i< n;)
		{
			int x = adj[v][i];
			i++;
			if (vertices[x].color == 0)
			{
				vertices[x].parent = v;
				vertices[x].d = time++;
				vertices[x].color = 1; //1 for grey
				s.push(x);
				v = x;
				i = 0;
				n = adj[v].size();
			}
		}
		vertices[v].color = 2;//2 for black
		vertices[v].f = time++;
		s.pop();
	}
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
	//it will print each component then return
	stack <int> s;
	s.push(v);
	vertices[v].d = time++;
	vertices[v].color = 1; //1 for grey

	while (!s.empty())
	{
		v = s.top();
		int n = adj[v].size();
		for (int i = 0; i< n;)
		{
			int x = adj[v][i];
			i++;
			if (vertices[x].color == 0)
			{
				vertices[x].parent = v;
				vertices[x].color = 1; //1 for grey
				s.push(x);
				v = x;
				i = 0;
				n = adj[v].size();
			}
		}
		vertices[v].color = 2;//2 for black
		s.pop();
		fout << v << ",";
		ElementsInComponentCounter++;
	}
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
		fout << "count=" << ElementsInComponentCounter;
		ElementsInComponentCounter = 0;
		fout << endl;
		ComponentCounter++;
	}
}
int main()
{ 

	ifstream fin;
	fin.open("SCC.txt");

	const long long int V = 875715; //size+_1
	vector<int> adj[V];
	vector<int> adjComplement[V];

	int tail,head;
	while (!fin.eof()) 
	{
		fin >> tail;
		fin >> head;
		addEdge(adj, tail, head);
		addEdge(adjComplement, head, tail);
	}

	vertex*vertices = new vertex[V];

	DFS(adj, vertices,V);

	SCC_DFS(adjComplement,vertices, V);
	fout <<"Total number of components "<< ComponentCounter<<endl;

	fin.close();
	fout.close();
	system("pause");
	return 0;
}