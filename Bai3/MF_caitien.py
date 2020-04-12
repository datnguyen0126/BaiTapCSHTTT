import numpy

def matrix_factorization(R, K, steps=1000, beta=0.004, lamda=0.02):
    W = numpy.random.rand(len(R),K)
    H = numpy.random.rand(K, len(R[0]))
    Ret = R
    step = 0
    stop = False
    while(not stop):
        for i in range(len(R)):
            for j in range(len(R[0])):
                if R[i][j] > 0:
                    eij = R[i][j] - numpy.dot(W[i,:],H[:,j])
                    for k in range(K):
                        W[i][k] += beta * (2 * eij * H[k][j] - lamda * W[i][k])
                        H[k][j] += beta * (2 * eij * W[i][k] - lamda * H[k][j])
        step += 1
        Ret = numpy.dot(W, H)
        if(step > steps):
            stop = True
    return Ret

with open('input.txt', 'r') as f:
    R = [[int(num) for num in line.split(' ')] for line in f if line.strip() != " " ]
R = numpy.array(R)
print("Rating array:")
print(R)
K = 4

nR = matrix_factorization(R, K, 5000, 0.0002).round()

for i in range(len(nR)):
    for j in range(len(nR[0])):
        if(nR[i][j] > 5): nR[i][j] = 5
print("Result: ")
print(nR)