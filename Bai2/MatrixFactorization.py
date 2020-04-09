import numpy

def check_valid_matrix(R):
    for i in range(len(R)):
        for j in range(len(R[0])):
            if(R[i][j].round() < 0 or R[i][j].round() > 5):
                return False
    return True


def matrix_factorization(R, K, beta=0.01):
    W = numpy.random.rand(len(R),K)
    H = numpy.random.rand(len(R[0]),K)
    H = H.T
    Ret = R
    step = 0
    stop = False
    while(not stop):
        for i in range(len(R)):
            for j in range(len(R[0])):
                if R[i][j] > 0:
                    eij = R[i][j] - numpy.dot(W[i,:],H[:,j])
                    for k in range(K):
                        W[i][k] += 2 * beta * (eij * H[k][j])
                        H[k][j] += 2 * beta * (eij * W[i][k])
        step += 1
        Ret = numpy.dot(W, H)
        if(step > 500):
            stop = True
    return Ret

with open('input.txt', 'r') as f:
    R = [[int(num) for num in line.split(' ')] for line in f if line.strip() != " " ]
R = numpy.array(R)
print("Rating array:")
print(R)
K = 3

nR = matrix_factorization(R, K).round()
print("Result: ")
print(nR)
