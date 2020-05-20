import numpy as np

A = np.array([
    [1 / 1, 1 / 2, 3 / 1, 5 / 3, 2 / 5],
    [0 / 1, 0 / 1, 0 / 1, 0, 0],
    [0 / 3, 0 / 4, 0 / 1, 0, 0],    
    [0 / 3, 0 / 4, 0 / 1, 0, 0],    
    [0 / 3, 0 / 4, 0 / 1, 0, 0]
])
rows, cols = A.shape

#khoi tao chieu doc
for i in range(1, rows):
    for j in range(0, cols):
        if(j == 0):
            A[i][j] = 1 / A[0][i]
            continue
        elif(i == j):
            A[i][j] = 1
            continue
        else:
            A[i][j] = A[i][0] * A[0][j]

sum_col = A.sum(axis = 0)
print(A)
#tinh trong so
A1 = np.random.rand(rows, cols)
for i in range(0, rows):
    for j in range(0, cols):
        A1[i][j] = A[i][j] / sum_col[j]

sum_row1 = A1.sum(axis = 1)
sum_row_A1 = sum_row1.sum()
for i in range(0, rows):
    sum_row1[i] = sum_row1[i] / sum_row_A1
sum_row1 = sum_row1.round(2) #trong so

#ty so nhat quan
C = np.dot(A, sum_row1).round(2)
vector = np.random.rand(rows, 1)
for i in range(0, rows):
    vector[i] = (C[i] / sum_row1[i])
lamda = vector.sum() / rows

#tinh CI
CI = (lamda - rows) / (rows - 1)

RI = np.array([0, 0, 0.52, 0.89, 1.11, 1.25, 1.35, 1.4, 1.45, 1.49, 1.52, 1.54, 1.56, 1.58, 1.59])
CR = CI / RI[rows]
print(CR)