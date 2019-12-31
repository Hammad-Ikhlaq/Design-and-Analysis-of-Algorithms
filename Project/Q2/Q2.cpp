#include <iostream> 
#include <fstream> 
#include <vector>
#include <stack>
#include <list>
using namespace std; 

int time = 0, ElementsInComponentCounter = 0;
stack <int> giantSCC;
struct vertex 
{
	int d,f,color,parent;
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

	for (int i = 0; i < V; i++)
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
	stack <int> s;
	stack <int> currSCC;
	s.push(v);
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
		currSCC.push(v);
		ElementsInComponentCounter++;
	}
	int x;
	if (currSCC.size() > giantSCC.size())
	{
		while (!giantSCC.empty())
			giantSCC.pop();
		while (!currSCC.empty())
		{
			x = currSCC.top();
			giantSCC.push(x);
			currSCC.pop();
		}
	}
}

int maxf(vertex*vertices,int V)
{
	int maxInd = 0;
	vertices[0].f = -1;
	for (int i = 0; i < V; i++)
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
		ElementsInComponentCounter = 0;
	}
}

list<int> BFS_Visit(vector<int> adj[], vertex*vertices, int V, int v)
{
	for (int i = 0; i < V; i++)
	{
		vertices[i].color = 0; // 0 for unvisited
	}
	list<int> queue;
	list<int> FinalQueue;
	vertices[v].color = 1; // 1 for visited
	queue.push_back(v);
	FinalQueue.push_back(v);

	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it 
		v = queue.front();
		queue.pop_front();

		int n = adj[v].size();
		for (int i = 0; i < n;)
		{
			int x = adj[v][i];
			i++;
			if (vertices[x].color == 0)
			{
				vertices[x].parent = v;
				vertices[x].color = 1; //1 for visited
				queue.push_back(x);
				FinalQueue.push_back(x);
				i = 0;
				n = adj[v].size();
			}
		}
	}
	return FinalQueue;
}

int main()
{ 

	ifstream fin;
	fin.open("epinions-network.txt");

	ofstream fout("outputQ2.txt");

	const long long int V = 75889; //size+1
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

	int x;
	int start = giantSCC.top();
	fout << "size of giant SCC is "<< giantSCC.size();
	stack<int> temp;
	temp = giantSCC;

	fout << "\n";

	list<int> OUTplusGiantSCC = BFS_Visit(adj, vertices, V, start);
	int y,flag = 0,n;
	n = OUTplusGiantSCC.size();
	for (int i = 0; i < n; i++)
	{
		x = OUTplusGiantSCC.front();
		OUTplusGiantSCC.pop_front();
		while (!giantSCC.empty())
		{

			y = giantSCC.top();
			if (y == x)
				flag = 1;
			
			giantSCC.pop();
		}
		if (flag == 0)
		{
			OUTplusGiantSCC.push_back(x);
			
		}
		else
		{
			flag = 0;
		}
		giantSCC = temp;
	}
	list<int> OUT = OUTplusGiantSCC;
	fout << "size of OUT is " << OUT.size();

	fout << "\n";

	list<int> INplusGiantSCC = BFS_Visit(adjComplement, vertices, V, start);
	n = INplusGiantSCC.size();
	for (int i = 0; i < n; i++)
	{
		x = INplusGiantSCC.front();
		INplusGiantSCC.pop_front();
		while (!giantSCC.empty())
		{

			y = giantSCC.top();
			if (y == x)
				flag = 1;

			giantSCC.pop();
		}
		if (flag == 0)
		{
			INplusGiantSCC.push_back(x);
			
		}
		else
		{
			flag = 0;
		}
		giantSCC = temp;
	}
	list<int> IN = INplusGiantSCC;
	fout << "size of IN is " << IN.size();

	fout << endl;

	vector<int> UndirectedAdj[V];
	for (int i = 0; i <V; i++)
	{
		UndirectedAdj[i] = adj[i];
	}

	for (int i = 0; i <V; i++)
	{
		for (int j = 0; j < adj[i].size(); j++)
		{
			addEdge(UndirectedAdj, adj[i][j], i);
		}
	}

	start = giantSCC.top();

	list<int> INplusOUTplusGiantSCCplusTendrlis = BFS_Visit(UndirectedAdj, vertices, V, start);

	n = INplusOUTplusGiantSCCplusTendrlis.size();
	for (int i = 0; i < n; i++)
	{
		x = INplusOUTplusGiantSCCplusTendrlis.front();
		INplusOUTplusGiantSCCplusTendrlis.pop_front();
		while (!giantSCC.empty())
		{

			y = giantSCC.top();
			if (y == x)
				flag = 1;

			giantSCC.pop();
		}
		if (flag == 0)
		{
			INplusOUTplusGiantSCCplusTendrlis.push_back(x);
			
		}
		else
		{
			flag = 0;
		}
		giantSCC = temp;
	}

	n = INplusOUTplusGiantSCCplusTendrlis.size();
	list<int> temp2 = OUT;
	for (int i = 0; i < n; i++)
	{
		x = INplusOUTplusGiantSCCplusTendrlis.front();
		INplusOUTplusGiantSCCplusTendrlis.pop_front();
		while (!OUT.empty())
		{

			y = OUT.front();
			if (y == x)
				flag = 1;

			OUT.pop_front();
		}
		if (flag == 0)
		{
			INplusOUTplusGiantSCCplusTendrlis.push_back(x);

		}
		else
		{
			flag = 0;
		}
		OUT = temp2;
	}

	n = INplusOUTplusGiantSCCplusTendrlis.size();
	list<int> temp3 = IN;
	for (int i = 0; i < n; i++)
	{
		x = INplusOUTplusGiantSCCplusTendrlis.front();
		INplusOUTplusGiantSCCplusTendrlis.pop_front();
		while (!IN.empty())
		{

			y = IN.front();
			if (y == x)
				flag = 1;

			IN.pop_front();
		}
		if (flag == 0)
		{
			INplusOUTplusGiantSCCplusTendrlis.push_back(x);

		}
		else
		{
			flag = 0;
		}
		IN = temp3;
	}

	fout << "size of Tendrils+Tubes is " << INplusOUTplusGiantSCCplusTendrlis.size();

	fout << endl;

	fout << "\nsize of Disconnected vertices is " << V-1 - (giantSCC.size()+OUT.size() + IN.size() + INplusOUTplusGiantSCCplusTendrlis.size()) <<endl;

	fin.close();
	system("pause");
	return 0;
}