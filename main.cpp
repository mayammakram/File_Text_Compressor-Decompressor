#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <utility>
#include <cmath>
#include <bitset>
#include <queue>
//#include <obitstream>
//#include <algorithm>
using namespace std;

struct Symbol
{
    char symbol;
    int freq;
    double prob;
    string virtualNode;
    string code;
};
vector<Symbol> storedSymbol;
vector<Symbol> codeTable;
void Menu();
void CalculateProbabilty(string& fileName, vector<Symbol>& alphabet, vector<char>& input);
ofstream WriteCodedFile(vector<Symbol> codeTable, vector<char> input, string);
void CalculateCRH(vector<Symbol>& alphabet, double& compressionratio, double& H);
void bitstream_comp(vector<char> codes, ofstream& output);
void CompChoice();
void DecompChoice();



void heapify(vector<Symbol>& arr, int n, int i);
void build_heap(vector<Symbol>& arr, int n);
void heap_sort(vector<Symbol>& arr, int n);
Symbol merge(Symbol a1, Symbol a2);
vector<Symbol>  mergeTree(vector<Symbol>& alphabet);



int find(string required, vector<Symbol>vec);
void decompression();

//void bitstream_comp(queue<string> codes, ofstream& output);

void decToBinary(int n)
{
    // array to store binary number
    int binaryNum[32];

    // counter for binary array
    int i = 0;
    while (n > 0) {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
        cout << binaryNum[j];
}

int main()
{
    Menu();
    //string all;
    //ifstream h;
    //h.open("OutputT9.txt");
    //getline(h, all);
    //bitset<100> c(all);
    //cout << c;
}

void Menu()
{
    cout << "\t\t\t\tWelcome to My Codec :) \n";
    cout << "\t\t\t\tKindly choose what would you like to do:\n";
    cout << "\t\t\t\tDo you like to: \n\t\t\t\t1. Compress\n\t\t\t\t2. Decompress\n";
    cout << "\t\t\t\tYour Choice: ";
    int choice;
    cin >> choice;
    cout << "\n";
    if (choice == 1)
        CompChoice();
    else if (choice == 2)
        decompression();
    else
    {
        cout << "\t\t\t\tPlease Try Again.\n";
        Menu();
    }
}
void CompChoice()
{
    string inpath, outpath, dummy;
    //cin >> dummy;
    cout << "Please insert your input file path: \n";
    getline(cin >> ws, inpath);
    //cout << "ok";
    //getline(cin, inpath, '!');
    //cin >> dummy;
    cout << "\nPath you entered: " << inpath << endl;
    cout << "Please insert your output file path: \n";
    //inpath = "Text File Name.txt";

    getline(cin >> ws, outpath);
    //outpath = "CompressedFile.bin";
    vector<char> input(100);
    vector<Symbol> alphabet(100);
    double compressionR = 0, H = 0;
    CalculateProbabilty(inpath, alphabet, input);
    codeTable = mergeTree(alphabet);
    /*for (int i = 0; i < codeTable.size(); i++)
    {
        cout << codeTable[i].symbol << "  " << codeTable[i].code << "    ";
    }*/
    WriteCodedFile(codeTable, input, outpath);
    CalculateCRH(codeTable, compressionR, H);
    cout << "\n\nCompression Ratio: " << compressionR << "\nEncoding Efficiency: " << H << "\n";
}
void DecompChoice()
{
    string path;
    cout << "Please insert your file path: \n";
    cin >> path;
    decompression();
}
void CalculateProbabilty(string& fileName, vector<Symbol>& alphabet, vector<char>& input)
{
    bool occurred = false;
    ifstream readFile(fileName.c_str());
    //vector<char> input(100);
    //vector<Symbol> alphabet(100);
    for (int i = 0; i < alphabet.size(); i++)
    {
        alphabet[i].freq = 1;
    }
    char byte;
    if (readFile.fail())
    {
        cout << "Error!";
        exit(1);
    }
    int k = 0, j = 0;
    while (readFile.get(byte))
    {
        input[k] = byte;
        k++;
        for (int i = 0; i < alphabet.size(); i++)
        {
            if (byte == alphabet[i].symbol)
            {
                alphabet[i].freq++;
                occurred = true;
            }
        }
        if (!occurred)
        {
            alphabet[j].symbol = byte;
            alphabet[j].virtualNode = byte;
            j++;
        }
        occurred = false;
    }
    input.resize(k);
    alphabet.resize(j);
    int total = 0;
    //double avgL = 0;
    //double temp1, temp2;
    for (int i = 0; i < alphabet.size(); i++)
    {
        /*  temp1 = alphabet[i].prob;
          temp2 = alphabet[i].code.length();*/
        total += alphabet[i].freq;
        /*cout << "Probability: " << temp1 << "  L: " << temp2 << endl;
        avgL = avgL + temp1 * temp2;
        H = H + temp1 * log(1 / temp1);*/
    }
    //cout << "Total: " << total << endl;
    for (int i = 0; i < alphabet.size(); i++)
    {
        alphabet[i].prob = (double)alphabet[i].freq / total;
    }
    //for (int i = 0; i < alphabet.size(); i++)
    //{
    //    cout << setw(15) << alphabet[i].symbol << /setw(15) << alphabet[i].freq <</ setw(15) << alphabet[i].prob << endl;
    //}
    /*cout << "AbgL: " << avgL << "\n\n";
    compressionratio = avgL / 8.0;*/

}
ofstream WriteCodedFile(vector<Symbol> codeTable, vector<char> input, string outFile)
{
    vector<int> Char;
    ofstream writeComp;
    writeComp.open(outFile, ios::out | ios::binary);

    if (writeComp.fail())
        cout << "ERRORRRRR";
    vector<char> all(5000);
    int l = 0;
    for (int i = 0; i < input.size(); i++)
    {
        //cout << "input: " << input[i] << " ";
        for (int j = 0; j < codeTable.size(); j++)
        {
            if (codeTable[j].symbol == input[i])
            {
                //writeComp.write(codeTable[j].code.c_str(), codeTable[j].code.length());
                //writeBit()
                //cout << codeTable[j].code << " ";
                //writeComp << codeTable[j].code;
                for (int k = 0; k < codeTable[j].code.length(); k++)
                {
      
                all[l] += codeTable[j].code[k];
                //all.resize(j + k);
                l++;
                }
            }
        }
    }
    all.resize(l);
    bitstream_comp(all, writeComp);
    /*for (int i = 0; i < codeTable.size(); i++)
    {
        writeComp << endl<<codeTable[i].symbol << "  " << codeTable[i].code;
    }*/
    return writeComp;
}
void CalculateCRH(vector<Symbol>& alphabet, double& compressionratio, double& H)
{
    double avgL = 0, sum = 0;
    double temp1, temp2;
    for (int i = 0; i < alphabet.size(); i++)
    {
        temp1 = alphabet[i].prob;
        temp2 = alphabet[i].code.length();
        cout << "Code: " << alphabet[i].code << endl;
        cout << "Probability: " << temp1 << "  L: " << temp2 << endl;
        avgL = avgL + temp1 * temp2;
        sum = temp1 * (log(1 / temp1) / log(2));
        H += sum;
    }
    cout << "AvgL: " << avgL << "\n\n";
    compressionratio = avgL / 8.0;
    cout << "Here is H: " << H << endl;
    H = H / avgL;
    cout << "Eta: " << H << endl;
}




void bitstream_comp(vector<char> codes, ofstream& output)
{
    string str = "";

    do
    {
        //vector<char> codes
        /*str = str + codes.front();
        codes.pop();*/
        str = str + codes[0];
        //cout << "Str: " << str << "\n";
        codes.erase(codes.begin());
        //cout << "\nStr After: " << str << "\n";
        while (str.size() >= 8) //for loop
        {
            string r = str.substr(0, 8);
            str = str.substr(8, str.size());
            bitset<8> temp(r);
            cout << "Bitset: " << temp << endl;
            cout << "Test: " << char(temp.to_ulong()) << endl;
            output << char(temp.to_ulong());
        }
    } while (!codes.empty());

    bitset<8> temp(str);
    output << char(temp.to_ulong());
    output << char(8 - str.size());
}

//void bitstream_comp(queue<string> codes, ofstream& output)
//{
//    string str = "";
//    do
//    {
//        str = str + codes.front();
//        codes.pop();
//        while (str.size() >= 8)
//        {
//            string r = str.substr(0, 8);
//            str = str.substr(8, str.size());
//            bitset<8> temp(r);
//
//            //output << char(temp.to_ulong());
//        }
//    } while (!codes.empty());
//
//    bitset<8> temp(str);
//    output << char(temp.to_ulong());
//    output << char(8 - str.size());
//}


//void file_compression(ifstream& inputfile, ofstream& output)
//{
//    char c;
//    inputfile.get(c);
//    queue<string> codes;
//    while (!inputfile.eof())
//    {
//        codes.push(encoding.at(c));
//        inputfile.get(c);
//    }
//    bitstream_comp(codes, output);
//    ofstream in("tree.txt");
//    if (!in.is_open()) cout << "error opening the file" << endl;
//
//    for (auto& it : encoding)
//    {
//        in << it.first << " " << it.second << endl;
//    }
//}
void heapify(vector<Symbol>& arr, int n, int i)
{
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].prob > arr[smallest].prob)
        smallest = l;

    if (r < n && arr[r].prob > arr[smallest].prob)
        smallest = r;

    if (smallest != i)
    {
        swap(arr[i], arr[smallest]);
        heapify(arr, n, smallest);
    }
}
void build_heap(vector<Symbol>& arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
}
void heap_sort(vector<Symbol>& arr, int n)
{
    build_heap(arr, n);
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
Symbol merge(Symbol a1, Symbol a2)
{
    Symbol merged;
    merged.virtualNode = a1.virtualNode + a2.virtualNode;
    merged.prob = a1.prob + a2.prob;
    merged.freq = -1;

    return merged;
}
vector<Symbol>  mergeTree(vector<Symbol>& alphabet)
{
    vector<Symbol> copy_alphabet;
    copy_alphabet = alphabet;
    Symbol dummy;
    int j = 0;
    storedSymbol.resize(5000);
    while (copy_alphabet.size() > 1)
    {
        heap_sort(copy_alphabet, copy_alphabet.size());

        dummy = merge(copy_alphabet[0], copy_alphabet[1]);

        storedSymbol[j] = copy_alphabet[0];
        storedSymbol[++j] = copy_alphabet[1];

        storedSymbol[++j] = dummy;
        copy_alphabet.push_back(dummy);
        copy_alphabet.erase(copy_alphabet.begin());
        copy_alphabet.erase(copy_alphabet.begin());
        j++;
    }
    storedSymbol.resize(j);
    storedSymbol[storedSymbol.size() - 1].code = "";
    for (int i = storedSymbol.size() - 2; i > 1; i--)
    {
        storedSymbol[i].code = storedSymbol[i + 1].code + "1"; //i = 10
        storedSymbol[--i].code = storedSymbol[i + 2].code + "0"; //i = 9
        i--;
        for (int j = i + 1; j < storedSymbol.size(); j++) //j = 9 --> 11
        {
            if (storedSymbol[i].virtualNode == storedSymbol[j].virtualNode) //i = 8
                storedSymbol[i].code = storedSymbol[j].code;
        }
    }
    storedSymbol[0].code = storedSymbol[2].code + "0";
    storedSymbol[1].code = storedSymbol[2].code + "1";


    /* for (int i = 0; i < storedSymbol.size(); i++)
         cout << "Symbol: " << storedSymbol[i].virtualNode<< " Code: "<< storedSymbol[i].code << endl;*/

    cout << endl << endl;
    codeTable.resize(5000);
    int k(0);
    for (int i = 0; i < storedSymbol.size(); i++)
    {
        if (storedSymbol[i].virtualNode.length() == 1)
        {
            codeTable[k] = storedSymbol[i];
            k++;
        }
    }

    codeTable.resize(k);

    //for (int i = 0; i < codeTable.size(); i++)
    //    cout << "Symbol: " << codeTable[i].virtualNode << "  Code:" << codeTable[i].code << endl;

    return codeTable;

}



int find(string required, vector<Symbol>vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (required == vec[i].code)
            return i;
    }
    return -1; //acts as a flag
}
void decompression()
{
    int index;
    string str;
    ifstream file;
    ofstream newfile;
    string filename;
    string compressed;
    string temp;
    vector<Symbol>vec;
    Symbol sym;


    cout << "Enter the path of the compressed file: ";
    getline(cin >> ws, filename);
    file.open(filename);
    cout << "Enter the desired path of the non-compressed file: ";
    getline(cin >> ws, filename);
    newfile.open(filename);
    file >> compressed; //compressed string
   // getline(file, compressed);
    //bitset<8> element(compressed);
    //cout << element << " ";
    while (!file.eof())
    {
        file >> temp;
        if (temp[0] != '"' && temp != "\n")
        {
            sym.virtualNode = temp;
            file >> temp;
            sym.code = temp;
            vec.push_back(sym);
        }
        else
        {
            if (temp.substr(1, 2) == "\\n")
            {
                sym.virtualNode = "\n";
                file >> temp;
                sym.code = temp;
                vec.push_back(sym);
            }
            else if (temp[0] == '"')
            {
                file >> temp;
                string s;
                s.push_back('"');
                s.push_back(' ');
                s.push_back('"');
                sym.virtualNode = s;
                file >> temp;
                sym.code = temp;
                vec.push_back(sym);
            }
        }
    }
    for (int i = 0; i < compressed.length(); i++)
    {
        str.push_back(compressed[i]);
        index = find(str, vec);
        if (index != -1)
        {
            temp = vec[index].virtualNode;
            if (temp[0] != '"' && temp != "\\n")
                newfile << temp;
            else if (temp == "\\n")
                newfile << '\n';
            else if (temp[0] == '"')
                newfile << " ";
            str = "";
        }
    }
    newfile.close();
    file.close();
}