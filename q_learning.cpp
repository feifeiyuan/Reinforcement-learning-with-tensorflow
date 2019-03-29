#include "q_learning.h"

static void writeDataToFile(const char *path, const char *s) {
    std::ofstream outfile;
    outfile.open(path, std::ios::app);
    outfile << s ;
    outfile.close();
    return;
}

void q_learning::setLable(int lable_index){
    for(int i=0; i<SCHDULE_SIZE; i++){
        if(lable_index==0){
            if(i==1)
                writeDataToFile(schedtune[i], "10");
            else
                writeDataToFile(schedtune[i], schedlable[lable_index]);
        }else{
            writeDataToFile(schedtune[i], schedlable[lable_index]);
        }
    }
}

void q_learning::build_q_table()
{
	for(int i=0; i< mStates; i++){
		vector<float> actions;
		for(int j=0; j<mActions; j++){
			actions.push_back(0.0);
		}
		mQtable.push_back(actions);
	}
}

void q_learning::print_q_table(){
	int nstates = mQtable.size();
	cout << "\n\t"<< setw(10) <<"a1" << "\t" << setw(10) <<"a2" \
	<< "\t" << setw(10) <<"a3"<<"\t" << setw(10) <<"a4"<< "\t" << setw(10)<<"a5" << endl;
	for(int i=0; i< nstates; i++){
		cout << "s" << i << "\t";
		int nactions = mQtable[i].size();
		for(int j=0; j<nactions; j++){
			cout << setw(10) << mQtable[i][j] << setprecision(3) <<"\t";
		}
		cout<<""<<endl;
	}
}

float q_learning::find_max(int state, int *index){
	vector<float> actions = mQtable[state];
	float max_q_value = actions[0];
	int max_index = 0;
	for(int j=0; j<actions.size(); j++){
		if(max_q_value<actions[j]){
			max_q_value = actions[j];
			max_index = j;
		}
	}
	if(index)
		*index = max_index;
	return max_q_value;
}

int q_learning::get_input_data(){
	FILE *f = fopen("/sdcard/ql", "r");
    if (!f) {
		cout << "falied open file " << endl;
        return -1;
    }
	float load = 0;
	fscanf(f, "%f", &load);
	//cout << "load: " << load << endl;
	fclose(f);
	return int(load*100)%10;
}

int q_learning::choose_action(int state){
	
	int action = -1;
	float rd = random(100);
	rd *=0.01;
	//cout << "rd :" << rd << "\t state : " << state ;
	if(rd>mEpsilon){
		action = random(5);
	}else{
		find_max(state, &action);
	}
	//cout << "\taction :" << action << endl;
	return action;
}

vector<int> q_learning::get_env_feedback(int state){
	usleep(mSampleTime);
	vector<int> feedback;
	int state_ = get_input_data();
	feedback.push_back(state_);
	int reward = 0;
	//cout << "state: " << state << "\tstate_: " << state_ << endl;
	if(state_ > state){
		reward = 1;
	}else if(state_ < state){
		reward = -1;
	}
	feedback.push_back(reward);
	return feedback;
}

void q_learning::update_env(int state, int episode){
	cout << "state: " << state << "\t";
	cout << "episode: " << episode << "\t";
	cout << "mStepCounter: " << mStepCounter << endl;
}

void q_learning::rl(){
	build_q_table();
	print_q_table();
	srand(time(NULL));
	for(int episode=0; episode<mMaxEpisodes; episode++){
		mStepCounter = 0;
		int is_terminal = 0;
		int state = get_input_data();
		
		//update_env(state, episode);
		while(!is_terminal){
			int action = choose_action(state);
			vector<int> feedback = get_env_feedback(state);
			float q_predict = mQtable[state][action];
			float q_target = feedback[1]+mGamma*find_max(state);
			mQtable[state][action] += mLr*(q_target - q_predict);
			state = feedback[0];
			//update_env(state, episode);
			mStepCounter++;
			if(mStepCounter>70)
				is_terminal = 1;
		}
	}
	print_q_table();
}

int main(){
	q_learning *ql = new q_learning();
	ql->rl();
	free(ql);
	return 0;
}
