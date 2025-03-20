import graphviz
def alg_mul(A: list[list[int]],B: list[list[int]])->list[list[int]]:
    C = [[0 for j in range(len(A))] for i in range(len(A))]
    for i in range(len(A)):
        for j in range(len(B)):
            sum = 0
            for k in range(len(A)):
                sum+=A[i][k]*B[k][j]
            C[i][j]=sum
    return C

def bool_mul(A: list[list[int]],B: list[list[int]])->list[list[int]]:
    am = alg_mul(A, B)
    C = [[int(am[i][j]!=0) for j in range(len(am))] for i in range(len(am))]
    return C

def tropical_mul(A: list[list[int]],B: list[list[int]])->list[list[int]]:
    C = [[0 for j in range(len(A))] for i in range(len(A))]
    for i in range(len(A)):
        for j in range(len(B)):
            sum = 1000000
            for k in range(len(A)):
                sum=min(sum, A[i][k]+B[k][j])
            C[i][j]=sum
    return C        

def draw_graph(M: list[list[int]], mode='usual'):
    if mode == 'Petersen':
        dot = graphviz.Graph(comment = 'Petersen', engine = 'neato')
        al = ['A', 'B','C','D','E','F','G','H','I','J']
        positions = {
            'A': "0,2!", 'B': "1.9,0.6!", 'C': "1.2,-1.6!", 'D': "-1.2,-1.6!", 'E': "-1.9,0.6!",
            'F': "0,1!", 'G': "1,0.3!", 'H': "0.6,-0.8!", 'I': "-0.6,-0.8!", 'J': "-1,0.3!"
        }
        for i in range(10):
            dot.node(al[i], str(i), pos=positions[al[i]])
            
        edgs = []  
        for i in range(len(M)):
            for j in range(i+1,len(M)):
                if M[i][j]:
                    edgs.append(al[i]+al[j])
        dot.edges(edgs)            
        dot.render('test.gv', view=True)
    else:
        dot = graphviz.Digraph(comment = 'Graph')
        al = [str(i) for i in range(len(M))]
        for i in range(len(al)):
            dot.node(al[i], al[i])
        edgs = [] 
        for i in range(len(M)):
            for j in range(len(M)):
                if M[i][j]:
                    edgs.append(al[i]+al[j])   
        dot.edges(edgs)            
        dot.render('test.gv', view=True)            



petersen = [
    [0, 1, 0, 0, 1, 1, 0, 0, 0, 0],  
    [1, 0, 1, 0, 0, 0, 1, 0, 0, 0],  
    [0, 1, 0, 1, 0, 0, 0, 1, 0, 0],  
    [0, 0, 1, 0, 1, 0, 0, 0, 1, 0],  
    [1, 0, 0, 1, 0, 0, 0, 0, 0, 1],  
    [1, 0, 0, 0, 0, 0, 0, 1, 1, 0],  
    [0, 1, 0, 0, 0, 0, 0, 0, 1, 1],  
    [0, 0, 1, 0, 0, 1, 0, 0, 0, 1],  
    [0, 0, 0, 1, 0, 1, 1, 0, 0, 0],  
    [0, 0, 0, 0, 1, 0, 1, 1, 0, 0]]   

a = [[0,1,1], 
     [1,0,1], 
     [0,1,0]]

one = petersen
two = alg_mul(petersen, petersen)
three = alg_mul(two, one)
four = alg_mul(three, one)
for row in two:
    print(row)

print('\n')
for row in three:
    print(row)
print('\n')

for row in four:
    print(row)    
draw_graph(petersen, 'Petersen')



 
            