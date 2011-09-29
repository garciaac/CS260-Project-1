#ifndef CONSTANTS_H
#define CONSTANTS_H

// FUNC_DEPTH represents how many turns in our opponents
// history are considered when making the decision. A function
// is represented by a list of FUNC_DEPTH numbers. The first two
// correspond to cooperate and defect respectively, and the remaining
// numbers correspond to the probability of doing what the opponent 
// did that many turns ago. e.g the third number in a function is the
// probability of the function deciding to do what the opponent did
// on the previous turn. 
const int FUNC_DEPTH = 12;

// Originally, we used POP_SIZE to represent how many algorithms
// that we kept in our pool, but we have since changed our 
// strategy, and this variable is now redundant.
const int POP_SIZE = 10;

// ROUNDS is the number of times we have all of our algorithms
// battle the other algorithms per combination of expected values
// for point loss.
const int ROUNDS = 100;

// ROUND_START is the number of points awarded at the start of
// each round
const int ROUND_START=100;

// RAND_COUNT is the number of algorithms in the pool that 
// contain completely random probabilities for decision making
const int RAND_COUNT = 5;

// PCC_RANGE is the max point loss for cooperate-cooperate
const int PCC_RANGE = 20;
// PCD_RANGE is the max point loss for cooperate-defect
const int PCD_RANGE = 100;
// PDD_RANGE is the max point loss for defect-defect
const int PDD_RANGE = 50;

#endif /*CONSTANTS_H*/
