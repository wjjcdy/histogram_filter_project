/**
	simulate.cpp

	Purpose: implements a Simulation class which
	simulates a robot living in a 2D world. Relies 
	on localization code from localizer.py 

	This file is incomplete! Your job is to make 
	this code work. 
*/

#include "localizer.cpp"
#include <algorithm>
// #include "helpers.cpp"

class Simulation {
	
private:
	vector <char> get_colors() {
		vector <char> all_colors;
		char color;
		int i,j;
		for (i=0; i<height; i++) {
			for (j=0; j<width; j++) {
				color = grid[i][j];
				if(std::find(all_colors.begin(), all_colors.end(), color) != all_colors.end()) {
				    /* v contains x */
				} else {
					all_colors.push_back(color);
					cout << "adding color " << color << endl;
				    /* v does not contain x */
				}
			}
		}
		colors = all_colors;
		num_colors = colors.size();
		return colors;
	}

public: 
	vector < vector <char> > grid;
	vector < vector <float> > beliefs;

	float blur, p_hit, p_miss, incorrect_sense_prob;

	int height, width, num_colors;
	
	std::vector<int> true_pose;
	std::vector<int> prev_pose;

	vector <char> colors;
	Simulation(vector < vector<char> >, float, float, vector <int>);
    void Sense();
	void Move(int dy,int dx);
	char get_observed_color();
	void show_beliefs();
	void random_move(int *dy, int *dx);
	void run(int num_steps=1);
};

/**
Constructor for the Simulation class.
*/
Simulation::Simulation(vector < vector <char> > map, 
	float blurring,
	float hit_prob, 
	std::vector<int> start_pos
	) 
{
	grid = map;
	blur = blurring;
	p_hit = hit_prob;
	p_miss = 1.0;
	beliefs = initialize_beliefs(map);
	incorrect_sense_prob = p_miss / (p_hit + p_miss);
	true_pose = start_pos;
	prev_pose = true_pose;
	height = map.size();
	width=map[0].size();
	colors = get_colors();
    num_colors = colors.size();
}

void Simulation::Sense()
{
	vector < vector <float> > new_beliefs;
	char color = get_observed_color();
    new_beliefs = sense(color,grid,beliefs,p_hit,p_miss);
	if(new_beliefs.size() == 0)
	{
		new_beliefs = beliefs;
	}
	this->beliefs = new_beliefs;
}

void Simulation::Move(int dy,int dx)
{
	vector < vector <float> > new_beliefs;
	int new_y = (true_pose[0] + dy+height)%height;
	int new_x = (true_pose[1] + dx+width)%width;
	prev_pose = true_pose;
	true_pose[0] = new_y;
	true_pose[1] = new_x;
    new_beliefs = move(dy,dx,beliefs,blur);
	this->beliefs = new_beliefs;
}

char Simulation::get_observed_color()
{
	char color;
	char true_color = grid[true_pose[0]][true_pose[1]];
	double num = 1.0*rand()/RAND_MAX;
	if(num < incorrect_sense_prob)
	{
		vector <char> not_true_colors;
		for(int i=0;i<num_colors;i++)
		{
			if(colors[i] != true_color)
			{
				not_true_colors.push_back(colors[i]);
			}
		}
		   
	    int rand_num = rand() % not_true_colors.size();
        color = not_true_colors[rand_num];
	}
	else
	{
		color = true_color;
	}
	return color;
}
void Simulation::show_beliefs()
{
  cout << "y, x = (" << true_pose[0] << ", " << true_pose[1] << ")" << endl;
  show_grid(this->beliefs);
}

void Simulation::random_move(int *dy, int *dx)
{
	int dy_window[3] = {-1,0,1};
	int dx_window[3] = {-1,0,1};
	int rand_num = rand() % 3;
	*dy = dy_window[rand_num];
	rand_num = rand() % 3;
	*dx = dx_window[rand_num];
}

void Simulation::run(int num_steps)
{
	for (int i=0;i<num_steps;i++)
	{
		Sense();
		int dy,dx;
		random_move(&dy,&dx);
		Move(dy,dx);
	}
}

/**
You can test your code by running this function. 

Do that by first compiling this file and then 
running the output.
*/
int main() {

	vector < vector <char> > map;
	vector <char> mapRow;
	int i, j, randInt;
	char color;
	std::vector<int> pose(2);

	for (i = 0; i < 4; i++)
	{
		mapRow.clear();
		for (j=0; j< 4; j++)
		{
			randInt = rand() % 2;
			if (randInt == 0 ) {
				color = 'r';
			}
			else {
				color = 'g';
			}
			mapRow.push_back(color);
		}
		map.push_back(mapRow);
	}
	cout << "map is\n";
	Simulation simulation (map, 0.1, 10, pose);
	// simulation = Simulation(map, 0.1, 0.9, pose);
	cout << "initialization success!\n";
	show_grid(map);

	cout << "y, x = (" << simulation.true_pose[0] << ", " << simulation.true_pose[1] << ")" << endl;
	 for(int i =0;i<10;i++)
	 {
		 simulation.run();
		 simulation.show_beliefs();
		 cout<<"----------------"<<endl;
	 }
	return 0;
}
