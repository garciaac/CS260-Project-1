#include "function.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "constants.h"
using namespace std;

function::
function(int size)
 : func_def(), hist(),score(0)
{
    int randNum;
    for (int ii=0; ii<size; ++ii)
    {
        randNum = rand() %100 + 1;
        func_def.push_back(randNum);
    }
    func_def = normalize(func_def);
}


function::
function(double x, double y, double z)
 : func_def(), hist(),score(0)
{
    func_def.push_back(x);
    func_def.push_back(y);
    func_def.push_back(z);
}

double
function::
get(size_t pos)
{
    return func_def[pos];
}

void
function::
emptyHist()
{
    this->hist.empty();
}

unsigned long long
function::
getScore()
{
 return this->score;
}

void
function::
clearScore()
{
    this->score = 0;
}

bool
function::
makeChoice()
{
    double coopPerc = 0.0;

    coopPerc += this->get(0);

    vector<bool>::iterator histIter = this->hist.begin();

    for(vector<double>::iterator funIt = this->func_def.begin()+3; funIt != this->func_def.end(); ++funIt)
    {
        if (histIter != this->hist.end())
            {
                if ( (*histIter) == 1) //cooperate
                    coopPerc += (*funIt);
                else //defect
                    {//no need to keep track
                    }
                histIter++;
            }
        else
            coopPerc += (*funIt);  //Default to them cooperating

    }

    int randNum = rand() %100 +1;

    if (randNum <= coopPerc*100)
        return 1; //decide to coop
    else
        return 0;  //decide to defect
}

void
function::
incrPoints(int incrPoints)
{
        this->score += incrPoints;
}

void
function::
updateHist(bool choice)
{
        this->hist.push_back(choice);
}

void
function::
print(ofstream& cout)
{
    cout << "(";
    for (int ii=0; ii<func_def.size(); ++ii)
    {
        cout << func_def[ii] << " ";
    }
    cout << ")";
}

vector<double>
function::
normalize(vector<double> rands)
{
    vector<double> normal(rands.size());
    double avg = 0.0;
    for (int ii=0; ii<rands.size(); ++ii)
        avg += rands[ii];

    for (int ii=0; ii<normal.size(); ++ii)
    {
        normal[ii] = rands[ii] / avg;
    }
    return normal;
}
