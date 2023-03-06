#include<iostream>
#include<fstream>
#include<unordered_map>
#define MAX 100 // maximum height of huffman tree
using namespace std;
unordered_map<char, string>CodeMap;
struct Node
{
    char data;
    int freq;
    Node *left, *right;
};

struct MinHeap
{
    unsigned int size;
    unsigned int capacity;
    Node** array;  //dynamic array of Node pointers where each pointer points to a Node structure. Node* start=new Node; start->data=.... and so on
    // Node* array; // dynamic array of Node structures !  Node start; start.data=.... start.freq=...
};

Node* create_node(char ch, int freq)
{
    static bool is_root = true;
    Node *new_node=new Node;
    new_node->data=ch;
    new_node->freq=freq;
    new_node->left=new_node->right=NULL;
    return new_node;
}

MinHeap* create_heap(int cap)
{
    MinHeap* heap=new MinHeap;
    heap->size=0;
    heap->capacity=cap;
    heap->array=new Node*[heap->capacity];
    return heap;
}

void swap_Nodes(Node*& node1, Node*& node2) // pass by reference because we want changes to be made in the calling function 
{
    Node* temp=node1;
    node1=node2;
    node2=temp;
}

void Heapify(MinHeap* heap, int i)
{
    int left=2*i+1; // for same index reason a[h]<a[2(h+1)] not a[h]<a[2h]
    int right=2*i+2;  // yup same reason
    int largest=i;
    if (left < heap->size && heap->array[left]->freq < heap->array[largest]->freq) {
        largest = left;
    }
    if (right < heap->size && heap->array[right]->freq < heap->array[largest]->freq) {
        largest = right;
    }
    if(largest!=i){
        swap_Nodes(heap->array[i], heap->array[largest]);
        Heapify(heap, largest);
    }
}

int isSizeOne(struct MinHeap* heap)
{
    return (heap->size == 1);
}

MinHeap* buildminheap(char data[], int freq[], int size)
{
    MinHeap* heap=create_heap(size);
    for(int i=0;i<size;i++)
    {
        heap->array[i]=create_node(data[i], freq[i]);
    }
    heap->size = size;
    for(int i=size/2-1;i>=0;i--)
    {
        Heapify(heap, i);
    }
    return heap;
}


void insertminheap(MinHeap *heap, Node* top)// insert the new node which is the sum of two freq 
{
    heap->size++;
    int i=heap->size-1; // suru ma sum wala node lai last ma add gareko jasto sochni
    while(i && top->freq<heap->array[(i-1)/2]->freq) // dui ta udayepaxi ko last node sanga sum wala node ko freq compare garya to again build a minheap 
    {
        heap->array[i]=heap->array[(i-1)/2]; // last ma add gareko jasto sochya thyo agi tei vayera swap garni
        i=(i-1)/2; // now further check sum node ko rightful place in min heap
    }
    heap->array[i]=top; // now put the sum node in rightful place finally
}

Node* extractmin(MinHeap* heap)
{
    Node* temp=heap->array[0];
    int size=heap->size;
    heap->array[0]=heap->array[size-1];
    heap->size--;
    Heapify(heap, 0);
    return temp;
}

Node* buildHuffmanTree(char carr[], int freq_arr[], int size)
{
    MinHeap* heap=buildminheap(carr, freq_arr, size);
    //until size becomes 1(only one node is left) extract minimum nodes
    while(!isSizeOne(heap))
    {
        Node* left=extractmin(heap);
        Node* right=extractmin(heap);
        // create a new node whose freq is the sum of left->freq and right->freq
        Node* tmp=create_node('#', left->freq+right->freq);
        // make the left and right node children of this node
        tmp->left=left;
        tmp->right=right;
        insertminheap(heap, tmp); // don't worry about children of nodes messing the heap because these nodes are in array not in tree like structure ;)
    }
    // the remaining node is root node and huffman tree is complete so return the root node
    return extractmin(heap);
}

void printArr(int arr[], int n, char key)
{
	int i;
    string value;
	for (i = 0; i < n; ++i)
	{
        cout << arr[i];
        value.append(to_string(arr[i]));
    }
    CodeMap[key]=value;
	cout << "\n";
}

void printCodes(struct Node* root, int arr[],
				int top)
{
	if (root->left) { // Assign 0 to left edge and recur

		arr[top] = 0;
		printCodes(root->left, arr, top + 1);
	}

	if (root->right) { // Assign 1 to right edge and recur

		arr[top] = 1;
		printCodes(root->right, arr, top + 1);
	}

	// If this is a leaf node, then it contains one of the input characters, print the character and its code from arr[]
	if (!root->left && !root->right) {
		cout << root->data << ": ";
        printArr(arr, top, root->data);
	}
}

void Huffman_Code(char carr[], int freq_arr[], int size)
{
    Node* root=buildHuffmanTree(carr, freq_arr, size);
    int arr[MAX], top = 0;   // this arr is to store 0 and 1 patterns
    printCodes(root, arr, top);
}

int main()
{   
    ifstream file;
    file.open("test.txt");
    string str;
    int counter[256] = {0};

    while (getline(file, str)) {
    }
    for (int i = 0; i < str.length(); i++) {
        counter[str[i]]++;
    }
    int count=0;
    int *freq=new int[1];
    char *carr=new char[1];
    for (int i = 0; i < 256; i++) {
        if (counter[i] != 0) {
            carr[count]=(char)i;
            freq[count]=counter[i];
            count++;
        }
    }
    Huffman_Code(carr, freq, count);
    string encodedStr;
    for (const auto& ch : str) 
    {
        auto it = CodeMap.find(ch); // Here 'it' variable points to the element(std::pair object) in the 'CodeMap' that has 'ch' as its key if the .find function doesn't return CodeMap.end()        
        if (it != CodeMap.end())  // if key is not present in map there is no corresponding huffman code either
        {
            encodedStr += it->second;  // Since 'it' points to key-value pair object it->second can access value
        }
    }
    file.close();
    cout<<encodedStr<<endl;
    ofstream outfile;
    outfile.open("compressed_test.txt");
    outfile<<encodedStr;
    outfile.close();
}
