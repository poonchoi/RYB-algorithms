#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define size 5


typedef struct vec2 {
	float x;
	float y;
} vec2;


vec2 receive_data(void);
float calculate_stress(float volume, float bpm);
int stress_in_bound(float curr_stress, vec2 kn);
vec2 traverse(vec2 k_matrix[][size], vec2 curr_pos);
void transmit_data(float amplitude, float frequency);
int cell_in_matrix(vec2 pos);
void print_matrix(vec2 k_matrix[][size], vec2 curr_pos);


void print_vec(vec2 vector) {
	printf("[%f, %f]\n", vector.x, vector.y);
}


void print_matrix (vec2 k_matrix[][size], vec2 curr_pos) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == (int)curr_pos.y && j == (int)curr_pos.x) {
				printf("  ");
			} else if (k_matrix[i][j].x == 0) {
				printf("K0");
			} else if (k_matrix[i][j].x == 11) {
				printf("K1");
			} else if (k_matrix[i][j].x == 21) {
				printf("K2");
			} else if (k_matrix[i][j].x == 31) {
				printf("K3");
			} else if (k_matrix[i][j].x == 41) {
				printf("K4");
			} else if (k_matrix[i][j].x == 51) {
				printf("K5");
			} else if (k_matrix[i][j].x == 61) {
				printf("K6");
			} else if (k_matrix[i][j].x == 71) {
				printf("K7");
			} else if (k_matrix[i][j].x == 81) {
				printf("K8");
			} else if (k_matrix[i][j].x == 91) {
				printf("K9");
			}
			if (i < size && j < size - 1) {
				printf(",");
			}
		}
		printf("\n");
	}
	printf("\n");
}


vec2 receive_data (void) {
	//float bpm = rand() % 181 + 60, volume = rand() % 101;
	float bpm, volume;
	printf("Volume, BPM: ");
	scanf(" %f %f", &volume, &bpm);
	vec2 data = {volume, bpm};
	return data;
}


void transmit_data (float amplitude, float frequency) {
	printf("Amplitude = %f, Frequency = %f\n", amplitude, frequency);
}


int cell_in_matrix (vec2 pos) {
	if (pos.x < 0 || pos.x >= size || pos.y < 0 || pos.y >= size) {
		return 0;
	} else {
		return 1;
	}
}


float calculate_stress (float volume, float bpm) {
	float a = 10;
	float stress = 0;

	if (volume == 100) {
		stress = (0.5 * bpm) - 20;

	} else if (volume < 100 && volume > a) {
		//stress = (((100 - a) / 40) * (volume - 50)) + 100;
		stress = ((40 / (100 - a)) * (volume - 100)) + 50;

	} else {
		stress = 10;
	}

	// if (volume <= a) {
		// stress = 10;
	// } else {
		// stress = (0.5 * bpm) - 20;
	// }

	return stress;
}


int stress_in_bound (float curr_stress, vec2 kn) {
	// if the current stress is in the bound return 1
	// else return 0
	if (curr_stress >= kn.x) {
		return 1;
	} else {
		return 0;
	}
}


vec2 traverse (vec2 k_matrix[][size], vec2 curr_pos) {
	vec2 null = {-1000, -1000};
	vec2 directions[4];
	int check = 0;
	vec2 orth_pos;

	// north
	orth_pos.x = curr_pos.x;
	orth_pos.y = curr_pos.y - 1;
	check = cell_in_matrix(orth_pos);
	if (check == 1) {
		directions[0] = k_matrix[(int)orth_pos.y][(int)orth_pos.x];
	} else {
		orth_pos = null;
		directions[0] = null;
	}
	print_vec(orth_pos);

	// east
	orth_pos.x = curr_pos.x + 1;
	orth_pos.y = curr_pos.y;
	check = cell_in_matrix(orth_pos);
	if (check == 1) {
		directions[1] = k_matrix[(int)orth_pos.y][(int)orth_pos.x];
	} else {
		orth_pos = null;
		directions[1] = null;
	}
	print_vec(orth_pos);	

	// south
	orth_pos.x = curr_pos.x;
	orth_pos.y = curr_pos.y + 1;
	check = cell_in_matrix(orth_pos);
	if (check == 1) {
		directions[2] = k_matrix[(int)orth_pos.y][(int)orth_pos.x];
	} else {
		orth_pos = null;
		directions[2] = null;
	}
	print_vec(orth_pos);

	// west
	orth_pos.x = curr_pos.x - 1;
	orth_pos.y = curr_pos.y;
	check = cell_in_matrix(orth_pos);
	if (check == 1) {
		directions[3] = k_matrix[(int)orth_pos.y][(int)orth_pos.x];
	} else {
		orth_pos = null;
		directions[3] = null;
	}
	print_vec(orth_pos);

	// find the next move
	vec2 curr_kn = k_matrix[(int)curr_pos.y][(int)curr_pos.x];
	int closest = 0;
	int temp_closest;

	for (int i = 0; i < 4; i++) {
		if (directions[i].x != null.x) {
			
			if (directions[i].y <= curr_kn.x) {
				temp_closest = i;

				if (directions[i].y >= directions[temp_closest].y) {
					closest = i;
				}
			}
		}
	}

	vec2 new_pos = curr_pos;
	switch (closest) {
		case 0:
			new_pos.y -= 1;
			break;
		case 1:
			new_pos.x += 1;
			break;
		case 2:
			new_pos.y += 1;
			break;
		case 3:
			new_pos.x -= 1;
			break;
	}
	return new_pos;		
}


int main (void) {
	srand(time(NULL));

	float curr_stress;
	vec2 inputs = {0, 0};
	float volume, bpm, frequency, amplitude;

	// value used to see if current stress level is in the bound for the current
	// K(n) value
	int check;

	// all the K(n) values (x is lower bound, y is upper bound)
	struct vec2 k0 = {0, 10}, k1 = {11, 20}, k2 = {21, 30}, k3 = {31, 40},
				k4 = {41, 50}, k5 = {51, 60}, k6 = {61, 70}, k7 = {71, 80},
				k8 = {81, 90}, k9 = {91, 100};

	// all the amplitude values (0-1)
	float amplitudes[size] = {0.2, 0.4, 0.6, 0.8, 1};

	// all the frequency values (Hz)
	float frequencies[size] = {0.2, 0.3, 0.4, 0.5, 0.7};

	// the K matrix
	struct vec2 k_matrix[size][size] = {
		{k1, k2, k3, k7, k9},
		{k5, k4, k4, k6, k9},
		{k6, k5, k5, k6, k9},
		{k9, k8, k7, k7, k9},
		{k9, k9, k8, k8, k9}
	};

	// the starting position in the K matrix
	vec2 curr_pos = {4, 4}, temp_pos;

	// main loop
	 while (1) {
		inputs = receive_data();    	
		volume = inputs.x;
		bpm = inputs.y;

		curr_stress = calculate_stress(volume, bpm);   	

		if (curr_stress > 10) {
			check = stress_in_bound(curr_stress, k_matrix[(int)curr_pos.x][(int)curr_pos.y]);
			if (check == 0) {
				curr_pos = traverse(k_matrix, curr_pos);
			}
		}

		amplitude = amplitudes[(int)curr_pos.x];
		frequency = frequencies[(int)curr_pos.y];

		//transmit_data(amplitude, frequency);
		printf("Volume = %.2f, BPM = %.2f\nCurrent Stress = %.2f\nAmplitude = %.2f, Frequency = %.2f\npos = (%i, %i)\n", volume, bpm, curr_stress, amplitude, frequency, (int)curr_pos.x, (int)curr_pos.y);
		print_matrix(k_matrix, curr_pos);
		//sleep(1);
		//system("clear");
	}

	return 0;
}
