#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int K, N, d, MAX_ITER;

float ** getInput();
float dist(float * vec1, float * vec2);
float ** kmeans(int k, int N, int d, int MAX_ITER);
int belongs (float ** centroids, float * vec);
float * addVectors(float * vec1,float * vec2);
float * centroidCalc(float * vec, int member);
void output(float ** centroids);

int main(int argc, char* argv[]) {
    int i;
    float ** centroids;
    assert(argc >= 5);
    K = atoi(argv[1]);
    N = atoi(argv[2]);
    d = atoi(argv[3]);
    MAX_ITER = atoi(argv[4]);
    if (K >= N || MAX_ITER < 1 || d < 1 || K < 1 || N < 1) {
        printf("The arguments you have given are invalid."
               "Please make sure that k <= n and all arguments are >= 1.");
        exit(-1);
    }
    centroids = kmeans(K, N, d, MAX_ITER);
    output(centroids);
    for (i = 0; i < K; i++) {
        free(centroids[i]);
    }
    free(centroids);
    return 0;
}

float ** getInput() {
    float num;
    char c;
    float ** vectors;
    float * currVector;
    int i,j;
    int last;
    currVector = (float *)malloc(d * sizeof(float));
    assert(currVector != NULL);
    vectors = (float **)malloc(N * sizeof(float *));
    assert(vectors != NULL);
    i = 0;
    j = 0;
    while(scanf("%f%c", &num, &c) == 2) {
        if (c != '\n') {
            currVector[i] = num;
            i += 1;
            last = 1;
        } else {
            currVector[i] = num;
            i = 0;
            vectors[j] = currVector;
            j += 1;
            currVector = (float *) calloc(d, sizeof(float));
            last = 0;
            assert(currVector != NULL);
        }
    }
    if (last == 1) {
        vectors[j] = currVector;
    }
    else {
        free(currVector);
    }
    return vectors;
}

float dist (float * vec1, float * vec2){
    float counter;
    int i;
    counter=0;
    for(i = 0; i < d; i++){
        counter += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
    }
    return counter;

}

int belongs (float ** centroids, float * vec){
    int minInd, i;
    float minDist, currDist;
    minInd=0;
    minDist=dist(centroids[0],vec);
    for (i = 1; i < K; i++){
        currDist = dist(centroids[i], vec);
        if (currDist < minDist){
            minInd= i;
            minDist = currDist;
        }

    }
    return minInd;
}

float * addVectors(float * vec1,float * vec2){
    int i;
    for(i = 0; i < d; i++){
        vec1[i] += vec2[i];
    }
    return vec1;

}


float * centroidCalc(float * vec, int member){
    int i;
    for(i = 0; i < d; i++){
        vec[i] = (vec[i] / member);
    }
    return vec;
}

void output(float ** centroids) {
    int i, j;
    for (i = 0; i < K; i++) {
        for (j = 0; j < d; j++) {
            printf("%.2f", centroids[i][j]);
            if (j < d - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

float ** kmeans(int k, int N, int d, int MAX_ITER) {
    float** vectors;
    int ind, changed, g, n, m, y, i, j, f, t, s, r, p;
    int * members;
    float ** sumVectors;
    float **centroids;
    vectors= getInput();
    centroids = (float **) malloc(k * sizeof(float *));
    assert(centroids != NULL);
    for (s = 0; s < k; s++) {
        centroids[s] = (float *) calloc(d, sizeof(float));
        assert(centroids[s] != NULL);
    }
    ind=0;
    changed=0;
    for (t = 0; t < k; t++) {
        for (r = 0; r < d; r++){
            centroids[t][r] = vectors[t][r];
        }
    }
    members = (int *) calloc(k, sizeof(int));
    assert(members != NULL);
    sumVectors = (float **)malloc(k * sizeof(float *));
    assert(sumVectors != NULL);
    for (g = 0; g < k; g++) {
        sumVectors[g]=(float *) calloc(d, sizeof(float));
        assert(sumVectors[g] != NULL);
    }
    while(ind < MAX_ITER && changed == 0) {
        for (n = 0; n < N; n++) {
            int belong = belongs(centroids, vectors[n]);
            members[belong] += 1;
            sumVectors[belong] = addVectors(sumVectors[belong], vectors[n]);
        }
        changed=1;
        for (m = 0; m < K; m++) {
            float *currCent = centroidCalc(sumVectors[m], members[m]);
            for (f = 0; f < d; f++) {
                if (currCent[f] != centroids[m][f]) {
                    changed = 0;
                    break;
                }
            }
            for (y = 0; y < d; y++) {
                centroids[m][y] = currCent[y];
            }
        }
        for (j = 0; j < K; j++) {
            members[j] = 0;
            for(i=0;i<d;i++){
                sumVectors[j][i] = 0;
            }
        }
        ind += 1 ;
    }
    free(members);
    for (p = 0; p < N; p++) {
        free(vectors[p]);
        if (p < k) {
            free(sumVectors[p]);
        }
    }
    free(sumVectors);
    free(vectors);
    return centroids;
}