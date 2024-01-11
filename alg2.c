#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


float A[5] = {0.2, 0.4, 0.6, 0.8, 1};
float F[5] = {0.2, 0.325, 0.45, 0.575, 0.7};



typedef struct vec2 {
	float x;
	float y;
} vec2;



vec2 receive_data (void) {
	float volume;
	float bpm;
	
	vec2 data = {volume, bpm};
	
	return data;
}



void send_data (float amplitude, float frequency) {
	printf("A=%f, F=%f\n", amplitude, frequency);
}



float calculate_stress (float volume, float bpm) {
	float a = 15; // lowest volume for baby
	float stress = 0;

	if (volume == 100) {
		stress = (0.5 * bpm) - 20; // stress based on heartbeat
		
	} else if (volume < 100 && volume > a) {
		stress = ((40 / (100 - a)) * (volume - 100)) + 50; // stress based on crying

	} else {
		stress = 10; // baby is relaxed	
	}

	return stress;
}



int cell_in_matrix (vec2 pos) {
	if (pos.x < 0 || pos.x > 4 || pos.y < 0 || pos.y > 4) {
		return 0;
	} else {
		return 1;
	}
}



int test_pos (vec2 pos, float prev_stress) {
	float new_stress, amplitude = A[(int)pos.x], frequency = F[(int)pos.y];
	vec2 input;
	
	time_t endwait;
	int seconds = 10;
	endwait = time(NULL) + seconds;

	// while loop that runs for X amount of seconds
	// the loop will rock the baby at the positions values
	while (time(NULL) < endwait) {
		input = receive_data();
		new_stress = calculate_stress(input.x, input.y);
		send_data(amplitude, frequency);	
	}

	// stress decreases return 1
	if (new_stress < prev_stress) {
		return 1;

	// stress increased or stayed the same
	} else {
		return 0;
	}
}



vec2 calm (vec2 pos, float curr_stress) {
	// find orthogonal positions
	vec2 p1 = {pos.x, pos.y - 1}; // down
	vec2 p2 = {pos.x - 1, pos.y}; // left

	// check if orthogonal positions are in matrix
	// if a pos isnt in matrx, coordinate = (1000, 1000)
	int check1 = cell_in_matrix(p1);
	int check2 = cell_in_matrix(p2);
	
	if (check1 != 0) {
		// test the first position (p1)
		int test1 = test_pos(p1, curr_stress);
		// if the stress decreases return p1
		if (test1 == 1) {
			return p1;
		}
	}
	
	if (check2 != 0) {
		// test the second position (p2)
		int test2 = test_pos(p2, curr_stress);
		// if the stress decreases return p2
		if (test2 == 1) {
			return p2;
		}
	}
}



void main (void) {
	float amplitude, frequency;
	vec2 pos = {4, 4}; // start at top right

	while (1) {
		vec2 input = receive_data(); // get input from crying&heartbeat
		
		int stress = calculate_stress(input.x, input.y); // calculate stress %
		
		pos = calm(input, stress); // find new position in matrix

		amplitude = A[(int)pos.x]; // get amplitude from matrix
		frequency = F[(int)pos.y]; // get frequency from matrix
		
		send_data(amplitude, frequency); // send data to motors
	}
}
