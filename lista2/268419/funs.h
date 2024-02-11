#pragma once
#include <stdio.h>

int max(int x, int y) {
	return x < y ? y : x;
}

int min(int x, int y) {
	return x < y ? x : y;
}

void fisher_yates(int *arr, int n) {
     int i, j, tmp;
     for (i = n - 1; i > 0; i--) {
         j = rand() % (i + 1);
         tmp = arr[j];
         arr[j] = arr[i];
         arr[i] = tmp;
     }
}

typedef struct {
	int value;
	int heuristic;
} Move;
