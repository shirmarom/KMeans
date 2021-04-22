import argparse


def getInput():
    arr_of_vectors = []
    while True:
        try:
            for line in input().split('\n'):
                arr_of_vectors.append([float(i) for i in line.split(',')])
        except EOFError:
            break
    return arr_of_vectors


def output(arr):
    for i in arr:
        i = ["%.2f" % j for j in i]
        newVec = ','.join(i)
        print(newVec)


def addVector(arr1, arr2):
    for i in range(len(arr1)):
        arr1[i] += arr2[i]
    return arr1


def kmeans(d=1, k=3, n=9, maxiter=100):
    vectors = getInput()
    arr1 = []
    ind = 0
    centroids = []
    changed = True
    for i in range(k):
        centroids.append(vectors[i].copy())
    while ind < maxiter and changed:
        arr1 = [[[0 for i in range(d)], 0] for j in range(k)]
        for i in vectors:
            belong = belongs(centroids, i)
            arr1[belong][0] = addVector(arr1[belong][0], i)
            arr1[belong][1] += 1
        changed = False
        for j in range(k):  # Calculating the new centroids ans checking whether they're changing
            currCent = centroidCalc(arr1[j])
            if currCent != centroids[j]:
                changed = True
            centroids[j] = currCent
        ind += 1
    output(centroids)


def dist(vec1, vec2):
    counter = 0
    for i in range(len(vec1)):
        counter += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i])
    return counter


def centroidCalc(cluster):
    newCentroid = []
    for i in cluster[0]:
        newCentroid.append(float(i / cluster[1]))
    return newCentroid


def belongs(arr, vector):
    minimum = dist(arr[0], vector)
    minI = 0
    for i in range(1, len(arr)):
        currDist = dist(arr[i], vector)
        if currDist < minimum:
            minI = i
            minimum = currDist
    return minI


parser = argparse.ArgumentParser()
parser.add_argument('K', type=int)
parser.add_argument('N', type=int)
parser.add_argument('d', type=int)
parser.add_argument('MAX_ITER', type=int)
args = parser.parse_args()

if args.K >= args.N or args.MAX_ITER < 1 or args.d < 1 or args.K < 1:
     print("The arguments you have given are invalid. Please make sure that: " + '\n')
     print("1. k <= n." + '\n')
     print("2. All arguments except n are at least 1, and natural numbers only.")
     exit(0)

kmeans(args.d, args.K, args.N, args.MAX_ITER)
