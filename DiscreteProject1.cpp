#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;
template <typename data>
struct Vertex {
	data Data;
	Vertex* Next;

	Vertex(const data val = 0, Vertex<data>* nex = nullptr)
	{
		Data = val;
		Next = nex;
	}
};
template <typename data>
class Stack {
public:
	Vertex<data>* Top;

	Stack();
	void Pop();
	bool IsEmpty();
	const data Peek();
	const data PopR();
	void DisplayStack();
	void Push(const data);
	const Vertex<data>* PeekV();
	~Stack();
};
template<typename data>
inline Stack<data>::Stack()
{
	Top = nullptr;
}
template<typename data>
inline void Stack<data>::Push(const data val)
{
	Vertex<data>* temp = new Vertex<data>(val, Top);
	Top = temp;
}
template<typename data>
inline void Stack<data>::Pop()
{
	if (IsEmpty())
		return;
	Vertex<data>* temp = Top;
	Top = Top->Next;
	delete temp;
}
template<typename data>
inline const data Stack<data>::PopR()
{
	const data ret = Peek();
	Pop();
	return ret;
}
template<typename data>
inline bool Stack<data>::IsEmpty()
{
	return Top == nullptr;
}
template<typename data>
inline const Vertex<data>* Stack<data>::PeekV()
{
	return Top;
}
template<typename data>
inline const data Stack<data>::Peek()
{
	if (!IsEmpty())
		return Top->Data;
	return NULL;
}
template<typename data>
inline void Stack<data>::DisplayStack()
{
	Vertex <data>* temp = Top;
	while (temp != nullptr)
	{
		std::cout << temp->Data << "  ";
		temp = temp->Next;
	}
}
template<typename data>
inline Stack<data>::~Stack()
{
	Vertex<data>* temp;

	while (Top != nullptr)
	{
		temp = Top;
		Top = Top->Next;
		delete temp;
	}
}
const long long ConvertToNum(string& str)
{
	long long res = 0;// = stoi(str);
	for (long long i = 0; i < str.length(); i++)
		res = (res * 10) + (str[i] - 48);
	str = "";
	return res;
}
bool IsNumber(const char ch)
{
	return (ch >= '0' && ch <= '9');
}
void SeperateNodes(string str, long long& node1, long long& node2)
{
	bool num1 = false;
	string num;
	for (long long i = 0; str[i] != '\0'; i++)
	{
		if (IsNumber(str[i]))
			num += str[i];
		else if (num1 == false && (str[i] == '\t' || str[i] == ' '))
		{
			node1 = ConvertToNum(num);
			num1 = true;
		}
	}
	node2 = ConvertToNum(num);
}
class Node {
public:
	bool status = 0;
	bool visited = 0;
	vector <long long> In;
	vector <long long> Out;
};
class graph {
public:
	Node** ArrNodes;
	unordered_set<long long> nodespresent;

	long long MaxIndex;
	long long TotalActiveNodes;
	long long TotalJumps = 0;
	long long TotalVisited = 0;
	graph(graph& copy) {
		MaxIndex = copy.MaxIndex;
		TotalActiveNodes = copy.TotalActiveNodes;
		TotalJumps = copy.TotalJumps;
		TotalVisited = copy.TotalVisited;
		nodespresent = copy.nodespresent;
		Node empty;
		ArrNodes = new Node * [MaxIndex];
		for (long long const& i : nodespresent) {
			ArrNodes[i] = new Node;
			ArrNodes[i][0].In = copy.ArrNodes[i][0].In;
			ArrNodes[i][0].Out = copy.ArrNodes[i][0].Out;
			ArrNodes[i][0].status = copy.ArrNodes[i][0].status;
			ArrNodes[i][0].visited = copy.ArrNodes[i][0].visited;

		}
	}
	graph(string filedest) {
		long long x;
		ifstream myfile;
		string line;
		long long max = 0;
		vector<long long>FromJumps;
		vector<long long>ToJumps;
		//long long* FromJumps = new long long[200000000]{0};
		//long long* ToJumps = new long long[200000000]{0};
		myfile.open(filedest, ios::in);
		//for (long long x = 0; x < 5; x++) {
		//	getline(myfile, line);
		//}
		long long JumpNum = 0;
		if (myfile.is_open())
		{
			while (!myfile.eof())
			{

				getline(myfile, line);
				if (line[0] > '9' || line[0] < '0') {
					//cout << line << endl;
					continue;
				}
				FromJumps.push_back(0);
				ToJumps.push_back(0);
				SeperateNodes(line, FromJumps[JumpNum], ToJumps[JumpNum]);
				nodespresent.insert(FromJumps[JumpNum]);
				nodespresent.insert(ToJumps[JumpNum]);
				JumpNum++;
			}
			myfile.close();
		}
		else {
			cout << "[UNABLE TO OPEN FILE]";
			exit(0);
		}
		TotalJumps = JumpNum;


		TotalActiveNodes = nodespresent.size();
		for (unsigned i = 0; i < nodespresent.bucket_count(); ++i)
			for (auto local_it = nodespresent.begin(i); local_it != nodespresent.end(i); ++local_it)
				if (MaxIndex < *local_it) MaxIndex = *local_it;
		MaxIndex++;
		ArrNodes = new Node * [MaxIndex];
		for (unsigned i = 0; i < nodespresent.bucket_count(); ++i)
			for (auto local_it = nodespresent.begin(i); local_it != nodespresent.end(i); ++local_it)
				ArrNodes[*local_it] = new Node;


		for (JumpNum = 0; JumpNum < TotalJumps; JumpNum++)
		{
			ArrNodes[FromJumps[JumpNum]][0].status = ArrNodes[ToJumps[JumpNum]][0].status = 1;
			ArrNodes[FromJumps[JumpNum]][0].Out.push_back(ToJumps[JumpNum]);
			ArrNodes[ToJumps[JumpNum]][0].In.push_back(FromJumps[JumpNum]);
		}
	}


	void Transpose() {
		for (long long const& i : nodespresent) {
			vector<long long> temp;
			temp = ArrNodes[i][0].In;
			ArrNodes[i][0].In = ArrNodes[i][0].Out;
			ArrNodes[i][0].Out = temp;

		}
	}
	void IterativeDFS(const long long& Centre, Stack<long long>& IdStack)
	{
		long long curr;
		vector<long long> OutNodes;
		Stack<long long> TStack;
		TStack.Push(Centre);
		while (!TStack.IsEmpty())
		{
			curr = TStack.PopR();
			if (!ArrNodes[curr][0].visited)
			{
				ArrNodes[curr][0].visited = 1;
				TotalVisited++;
				TStack.Push(curr);
				IdStack.Push(curr);
			}
			OutNodes = ArrNodes[curr][0].Out;
			for (long long i = 0; i < OutNodes.size(); i++)
				if (!ArrNodes[OutNodes[i]][0].visited)
					TStack.Push(OutNodes[i]);
		}
	}
	long long IDFS(long long& Start, Stack<long long>& IdStack)
	{
		vector<long long> Nodes;
		long long MaxSize = 0, temp = 0, prev = 0;
		for (unsigned i = 0; i < nodespresent.bucket_count(); ++i) {
			for (auto local_it = nodespresent.begin(i); local_it != nodespresent.end(i); ++local_it)
				Nodes.push_back(*local_it);
		}
		//GetNIdV(Nodes);
		if (Start == -1) Start = Nodes[0];
		for (long long i = 0; temp < Nodes.size() - 1; i++)
		{
			prev = temp;
			if (!ArrNodes[Start][0].visited)
			{
				IterativeDFS(Start, IdStack);
				temp = TotalVisited;
				if (temp - prev > MaxSize)
					MaxSize = temp - prev;
			}
			Start = Nodes[i];
		}
		return MaxSize;
	}
	void IterativeStackDFS(const long long& Id, Stack<long long>& S)
	{
		long long curr;
		vector<long long> OutNodes;
		Stack<long long> TStack;

		TStack.Push(Id);
		while (!TStack.IsEmpty())
		{
			curr = TStack.PopR();
			if (!ArrNodes[curr][0].visited)
			{
				TStack.Push(curr);
				ArrNodes[curr][0].visited = 1;
				TotalVisited++;
			}
			OutNodes = ArrNodes[curr][0].Out;
			for (long long i = 0; i < OutNodes.size(); i++)
				if (!ArrNodes[OutNodes[i]][0].visited)
					TStack.Push(OutNodes[i]);
		}
	}
	long long StackIDFS(Stack<long long>& S)
	{
		vector<long long> Nodes;

		for (unsigned i = 0; i < nodespresent.bucket_count(); ++i) {
			for (auto local_it = nodespresent.begin(i); local_it != nodespresent.end(i); ++local_it)
				Nodes.push_back(*local_it);
		}
		//GetNIdV(Nodes);
		long long MaxSize = 0, temp = 0, prev = 0;

		while (temp < Nodes.size() - 1 && !S.IsEmpty())
		{
			prev = temp;
			while (ArrNodes[S.Peek()][0].visited)
			{
				S.Pop();
				if (S.IsEmpty())
					return MaxSize-1;
			}

			IterativeStackDFS(S.PopR(), S);

			temp = TotalVisited;
			if (temp - prev > MaxSize)
				MaxSize = temp - prev;
		}
		return MaxSize-1;
	}
};
void MaxSCComponent(graph& ActualG)
{
	long long Start = -1;
	Stack<long long> IDStack;
	graph a21(ActualG);
	cout << "[DUPLICATE GRAPH CREATED]\n";
	a21.Transpose();
	cout << "[DUPLICATE GRAPH TRANSPOSE PROCESSED]\n";
	const long long MaxSCCA = ActualG.IDFS(Start, IDStack);
	cout << "[SSC OF ACTUAL GRAPH PROCESSED]\n";
	cout << "[IDSTACK CREATED]\n";
	const long long MaxSCCT = a21.StackIDFS(IDStack);
	cout << "[SSC OF TRANSPOSE GRAPH PROCESSED]\n";
	cout << "[COMPUTING FINAL RESULTS]\n";
	cout << "-------------------------\n\n";
	cout << "Total Nodes  :\t\t" << ActualG.TotalActiveNodes << endl;
	cout << "Total Edges  :\t\t" << ActualG.TotalJumps << endl;
	cout << "Nodes In Largest SCC :\t" << MaxSCCT << endl << endl;
	cout << "---------------\n";
	cout << "[PROGRAM ENDED]\n";
	cout << "---------------\n";

}
int main() {
	string graphdest = "C:/Users/hamza/Desktop/testgraph/testgraph/examples/as20graph.txt";
	cout << "-----------------\n";
	cout << "[PROGRAM STARTED]\n";
	cout << "[READING GRAPH FROM FILE]\n";
	graph a20(graphdest);
	cout << "[GRAPH FROM FILE PROCESSED]\n";
	MaxSCComponent(a20);
	return 0;
}