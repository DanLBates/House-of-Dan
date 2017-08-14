''' This tries to solve a question of How to obtain a specified amount of liquid
    in any one of three buckets or any combination of buckets.

    The method is a breadth first search.

     To start we have the capacity of the bucket A_capacity, B_capacity and
     C_capacity. The units don't matter as long as every specified amount is in
     the same units.
     We also have the initial amount in each bucket A_has, B_has and C_has
     the last specified quantity is the amount of liquid we desire.

     The initial parameters are put into a list.
     we traverse the list generating new states.
     To get a new state we can perform any one of the following:
       Pour out A making A_has = 0
       Pour out B making B_has = 0
       Pour out C making C_has = 0
       Fill A completely making A_has = A_capacity
       Fill B completely making B_has = B_capacity
       Fill C completely making C_has = C_capacity

       if A_has >= B_capacity - B_has we fill B from A
           A_has = A_has - (B_capacity - B_has); B_has = B_capacity
       if A_has >= C_capacity - C_has we fill C from A
           A_has = A_has - (C_capacity - C_has); C_has = C_capacity
       if B_has >= C_capacity - C_has we fill C from A
           B_has = B_has - (C_capacity - C_has); C_has = C_capacity
       if B_has >= A_capacity - A_has we fill A from B
           B_has = B_has - (A_capacity - A_has); A_has = A_capacity
       if C_has >= A_capacity - A_has we fill A from C
           C_has = C_has - (A_capacity - C_has); A_has = A_capacity
       if C_has >= B_capacity - C_has we fill B from C
           C_has = C_has - (B_capacity - B_has); B_has = B_capacity

       if A_has < B_capacity - B_has we fill B from A
           B_has = A_has + B_has; A_has = 0
       if A_has < C_capacity - C_has we fill C from A
           B_has = A_has + C_has; A_has = 0
       if B_has < C_capacity - C_has we fill C from B
           C_has = B_has + C_has; B_has = 0
       if B_has < A_capacity - A_has we fill A from B
           A_has = A_has + B_has; B_has = 0
       if C_has < A_capacity - A_has we fill A from C
           A_has = A_has + C_has; C_has = 0
       if C_has < B_capacity - N_has; C_has=0

       To reduce to 2 Bucket problem make C_capacity = 0

       A new generated state is never appended to the list if a state
           with A_has, B_has and C_has is already on the list (We want shortest)
'''

def PrintSolution(i):               #traverse to the beginning and back up
    backindx, msg, a, b, c = List[i]
    if i > 0:                       # printing the text and the state
        PrintSolution(backindx)
    print(msg)
    print(a, b, c)


def Solved (index):                 #if any bucket or any sum of buckets
    bl,msg,A,B,C = List[index]      #contians the goal then we are done
    if goal is A:
        return True
    if goal is B:
        return True
    if goal is C:
        return True
    if goal is (A + B):
        return True
    if goal is (B + C):
        return True
    if goal is (C + A):
        return True
    if goal is (A + B + C):
        return True
    return False

def MaybeAdd(listelement):
    indx, msg, Can_A, Can_B, Can_C = listelement
    for le in List:
        indx, msg, A, B, C = le     #never put a state we already generated
        if A != Can_A:              #on the list again. 1) there is a
            continue                #shorter way to get to that state and 2)
        if B != Can_B:              # we may loop only terminating when
            continue                # forced to, or by lack of resources.
        if C != Can_C:
            continue
        return
    List.append(listelement)

def Generate (indx):
    bl,msg,A,B,C = List[indx]       #try all we can to from indx state
    if A > 0:
        MaybeAdd((indx, "Pour Out A", 0, B, C))
    if B > 0:
        MaybeAdd((indx, "Pour Out B", A, 0, C))
    if C > 0:
        MaybeAdd((indx, "Pour Out C", A, B, 0))
    if A < A_capacity:
        MaybeAdd((indx, "Fill Up A", A_capacity, B, C))
    if B < B_capacity:
        MaybeAdd((indx, "Fill up B", A, B_capacity, C))
    if C < C_capacity:
        MaybeAdd((indx, "Fill up C", A, B, C_capacity))
    A_left = A_capacity - A
    B_left = B_capacity - B
    C_left = C_capacity - C
    if A > 0:
        if B_left > 0:
            if A >= B_left:
                MaybeAdd((indx, "Pour A into B", (A - B_left), B_capacity, C))
            else:
                MaybeAdd((indx, "Pour A into B", 0, (A + B), C))
        if C_left > 0:
            if A > C_left:
                MaybeAdd((indx, "Pour A into C", (A - C_left), B, C_capacity))
            else:
                MaybeAdd((indx, "Pour A into C", 0, B, (A + C)))
    if B > 0:
        if C_left > 0:
            if B > C_left:
                MaybeAdd((indx, "Pour B into C", A, (B - C_left), C_capacity))
            else:
                MaybeAdd((indx, "Pour B into C", A, 0, (B + C)))
        if A_left > 0:
            if B > A_left:
                MaybeAdd((indx, "Pour B into A", A_capacity, (B - A_left), C))
            else:
                MaybeAdd((indx, "Pour B into A", (A + B), 0, C))
    if C > 0:
        if A_left > 0:
            if C > A_left:
                MaybeAdd((indx, "Pour C into A", A_capacity, B, (C - A_left)))
            else:
                MaybeAdd((indx, "Pour C into A", (A + C), B, 0))
        if B_left > 0:
            if C > B_left:
                MaybeAdd((indx, "Pour C into B", A, B_capacity, (C - B_left)))
            else:
                MaybeAdd((indx, "Pour C into B", A, (B + C), 0))

A_capacity = int(input("What is the capacity of bucket A:"))
B_capacity = int(input("What is the capacity of bucket B:"))
C_capacity = int(input("What is the capacity of bucket C:"))
A_has = int(input("How much in bucket A initially:"))
B_has = int(input("How much in bucket B initially:"))
C_has = int(input("How much in bucket C initially:"))
goal  = int(input("How much liquid is the goal:"))
#first item is Backlink
List = [(0, "Initial State", A_has, B_has, C_has)]
i = 0;
k = len(List)
Solution = False
while i < k:
    if(Solved(i)):
        Solution = True
        break;
    Generate(i)
    i += 1
    k = len(List)
if Solution:
    PrintSolution(i)
else:
    print("There is no solution to this!")
