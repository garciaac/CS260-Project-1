#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include "constants.h"
using namespace std;

class function
{
    public:
        function(int size = FUNC_DEPTH);
        function(double x, double y, double z);

        void print(ofstream& cout);
        void emptyHist();
        bool makeChoice();
        double get(size_t pos);
        void incrPoints(int incrPoints);
        void updateHist(bool choice);
        unsigned long long getScore();
        void clearScore();
    private:
        vector<double> func_def;
        vector<bool> hist;
        unsigned long long score;

        vector<double> normalize(vector<double> rands);
};
#endif
