#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <windows.h>
#include <time.h>

using namespace std;

vector<string> FILES =
{
    "ins-PDP-11a-asc",//Y
    "ins-PDP-12a-asc",//N
    "ins-PDP-13a-asc",//N
    "ins-PDP-14a-asc",//N
    "ins-PDP-11b-asc",//Y
    "ins-PDP-11b-desc",//Y
    "ins-PDP-14b-asc",//Y
    "ins-PDP-14b-desc",//Y
    "k5inst1",//Y
    "k5inst2",//Y
    "k8inst1",//Y
    "k8inst2",//Y
    "k11inst1",//Y
    "k11inst2",//Y
    "k14inst1",//Y
    "k14inst2",//Y
};

vector<int> MapOfCuts;///obvious, to be filled in main
vector<vector<int>> Instance;///one is value, second is 'boolean' flag
int MapLen, n;///to be filled

void FileRead(string filename)
{
    ifstream input;
    string line, num;
    input.open("Instancje/" + filename + ".txt");
    Instance.clear();///
    Instance.push_back(vector<int>());
    Instance.push_back(vector<int>());

    if(!input.good())///when something goes wrong
    {
        cout<<"Blad otwarcia"<<endl;
        system("pause");
        exit(0);
    }
    while(input.good())
    {
        input>>line;
        num.clear();
        for(int i = 0; i < line.size(); i++)
        {
            if(isdigit(line[i])) num.push_back(line[i]);
        }
        int k = atoi(num.c_str());
        Instance[0].push_back(k);
        Instance[1].push_back(0);
    }
    input.close();

    cout<<endl<<"Instancja '"<<filename<<"':"<<endl;
    for(int i = 0; i < Instance[0].size(); i++) cout<<Instance[0][i]<<" ";
};

int CheckCutNum(int ElemCnt)///thats gonna be a check
{
    float num = ((1 + sqrtf(1 + 8 * ElemCnt) ) / 2) - 1;
    return num;
};

int Sum(vector<int> tmp)
{
    int add = 0;
    for(int i = 0; i < tmp.size(); i++) add += tmp[i];
    return add;
};

void GetSetFirstElem(vector<vector<int>> tmp)
{
    MapOfCuts.clear();
    tmp[0].erase(find(tmp[0].begin(), tmp[0].end(), MapLen));
    int first = (MapLen) - (*max_element(tmp[0].begin(), tmp[0].end()));
    MapOfCuts.push_back(first);
    auto it = find(Instance[0].begin(), Instance[0].end(), first);
    Instance[1][distance(Instance[0].begin(), it)] = 1;///set as used
};

bool CheckAnswer(vector<int> tmp_answer, int elem)///check if it fits to a map
{
    int k = 2;///pair, couple of three and whatever is next
    int x = 0;
    vector<int> tmp;///sum every k-elem subcontainer

    tmp_answer.push_back(elem);///add hipothetical element

    if(tmp_answer.size() == 1) return true;///obvious
    while(k <= tmp_answer.size())///checking k-elem
    {
        for(int i = 0; i < tmp_answer.size() - (k - 1); i++)///we dont want to jump into std::bad_alloc
        {
            x = 0;
            tmp.clear();
            for(int j = 1; j <= k; j++)///do k times
            {
                tmp.push_back(tmp_answer[i + x]);
                x++;
            }
            if(find(Instance[0].begin(), Instance[0].end(), Sum(tmp)) == Instance[0].end()) return false;
            else continue;
        }
        k++;
    }
    return true;
};

void GiveMeMapOfCutsPlease(vector<vector<int>> tmp, vector<int> answer)
{
    vector<vector<int>> local_tmp;///local copies
    vector<int> local_answer;
    for(auto it = tmp[0].begin(); it != tmp[0].end(); ++it)
    {
        local_tmp = tmp;///they cannot be assigned out of loop
        local_answer = answer;
        int index = distance(tmp[0].begin(), it);///to know what should be marked as used
        if((Sum(local_answer) + *it) <= MapLen)///hipothesis
        {
            if(CheckAnswer(local_answer, *it) == true)
            {
                if(local_tmp[1][index] == 0)///if it is unused
                {
                    local_answer.push_back(*it);
                    local_tmp[1][index] = 1;///mark as used
                    if(Sum(local_answer) != MapLen) GiveMeMapOfCutsPlease(local_tmp, local_answer);///backtrack
                    else if(Sum(local_answer) == MapLen && local_answer.size() == n)///we have finished
                    {
                        MapOfCuts = local_answer;
                        return;
                    }
                }
            }
        }
    }
};

main()
{
    FileRead(FILES[11]);///obvious
    MapLen = *max_element(Instance[0].begin(), Instance[0].end());///determining maximum length of answer
    n = CheckCutNum(Instance[0].size());///determining number of elements in a map

    cout<<endl<<"Liczba elementow instancji: "<<Instance[0].size();
    cout<<endl<<"Dlugosc mapy = "<<MapLen;
    cout<<endl<<"Liczba elementow mapy: "<<n;

    GetSetFirstElem(Instance);
    const clock_t begin_time = clock(); /// Measure time
    GiveMeMapOfCutsPlease(Instance, MapOfCuts);
    cout<<endl<<"Wynik: "<<endl;
    for(int i = 0; i < MapOfCuts.size(); i++) cout<<MapOfCuts[i]<<" ";
    cout<<endl<<"Czas: "<<float(clock() - begin_time) /  CLOCKS_PER_SEC;

    cout<<endl;
    system("pause");
    return 0;
};
