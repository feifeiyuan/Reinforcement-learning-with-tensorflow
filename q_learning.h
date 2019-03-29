#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <unistd.h>
#define random(x) rand()%x;

using namespace std;

#define SCHDULE_SIZE 4

static const char *schedlable[] = {"0", "-10", "-20", "-30", "-40"};
static const char *schedtune[SCHDULE_SIZE] = {
	"/dev/1",
    "/dev/2",
    "/dev/3",
    "/dev/4"
    };

/*
class States{
public:
	States():mLitePower(0), mBigPower(0), mFPS(0){}
private:
	int mLitePower;
	int mBigPower;
	int mFPS;
};
*/

class q_learning{
public:
	q_learning():mEpsilon(0.9), mLr(0.1), mGamma(0.9), mMaxEpisodes(5),\
	mSampleTime(100*1000), mStepCounter(0), mStates(10), mActions(5){}
	void setLable(int lable_index);
	void build_q_table();
	void print_q_table();
	int choose_action(int state);
	vector<int> get_env_feedback(int state);
	void update_env(int state, int episode);
	void rl();
	float find_max(int state, int *index=NULL);
	int get_input_data();
private:
	vector< vector<float> > mQtable;
	float mEpsilon;
	float mLr;
	float mGamma;
	long int mStepCounter;
	int  mStates;
	int  mActions;
	long int mSampleTime;
	long int mMaxEpisodes;
};
