# //2, 4, 6, 10, 16, 26, 42, 68, 110, 178, 288, 466, 754, 1220, 1974, 3194, 5168, 8362

SIZE = 4
box = [0] * SIZE * SIZE
corners = [0, SIZE - 1, SIZE * (SIZE - 1), SIZE * SIZE - 1]
threats = [-2 * SIZE - 1, -2 * SIZE + 1, 2 - SIZE, 2 + SIZE, 2 * SIZE - 1, 2 * SIZE + 1, -2 + SIZE, -2 - SIZE]

def pr():
    for i in range(SIZE * SIZE):
        if i % SIZE == 0:
            print()
        print(box[i], end="")
    print("\n")
        

def mandis(ida, idb):
    x_a = ida % SIZE
    x_b = idb % SIZE
    y_a = ida // SIZE
    y_b = idb // SIZE
    return abs(x_a - x_b) + abs(y_a - y_b)

def count_threats(pos): # works is SIZE >= 4
    if pos % SIZE > 3 and pos % SIZE < SIZE - 5 and pos // SIZE > 3 and pos // SIZE < SIZE - 5:
        return 8
    magic = 8
    for i in range(4):
        dis = mandis(pos, corners[i])
        if dis == 0:
            return 2
        if dis == 1:
            return 3
        if dis == 2:
            magic = 4
        if dis == 3:
            magic = min(6, magic)
        if pos % SIZE == 0 or pos % SIZE == SIZE - 1:
            magic = 4
        if pos // SIZE == 0 or pos // SIZE == SIZE - 1:
            magic = 4
        if pos % SIZE == 1 or pos % SIZE == SIZE - 2:
            magic = min(6, magic)
        if pos // SIZE == 1 or pos // SIZE == SIZE - 2:
            magic = min(6, magic)
    return magic

def nope(i, j):
    row_diff = i // SIZE - j // SIZE
    col_diff = i % SIZE - j % SIZE
    return abs(row_diff) + abs(col_diff) == 3 and row_diff != 0 and col_diff != 0

def printbox(h1, h2):
    for i in range(SIZE * SIZE):
        if i % SIZE == 0:
            print()
        if i == h1 or i == h2:
            print("H", end="")
        elif nope(i, h1) or nope(i, h2):
            print("X", end="")
        else:
            print("-", end="")
    print("\n*******")


# fixes doubly removed mutual threats
# adds doubly removed threats left of h2
def mutual_and_readd(h1, h2):
    ret = 0
    for i in range(8):
        t1 = h1 + threats[i]
        if t1 < 0 or t1 >= SIZE *SIZE or mandis(t1, h1) != 3:
            t1 = -1
        t2 = h2 + threats[i]
        if t2 < 0 or t2 >= SIZE *SIZE or mandis(t2, h2) != 3:
            t2 = -1
            
        if t1 != -1:
            if nope(t1, h2): # fix mutual
                ret += 1
            elif t1 < h2: # if wasnt mutual, readd a threat left of h2
                ret += 1
        if t2 != -1:
            if t2 < h2: # readd a threat left of h2
                ret += 1

    return ret
    
    
def main():

    for i in range(SIZE * SIZE):
        box[i] = count_threats(i)
    
    possible = 0
      
    for i in range(SIZE * SIZE):
        print(f" Calculated: {(i * 100) / (SIZE * SIZE - 1):.1f}", end="\r")
        for j in range(i + 1, SIZE * SIZE):
            if (nope(i, j)):
                continue
            possible += SIZE * SIZE - box[i] - box[j] - j - 1 + mutual_and_readd(i, j)
            # printbox(i , j)
            # print(possible)
    print()
    print(SIZE, possible)
        
        
main()