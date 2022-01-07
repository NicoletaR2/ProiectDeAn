// proiect_de_an.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <limits>
#include <fstream>

using namespace std;
int n;
string str, strTemp;
double entrH = 0;


#define EMPTY_STRING ""

// Nod al arborelui
// Tree node
struct Node
{
    char ch;
    int freq;
    Node* left, * right;
};

// Metoda pentru alocarea memoriei unui nou nod al arborelui
// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Compararea obiectului utilizat pentru ordonarea heap-ului
// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node* l, const Node* r) const
    {
        // Prioritatea i se acorda elementului cu cea mai joasa frecventa 
        // The highest priority item has the lowest frequency
        return l->freq > r->freq;
    }
};

// Metoda pentru verificarea arborelui Huffman daca acesta contine un singur nod 
// Utility function to check if Huffman Tree contains only a single node
bool isLeaf(Node* root)
{
    return root->left == nullptr && root->right == nullptr;
}

// Traversarea arborelui Huffman si stocarea codurilor intr-o mapa
// Traverse the Huffman Tree and store Huffman Codes in a map
void encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
    if (root == nullptr)
    {
        return;
    }

    // gasirea unui nod de sfarsit (care nu mai genereaza alt nod in continuare)
    // found a leaf node
    if (isLeaf(root))
    {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// Traversarea arborelui Huffman si decodificarea sirului codificat
// Traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int& index, string str)
{
    if (root == nullptr)
    {
        return;
    }

    // gasirea unui nod de sfarsit (care nu mai genereaza alt nod in continuare)
    // found a leaf node
    if (isLeaf(root))
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] == '0')
    {
        decode(root->left, index, str);
    }
    else
    {
        decode(root->right, index, str);
    }
}

//Construieste arborele Huffman si decodifica sirul de input introdus 
// Builds Huffman Tree and decodes the given input text
void buildHuffmanTree(string text)
{
    //cazul de baza: sirul gol/ fara caractere
    // base case: empty string
    if (text == EMPTY_STRING)
    {
        return;
    }

    // numara frecventa aparitiei fiecarui caracter
    // count the frequency of appearance of each character

    //si il stocheaza intr-o mapa
    // and store it in a map
    unordered_map<char, int> freq;
    for (char ch : text)
    {
        freq[ch]++;
    }
   
    for (auto pair : freq)
    {
        cout << "\n" << pair.first << " = " << pair.second;
        entrH += ((double)pair.second / n) * log2((double)n / pair.second);
    }

    cout << "\n\t Entropia log = " << fixed << setprecision(2) << entrH;

    // Crearea unei priority queue pentru a stoca nodurile arborelui Huffman
    // Create a priority queue to store live nodes of the Huffman tree
    priority_queue<Node*, vector<Node*>, comp> pq;

    // Creaza un nod de sfarsit pentru fiecare caracter si il adauga la priority queue
    // Create a leaf node for each character and add it to the priority queue

    for (auto pair : freq)
    {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));


    }

    // executa pana nu mai sunt noduri in queue
    // do till there is more than one node in the queue
    while (pq.size() != 1)
    {
        // Eliminarea celor 2 noduri cu cea mai inalta prioritate (cea mai joasa frecventa) din queue
        // Remove the two nodes of the highest priority (the lowest frequency) from the queue
        

        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        // crearea unui nou nod intern cu aceste 2 noduri ca 'copii' (noduri generate din alte noduri) cu o frecv. egala cu suma frecventelor celor 2 noduri
        // create a new internal node with these two nodes as children and with a frequency equal to the sum of the two nodes' frequencies
        
        // Adaugarea noului nod la priority queue
        // Add the new node to the priority queue

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    // 'root' stocheaza pointerul catre radacina arborelui Huffman
    // 'root' stores pointer to the root of Huffman Tree
    Node* root = pq.top();

    // Traversarea arborelui Huffman si stocharea codurilor Huffman intr-o mapa. Afisarea lor
    // Traverse the Huffman Tree and store Huffman Codes in a map. Also, print them 
    unordered_map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    cout << "\nCodul Huffman este:\n" << endl;
    for (auto pair : huffmanCode)
    {
        cout << pair.first << " " << pair.second << endl;
    }

    cout << "\nTextul inainte de codificare este:\n" << text << endl;

    // Afisiarea/printarea sirurilor codificate
    // Print encoded string

    for (char ch : text)
    {
        str += huffmanCode[ch];
    }

    cout << "\nTextul codificat este:\n" << str << endl;
    cout << "\nTextul decodificat este:\n";

    if (isLeaf(root))
    {
        // Cazul special: Pentru input de felul a, aa, aaa, etc. 
        // Special case: For input like a, aa, aaa, etc.
        while (root->freq--)
        {
            cout << root->ch;
        }
    }
    else
    {
        // Traversarea arborelui Huffman, insa de data aceasta sa decodifice sirul codificat
        // Traverse the Huffman Tree again and this time, decode the encoded string
        int index = -1;
        while (index < (int)str.size() - 1)
        {
            decode(root, index, str);
        }
    }
}

// Metoda de citire a sirului din fisier ca una din variante
// Method to read the string from file as an option
int CitireFisier(char* pathFile)
{
    ifstream filein(pathFile, std::ios_base::in);

    if (!filein)
    {
        cout << "Fisierul nu poate fi deschis!" << endl;
    }

    filein >> strTemp;

    ofstream fileout_css("input/mystyle.css", std::ios_base::out);

    if (!fileout_css)
    {
        cout << "Fisierul nu poate fi deschis!" << endl;
    }

    cout << "Continutul din fisier: " << strTemp << endl;

    n = strTemp.length();

    cout << "\nlung= " << n;

    buildHuffmanTree(strTemp);

    fileout_css << "body{background-color: #e0e2e4;}";

    fileout_css.close();

    ofstream fileout("input/index.html", std::ios_base::out);


    // afisarea unui mesaj de eroare in cazul unei probleme la deschiderea fisierului
    // Show a message in case of error with opening the file 
    if (!fileout)
    {
        cout << "Fisierul nu poate fi deschis!" << endl;
    }

    //Generarea unei pagini web de tip HTML cu elemente de interfata grafica CSS
    //Generating a HTML Web page with CSS elements
    fileout << "<html>" << endl;

    fileout << "<head>" << endl;

    fileout << "<link rel=\"stylesheet\" type=\"text/css\" href=\"mystyle.css\">";

    fileout << "<title>" << endl;
    fileout << "081720" << endl;
    fileout << "</title>" << endl;

    fileout << "</head>" << endl;

    fileout << "<style>" << endl;

    fileout << "div {background-color: #c6bcb6  ; width: 1000px; border: 15px solid #625750; padding: 100px; margin: 20px; position: absolute; top: 50%; left: 50%; margin-right: -50%; transform: translate(-50%, -50%); }" << endl;

    fileout << "</style>" << endl;

    fileout << "<body>" << endl;

    fileout << "<h1 style=\"text-align:center; outline-style: solid; outline-color:#ada397; font-family: Garamond, serif; \">Analiza si implementarea algoritmului de codificare Huffman <br> Analysis and implementation of the Huffman coding algorithm </br></h1>" << endl;

    fileout << "<h2>" << endl;
    fileout << " <div> Sirul initial: " << strTemp << "<br> <br> <br>" << "Sirul codificat: " << str << "<br> <br> <br>" << "Lungimea sirului: " << n << "<br> <br> <br>" << "Entropia: " << entrH << "<br> <br> <br>" << "</div> " << endl;
    fileout << "</h2>" << endl;

    fileout << "<p style=\"font-size:20px; font-family: verdana\">";
    fileout << "</p>";

    fileout << "</body>" << endl;
    fileout << "</html>" << endl;

    fileout.close();
    filein.close();

    return 1;
}

// Implementarea propriu-zisa a algoritmului Huffman 
// Huffman coding algorithm implementation in C++
int main()
{
    int size = 0;
    int  i, j, k;


    char str[10000];
    int freq[1000];
    char text[10000];

    int optiune, revenire;

    char pathFile[] = "input/info.txt";

    cout << "\t\t\tMENIU" << endl;
    cout << "Optiunile: " << endl;
    cout << "1.Citirea sirului din fisier." << "\n2.Citirea sirului de la tastatura." << "\n0.Exit" << endl;
    cout << "Optiunea aleasa: ";
    cin >> optiune;

    switch (optiune)
    {
    case 1:
    {
        CitireFisier(pathFile);

        break;
    }

    case 2:
    {

        cout << "Introduceti textul: ";
        getchar();
        cin.getline(text, 10000);

        for (j = 0; text[j]; j++);
        n = j;

        cout << "\nlung= " << n;

        buildHuffmanTree(text);

        break;
    }

    case 0:
    {
        break;
    }
    }

    return 0;
}

