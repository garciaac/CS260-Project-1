#include "function.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "constants.h"
using namespace std;

// Default ctor --> creates a random algorithm 
// (randomly weights its decision making)
function::
function(int size)
 : func_def(), hist(),score(0)
{
    // The random number to derive a probability from
    int randNum;
    // Loops the number of times given as a parameter
    for (int ii=0; ii<size; ++ii)
    {
    	// Make a random number from 1-100 and add it to the
	// algorithm's definition
        randNum = rand() %100 + 1;
        func_def.push_back(randNum);
    }
    // Normalizes all of the random numbers so that they are probabilities
    // that sum to one
    func_def = normalize(func_def);
}

// Ctor --> creates a function with given probabilities for
// cooperate, defect, and what opponent did last turn
function::
function(double x, double y, double z)
 : func_def(), hist(),score(0)
{
    // Add the given probabilities to the algorithm's definition -->
    // up to user to make sure that they sum to one
    func_def.push_back(x);
    func_def.push_back(y);
    func_def.push_back(z);
}

// Prints a readable representation of the function (in lisp syntax)
void
function::
print(ofstream& cout)
{
    // For proper lisp syntax, we surround the function's values with parens
    cout << "(";
    // Loop through the functions definition and print out its probability
    // distribution
    for (int ii=0; ii<func_def.size(); ++ii)
    {
        cout << func_def[ii] << " ";
    }
    cout << ")";
}

// Empties an algorithm's history
void
function::
emptyHist()
{
    this->hist.empty();
}

// Looks at its probabilities and decides to cooperate or defect
// NOTE: we only calculate the chance of the algorithm cooperating
//       since there are only two possible choices
bool
function::
makeChoice()
{
    // Percent chance of this decision being cooperate
    double coopPerc = 0.0;

    // Since the first spot in the algorithm's definition corresponds
    // to probability of blindly cooperating, we add that to the 
    // percentage right away.
    coopPerc += this->get(0);

    // Initialize an iterator to point to the beginning of the history
    // of this algorithm
    vector<bool>::iterator histIter = this->hist.begin();

    // Loops through the algorithm's probability distribution and see if 
    // the percentage should be increased.
    for(vector<double>::iterator funIt = this->func_def.begin()+3; funIt != this->func_def.end(); ++funIt)
    {
    	// The history may be empty, so we make sure there is a corresponding
	// decision there, otherwise we default to cooperate.
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

    // Generate a random number between 1 and 100 and then use it
    // to make the decision
    int randNum = rand() %100 +1;

    // We only defect if the random number is higher than the 
    // chance of cooperating
    if (randNum <= coopPerc*100)
        return 1; //decide to coop
    else
        return 0;  //decide to defect
}

// Retrieves the probability at the given index of the function 
double
function::
get(size_t pos)
{
    return func_def[pos];
}

// Adds the given number of points to the algorithms score
void
function::
incrPoints(int incrPoints)
{
        this->score += incrPoints;
}

// Adds a decision to its history
void
function::
updateHist(bool choice)
{
        this->hist.push_back(choice);
}

// Returns the algorithm's score
unsigned long long
function::
getScore()
{
 return this->score;
}

// Resets the algorithm's score
void
function::
clearScore()
{
    this->score = 0;
}

// Given a set of random numbers, normalize converts them to 
// probabilities that sum to 1
vector<double>
function::
normalize(vector<double> rands)
{
    // Create the return value. Should be same size as parameter
    vector<double> normal(rands.size());
    // Our math was initially incorrect. Avg contains a sum not the average.
    double avg = 0.0;

    // Add up all of the random numbers
    for (int ii=0; ii<rands.size(); ++ii)
        avg += rands[ii];

    // Divide all the random numbers by the sum of all of them to yeild 
    // their relative weight.
    for (int ii=0; ii<normal.size(); ++ii)
    {
        normal[ii] = rands[ii] / avg;
    }
    return normal;
}
