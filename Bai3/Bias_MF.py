import numpy

def matrix_factorization_Bias(R, K, steps=5000, beta=0.0004, lamda=0.008):
    user = len(R)
    item = len(R[0])
    u = numpy.sum(R) / (user * item - numpy.count_nonzero(R == 0))
    bs = numpy.arange(user, dtype='float')
    sum_rate_s = numpy.sum(R, axis=1)
    for s in range(user):
        ds = numpy.count_nonzero(R[s, :] > 0)
        bs[s] = (sum_rate_s[s] - ds * u) / ds

    bi = numpy.arange(item, dtype='float')
    sum_rate_i = numpy.sum(R, axis=0)
    for s in range(item):
        di = numpy.count_nonzero(R[:, s] > 0)
        bi[s] = (sum_rate_i[s] - di * u) / di

    W = numpy.random.rand(user,K)
    H = numpy.random.rand(K, item)

    for s in range(steps):
        for i in range(user):
            for j in range(item):
                if R[i][j] > 0:
                    pij = u + bs[i] + bi[j] + numpy.dot(W[i,:], H[:, j]) 
                    eij = R[i][j] - pij
                    u += (beta * eij)
                    bs[i] += beta * (eij - lamda * bs[i])
                    bi[j] += beta * (eij - lamda * bi[j])
                    for k in range(K):
                        W[i][k] += beta * (2 * eij * H[k][j] - lamda * W[i][k])
                        H[k][i] += beta * (2 * eij * W[i][k] - lamda * H[k][j])

    Ret = numpy.dot(W, H)
    for i in range(user):
        for j in range(item):
            Ret[i][j] += u + bs[i] + bi[j]

    return Ret.round(), bs, bi, u

with open('input.txt', 'r') as f:
    R = [[int(num) for num in line.split(' ')] for line in f if line.strip() != " " ]
R = numpy.array(R)
print("Rating array:")
print(R)
K = 4

nR, bs, bi, u = matrix_factorization_Bias(R, K, 5000, 0.0002)

for i in range(len(nR)):
    for j in range(len(nR[0])):
        if(nR[i][j] > 5): nR[i][j] = 5
print("Result: ")
print(nR)