#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "..\include\function.h"
using namespace std;

struct score_compare
{
    bool operator ()(function a, function b)
    {
        return a.getScore() < b.getScore();
    }
};

int pointLoss(int expecLoss)
{
    /*
    int randNum = rand()%100 +1;
    double lastVal = 0.0;
    for (map<int,double>::iterator iter = probDist.begin(); iter!=probDist.end(); ++iter)
    {
        if (randNum <=  (((*iter).second)*100) + lastVal)
                return (*iter).first;

        lastVal += (*iter).second*100;
    }
    cout<<"ERROR dropped out of for in pointLoss "<<endl;
    */
    return expecLoss;
}

void battle(function& A,function& B, int ccExpec,
       int cdExpec, int ddExpec, int rds)
{
    A.emptyHist();
    B.emptyHist();
    for (int i=0;i<rds;++i)
    {
        bool aChoice = A.makeChoice();
        bool bChoice = B.makeChoice();

       // cout<<"a decides to "<<aChoice<<endl;
        //cout<<"b decides to "<<bChoice<<endl;
        int aPoints = ROUND_START;
        int bPoints = ROUND_START;
        if (aChoice & bChoice) //c,c
        {
            aPoints -= pointLoss(ccExpec);
            bPoints -= pointLoss(ccExpec);
        }
        else if (!aChoice & bChoice) //d,c
        {
            bPoints -= pointLoss(cdExpec);
        }
        else if (aChoice & !bChoice)//c,d
        {
            aPoints -= pointLoss(cdExpec);
        }
        else //d,d
        {
            aPoints -= pointLoss(ddExpec);
            bPoints -= pointLoss(ddExpec);
        }

        A.incrPoints(aPoints);
        B.incrPoints(bPoints);

        A.updateHist(bChoice);
        B.updateHist(aChoice);

         //Debug
        //cout<<"A won "<< aPoints<<endl;
        //cout<<"B won "<< bPoints<<endl;
    }
    //cout<<"Battle over"<<endl;
}

int main()
{
    //Create a file which saves winning functions
    ofstream file;
    file.open("masterLisp.txt");


/*
    //Creat set  contesting functions
    function alwaysC (1,0,0);
    function alwaysD (0,1,0);
    function opChoice (0,0,1);
    function opChoice90 (0.1,0,0.9);

	//Initial strategy
    //Add customs to list
    vector<function> funcList;
    funcList.push_back(alwaysC);
    funcList.push_back(alwaysD);
    funcList.push_back(opChoice);
    funcList.push_back(opChoice90);
    //Create and add random functions
    for (int i = 4; i<POP_SIZE;++i)
    {
        function obj;
        funcList.push_back(obj);
    }
   */ 


	//Since the majority of the time, the winner
	//is alwaysC, alwaysD, or op1, then maybe we should
	//have a combo of these battling instead of just
	//those 3 and randoms. Then add randoms as wanted.
	//Plus this will cover the customs we put in.
	//This yields all possible combos of the first three spots.
	//with res 0.1
	vector<function> funcList;
	for (double c = 0; c<=1; c+=0.1)
	{
		for (double d = 0; d<=1, d+=0.1)
		{
			for (double op1 = 0; op1<=1; op1+=0.1)
			{
				if ( (c + d + op1)==1)
				{
					function obj(c,d,op1);
					funcList.push_back(obj);
				}
			}
		}
		
	}

	//Then add the number of randoms we would like:
	//Note: need to add constant if used
	 for (int i = 0; i<RAND_COUNT;++i)
    	{
        	function obj;
        	funcList.push_back(obj);
    	} 



//Debug
//    for (int i = 0; i<funcList.size();++i)
//        {
//            funcList[i].print();
//            cout<<endl;
//        }
//
//    cout<<funcList.size();

//Three specific random distributions for initial debug
    /*
    map<int,double> Pcc;
    map<int,double> Pcd;
    map<int,double> Pdd;



    //set up PCC
    function pcc_probs(PCC_RANGE+1);
    for (int key =0;key<=PCC_RANGE;++key)
    {
        Pcc.insert(pair <int,double>(key,pcc_probs.get(key)));
    }

    function pcd_probs(PCD_RANGE+1);
    for (int key =0;key<=PCD_RANGE;++key)
    {
    Pcd.insert(pair <int,double>(key,pcd_probs.get(key)));
    }

    function pdd_probs(PDD_RANGE+1);
    for (int key =0;key<=PDD_RANGE;++key)
    {
        Pdd.insert(pair <int,double>(key,pdd_probs.get(key)));
    }
*/
    //Battle of all possible expected values
    file << "(";
    for (int cc = 0; cc<=20;++cc)
    {
        file << "(" << cc;
        for (int cd = 0; cd <=100;++cd)
        {
            if (cd == 0)
                    file<<"(";
            file << "(" << cd;
            for (int dd = 10; dd<=60 ; ++dd)
            {
                if (dd == 10)
                    file<<"(";
                file << "(" << dd;
                //Battle all functions under given prob dists
                for (int i = 0; i<funcList.size();++i)
                {
                    for (int j = i+1; j<funcList.size();++j)
                    {
                        battle(funcList[i],funcList[j],cc, cd, dd,ROUNDS);
                    }
                }

                //Determine Winner
                function winner = *(std::max_element(funcList.begin(), funcList.end(), score_compare()));

                //Write winner data to screen
                /*
                cout << "Winner's score is: " << winner.getScore() << endl;
                cout << "The Winner's func is "; winner.print();
                cout << "\nThe loser's (and winner's) scores were: "<<endl;
                for (int ii=0; ii<funcList.size(); ++ii)
                    cout << funcList[ii].getScore() << endl;
                */

                //Write to Diagram
                winner.print(file);

                //Clear scores
                for (int ii=0; ii<funcList.size(); ++ii)
                    funcList[ii].clearScore();
                file << ")";
            }
            file << "))";
        }
        file << "))";
    }
    file << ")";
    file.close();
}
