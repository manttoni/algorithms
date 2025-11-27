koot = [2, 4, 6, 10, 16, 26, 42, 68, 110, 178, 288, 466, 754, 1220, 1974, 3194, 5168, 8362]

# How many spots does a knight threaten in each spot on the board?
def create_threatnumbers(n):
    threatnumbers = []
    for y in range(n):
        row = []
        for x in range(n):
            if y == 0 or y == n - 1:
                if x == 0 or x == n - 1:
                    row.append(2)
                elif x == 1 or x == n - 2:
                    row.append(3)
                else:
                    row.append(4)
            elif y == 1 or y == n - 2:
                if x == 0 or x == n - 1:
                    row.append(3)
                elif x == 1 or x == n - 2:
                    row.append(4)
                else:
                    row.append(6)
            else:
                if x == 0 or x == n - 1:
                    row.append(4)
                elif x == 1 or x == n - 2:
                    row.append(6)
                else:
                    row.append(8)
        threatnumbers.append(row)
    return threatnumbers

# How many spots does a knight threaten in each spot on the board?
# Don't count threats given to smaller indices than the knight itself
# , because then would count double later
def thing(n, a_threats):
    tn = []
    area = n ** 2
    moves = [[1,2],[2,1],[1,-2],[2,-1]]
    for y in range(n):
        row = []
        for x in range(n):
            row.append([])
            for move in moves:
                if y + move[0] < 0 or y + move[0] >= n:
                    continue
                if x + move[1] < 0 or x + move[1] >= n:
                    continue
                row[x].append(a_threats[y + move[0]][x + move[1]])
        tn.append(row)
    return tn


def print_tn(tn):
    for row in tn:
        for number in row:
            print(number, end="")
        print()


def solve(at, lt):
    illegal = 0
    width = len(at)
    size = width ** 2
    for y in range(width):
        for x in range(width):
            a_threats = at[y][x]
            l_threats = len(lt[y][x])
            # first and second horse threaten each other, but third threatens neither
            illegal = illegal + l_threats * (size - 1)
            for t in lt[y][x]:
                illegal = illegal - t
    all_possible = size * (size - 1) * (size - 2) // 6
    print(f"{width} {all_possible - illegal}")



def main():
    for n in koot:
        all_threats = create_threatnumbers(n)
        later_threats = thing(n, all_threats)
        solve(all_threats, later_threats)





main()
