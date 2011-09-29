#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include "constants.h"
using namespace std;

// This class is used to represent an algorithm for the prisoner's dilemma.
// We wrote a simulator and created many, many of these objects and had them
// play against each other with given expected values for point loss for every
// scenario. For every combination of expected values, every algorithm plays
// against every algorithm a number of times specified in constants.h (ROUNDS)
class function
{
    public:
    	// Default ctor --> creates a random algorithm 
	// (randomly weights its decision making)
        function(int size = FUNC_DEPTH);
	// Ctor --> creates a function with given probabilities for
	// cooperate, defect, and what opponent did last turn
        function(double x, double y, double z);

	// Prints a readable representation of the function (in lisp syntax) 
        void print(ofstream& cout);
	// Empties an algorithms history
        void emptyHist();
	// Looks at its probabilities and decides to cooperate or defect
        bool makeChoice();
	// Retrieves the probability at the given index of the function 
        double get(size_t pos);
	// Adds the given number of points to the algorithms score
        void incrPoints(int incrPoints);
	// Adds a decision to its history
        void updateHist(bool choice);
	// Returns the algorithm's score
        unsigned long long getScore();
	// Resets the algorithm's score
        void clearScore();
    private:
    	// Contains the probabilities for decision making
        vector<double> func_def;
	// History of the algorithm's decisions
        vector<bool> hist;
	// Total score of the algorithm over the tournament
        unsigned long long score;
	
	// Given a set of random numbers, normalize converts them to 
	// probabilities that sum to 1
        vector<double> normalize(vector<double> rands);
};
#endif
