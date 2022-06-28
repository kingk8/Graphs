// Markino Samuels
// Project 5
// 11/29/2018

#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <ctime>
#include <fstream>
#include <locale>
#include <sstream>
#include <vector>
#include <climits>
#include <list> 

using namespace std;
#define vertex 100

void Menu()
{
    cout << endl << endl;
    string Cmd[13] = 
    {
    "                           MAIN MENU",
    "+------------------------------------------------------------------+",
    "|",
    "|  1) Enter start city and destination city [Atlanta to New York] ",
    "|  2) Find shortest route ",
    "|  3) Find cheapest route ",
    "|  4) Find ALL available routes with cost ",
    "|",
    "|  5) Exit system ",
    "|",
    "|  *  Direct flights Cost 1500.00 ",
    "|  *  50.00 less for each stop in between but a minimum of 100.00",
    "+------------------------------------------------------------------+"
    };
    for (int k=0; k<13;k++)
    {
        cout << "     " << Cmd[k] << endl;
    }
    cout << endl << endl;
}// main menu

class graph 
{ 
    private:
    int V; 
    vector<int> *Matrix;
    static const int MAX = 100;
    
    
    public: 
    string cities[MAX];
    string column;
    
    graph(int V);  // Constructor
    void buildGraph(ifstream &inF, int n);
    bool isReachable(int s, int d);
    int loadcities();
    int cityExist(string city, int n);
    
    void printpath(vector<int>& path, int &route, ofstream &outF);
    int isNotVisited(int x, vector<int>& path);
    void findpaths(int src, int dst, int &route, ofstream &outF);
    
    void printpathShortest(vector<int>& path, int &route, int n, int &prev, vector<int>& p, ofstream &outF);
    void findpathsShortest(int src, int dst, int &route, ofstream &outF);
    
    void getpaths(vector<int>& path, int &route, ofstream &outF);
    void loadCheapest(ifstream &inF, ofstream &outF);
    void printCheapest();
    void findpathsLongest(int src, int dst, int &route, ofstream &outF);
}; 
  
graph::graph(int V) 
{ 
    this->V = V; 
    Matrix = new vector<int>[V]; 
}// Constructor

// Check if there is path from source to destination
bool graph::isReachable(int s, int d)
{
    if (s == d)
        return true;
    
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
    
    list<int> queue;
    
    visited[s] = true;
    queue.push_back(s);
    
    vector<int>::iterator i;
 
    while (!queue.empty())
    {
        s = queue.front();
        queue.pop_front();
        
        for (i = Matrix[s].begin(); i != Matrix[s].end(); ++i)
        {
            if (*i == d)
                return true;
            
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
 
    return false;
}// Check if there is path from source to destination

// Create graph
void graph::buildGraph(ifstream &inF, int n) 
{
    string line;
    int i = 0;
    while(inF >> line)
    {
        for(int j=0; j<n; j++)
        {
            if(line[j] == '1'){Matrix[i].push_back(j);}
        }i++;
    }
}// Creates graph

// Print all paths
void graph::printpath(vector<int>& path, int &route, ofstream &outF) 
{ 
    int size = path.size();
    int price = 1500;
    for (int i = 0; i < size; i++)
    {  
        cout << "Route " << route << ": ";
        outF << "Route " << route << ": ";
        for (int j = 0; j < size; j++)
        {
            cout << "[" << cities[path[j]] << "] ";
            outF << "[" << cities[path[j]] << "] ";
        }
        price = price - (50*(size-2));
        if(price < 100){ price = 100;}
        if(size == 2){ price = 1500;}

        cout << " $" << price << endl << endl << endl;
        outF << " $" << price << endl << endl << endl;
        route++; break;
    }
}// Prints all paths
  
// Utility function to check if current vertex is in path
int graph::isNotVisited(int x, vector<int>& path) 
{ 
    int size = path.size(); 
    for (int i = 0; i < size; i++)  
        if (path[i] == x)  
            return 0;  
    return 1; 
}// Utility function to check if current vertex is in path
  
// Finds path from source to destination
void graph::findpaths(int src, int dst, int &route, ofstream &outF) 
{
    queue<vector<int> > q; 
    
    vector<int> path; 
    path.push_back(src); 
    q.push(path); 
    while (!q.empty()) { 
        path = q.front(); 
        q.pop(); 
        int last = path[path.size() - 1];
        
        if (last == dst)  
            printpath(path, route, outF);
        
        for (int i = 0; i < Matrix[last].size(); i++) { 
            if (isNotVisited(Matrix[last][i], path)) { 
                vector<int> newpath(path); 
                newpath.push_back(Matrix[last][i]); 
                q.push(newpath); 
            } 
        } 
    } 
}// Finds path from source to destination

// Check if path is different from previous
bool diffpath(vector<int>& path, vector<int>& p)
{
    for(int i=0; i<path.size(); i++)
    { 
        if(path[i] != p[i])
        {
            return true;
        }
    }
    return false;
}// Check if path is different from previous

// Prints shortest paths
void graph::printpathShortest(vector<int>& path, int &route, int n, int &prev, vector<int>& p, ofstream &outF) 
{ 
    int size = path.size();
    int price = 1500;
    
    if(prev == 0)
    {
        if(diffpath(path, p) == false) return;
        else
        {
            for (int i = 0; i < size; i++)
            {
                cout << "Route " << route << ": ";
                outF << "Route " << route << ": ";
                for (int j = 0; j < size; j++)
                {
                    cout << "[" << cities[path[j]] << "] "; p[j] = path[j];
                    outF << "[" << cities[path[j]] << "] "; p[j] = path[j];
                }
                price = price - (50*(size-2));
                if(price < 100){ price = 100;}
                if(size == 2){ price = 1500;}
                cout << " $" << price << endl << endl << endl;
                outF << " $" << price << endl << endl << endl;
                route++; prev = n; break;
            }
        }
    }
    
    if(n > prev)return;
    
    if(n == prev)
    {
        if(diffpath(path, p) == false) return;
        else
        {
            for (int i = 0; i < size; i++)
            {
                cout << "Route " << route << ": ";
                outF << "Route " << route << ": ";
                for (int j = 0; j < size; j++)
                {
                    cout << "[" << cities[path[j]] << "] "; p[j] = path[j];
                    outF << "[" << cities[path[j]] << "] "; p[j] = path[j];
                }
                price = price - (50*(size-2));
                if(price < 100){ price = 100;}
                if(size == 2){ price = 1500;}
                cout << " $" << price << endl << endl << endl;
                outF << " $" << price << endl << endl << endl;
                route++; prev = n; break;
            }
        }
    }  
}// Prints shortest paths

// Find Shortest Paths
void graph::findpathsShortest(int src, int dst, int &route, ofstream &outF) 
{
    queue<vector<int> > q;
    
  int prev = 0, n;
    vector<int> p;
    p.push_back(-1);
    
    vector<int> path; 
    path.push_back(src); 
    q.push(path); 
    while (!q.empty()) { 
        path = q.front(); 
        q.pop(); 
        int last = path[path.size() - 1];
        
        if (last == dst)
        {
            n = path.size();
            printpathShortest(path, route, n, prev, p, outF);
        }
        
        for (int i = 0; i < Matrix[last].size(); i++) { 
            if (isNotVisited(Matrix[last][i], path)) { 
                vector<int> newpath(path); 
                newpath.push_back(Matrix[last][i]); 
                q.push(newpath); 
            } 
        } 
    } 
}// Find Shortest Paths

// Get paths to find cheapest
void graph::getpaths(vector<int>& path, int &route, ofstream &outF) 
{ 
    int size = path.size();
    int price = 1500;
    
        for (int j=0; j<size; j++)
        {
            outF << "[" << cities[path[j]] << "] ";    
        }
     price = price - (50*(size-2));
     if(price < 100){ price = 100;}
     if(size == 2){ price = 1500;}
     
     outF << " $" << price << endl; route++;
}// Get paths to find cheapest

// Finds Longest Paths
void graph::findpathsLongest(int src, int dst, int &route, ofstream &outF) 
{
    queue<vector<int> > q;
    
  int prev = 0, n;
    vector<int> p;
    p.push_back(-1);
    
    vector<int> path; 
    path.push_back(src); 
    q.push(path); 
    while (!q.empty()) { 
        path = q.front(); 
        q.pop(); 
        int last = path[path.size() - 1];
        
        if (last == dst)
        {
            n = path.size();
            getpaths(path, route, outF);
        }
        
        for (int i = 0; i < Matrix[last].size(); i++) { 
            if (isNotVisited(Matrix[last][i], path)) { 
                vector<int> newpath(path); 
                newpath.push_back(Matrix[last][i]); 
                q.push(newpath); 
            } 
        } 
    } 
}// Finds Longest Paths

// Print Cheapest Paths
void graph::loadCheapest(ifstream &inF, ofstream &outF)
{
    string line;
    vector<string> L;
    string num;
    while(getline(inF,line))
    {
        L.push_back(line);
    }
    
    int route = 1;
    
    int m, f;
    int h = 0;
    
    for(int j=L.size()-1; j>0; j--)
    {
        line = L[j];
        for(int k=0; k<line.size(); k++)
        {
            if(isdigit(line[k]))
            {
                num.push_back(line[k]);
            }
            if(k == line.size()-1)
            {
                m = atoi(num.c_str());
                num = "";
                if(h == 0)
                {
                    f = m;
                    h++;
                    route = 1;
                }
            }
        }
        
        if(m != f){break;}
        if(m == f)
        {
            cout << "Route " << route << ": ";
            cout << line;
            cout << endl << endl << endl;
            outF << "Route " << route << ": ";
            outF << line;
            outF << endl << endl << endl;
            route++;     
        }
    }
}// Print Cheapest Paths

// Check if City exist
int graph::cityExist(string city, int n)
{
    bool w = true;
    string c;
    for(int i=0; i<n; i++)
    {
        c = cities[i];
        w = true;
        for(int j=0; j<c.length() && w==true; j++)
        {
            if(toupper(c[j]) != toupper(city[j])){ w=false;}
            if(j == c.length()-1 && toupper(c[j]) == toupper(city[j]) && w != false){ return i;}
        }
    }
    if(w == false)return -1;
}// Check if City exist

// Build Array holding all destionations
int graph::loadcities()
{
    string cityname;
    int j = 0;
    int k = 0;
    
    for(int i=0; i<column.length(); i++)
    {
        if(column[i] != ',')
        {
            if(column[i+1] == ',')
            {
                cityname.push_back(column[i]);
                cities[k] = cityname;
                k++;
                j = 0;
                cityname = "";
            }
            else { cityname.push_back(column[i]); j++;}
            
        }
    }
    return k;
}// Build Array holding all destionations

// Format csv file to insert in 2d array
string addZeros(string line)
{
    int evenCommas = 1;
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    for(int i=0; i<line.length(); i++)
    {
        if(line[i] == ',' && line[i-1] == ',' )
        {
            line.insert(i,"0");
        }
    }
    
    line.erase(remove(line.begin(), line.end(), ','), line.end());
    string newline;
    int j = 0;
    int k = 0;
    while(k<line.length())
    {
        if(line[k] == '1' || line[k] == '0')
        {
            newline.assign(line.begin()+k,line.end());
            break;
        }
        else k++;
    }
    return newline;
}// Format csv file to insert in 2d array



int main()
{
    int h = 0;
    graph T(1);
    string line;
    ifstream inF, cre;
    ofstream outF, ins;
    inF.open("direct.csv");
    ins.open("insert.txt");
    string node;
    int size;
    
    
    while(!inF.eof())
    {
        if(h == 0)
        {
            getline(inF, T.column);
            size = T.loadcities();
            h++;
        }
        else
        {
            getline(inF,line);
            node = addZeros(line);
            ins << node;
        }
    }
    inF.close();
    ins.close();
    
    graph TT(size);
    TT.column = T.column;
    size = TT.loadcities();
    
    cre.open("insert.txt");
    TT.buildGraph(cre, size);
    cre.close();
    
    string start, end;
    int route;
    int src, dest;
    bool running = true;
    
    bool active = true;
    char cmd;
    char option;
     while(option != '2' && running == true)
     {
         Menu();
         cout << " Enter Command: ";
         cin >> option;
         cout << endl;
         
         if(option == '5'){option = '2'; running = false;}
         switch(option)
         {
             case '1': 
                 {
                     cout << "Enter start city: ";
                     cin >> start;
                     if(TT.cityExist(start, size) == -1){ cout << "ERROR CITY DOES NOT EXIST! "; break;}
                     else{src = TT.cityExist(start, size);}
                     cout << "Enter destination: ";
                     cin >> end;
                     if(TT.cityExist(end, size) == -1){ cout << "ERROR CITY DOES NOT EXIST! "; break;}
                     else{dest = TT.cityExist(end, size);}

                     while(cmd != '5' && active == true)
                     {
                         Menu();
                         cout << " Enter Command: ";
                         cin >> cmd;
                         cout << endl;

                         switch(cmd)
                         {
                             case '1':
                                 {
                                     cout << "Enter start city: ";
                                     cin >> start;
                                     if(TT.cityExist(start, size) == -1){ cout << "ERROR CITY DOES NOT EXIST! "; break;}
                                     else{src = TT.cityExist(start, size);}
                                     cout << "Enter destination: ";
                                     cin >> end;
                                     if(TT.cityExist(end, size) == -1){ cout << "ERROR CITY DOES NOT EXIST! "; break;}
                                     else{dest = TT.cityExist(end, size);}
                                 }break;

                             case '2':
                                 {
                                     if(TT.isReachable(src, dest) ==  true){route = 1; outF.open("Route_shortest.txt"); TT.findpathsShortest(src, dest, route, outF); outF.close();}
                                     else{ cout << "ERROR PATH FROM SOURCE TO DESTINATION DOES NOT EXIST!...";}
                                 }break;

                             case '3':
                                 {

                                     if(TT.isReachable(src, dest) ==  true)
                                     {
                                         string file = "get";
                                         outF.open(file.c_str());
                                         route = 1; TT.findpathsLongest(src, dest, route, outF);
                                         outF.close();

                                         outF.open("Route_cheapest.txt");
                                         inF.open(file.c_str());
                                         TT.loadCheapest(inF, outF);
                                         inF.close();  outF.close();
                                     }
                                     else{ cout << "ERROR PATH FROM SOURCE TO DESTINATION DOES NOT EXIST!...";}
                                 }break;

                             case '4':
                                 {

                                     if(TT.isReachable(src, dest) ==  true){ route = 1; outF.open("Route_all.txt"); TT.findpaths(src, dest, route, outF); outF.close();}
                                     else{ cout << "ERROR PATH FROM SOURCE TO DESTINATION DOES NOT EXIST!...";}
                                 }break;

                             case '5':
                                 {
                                     active = false; running == false; option++; cout << "EXITING...";
                                 }break;

                             default:   cout << " UNKNOWN Command '" << cmd << "' ... retry" << endl;
                         }// second switch
                     }
                 }break;
                 
             case '2':
                 {
                     running == false; cout << "EXITING...";
                 }break;
             
             default:   cout << " MUST CHOOSE OPTION '1' TO CONTINUE..." << endl;
         }// first switch
     }// while
    
    return 0;
}// main