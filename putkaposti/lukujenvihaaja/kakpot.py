def kakpot(kp):
    n = 0
    k = 0
    while k < kp:
        n = n * 2 + 2 ** k
        k += 1
    return n

def tv(n):
    v = 0
    lasketut = 0
    kp = 0
    while lasketut + 2 ** kp <= n:
        lasketut += 2 ** kp
        kp += 1
    v += kakpot(kp)
    jaljella = n - lasketut
    
    i = 0
    while jaljella != 0:
        kp = 0
        while 2 ** kp <= jaljella:
            kp += 1
        v += kakpot(kp) - kakpot(kp - 1) + i * (2 ** kp - 2 ** (kp - 1))
        lasketut += 2 ** (kp - 1)
        jaljella = n - lasketut
        i += 1

    return v

num = "99999999999999999999999999999999999999999999999999999999999999999999999999999999999999"
n = len(num)
for i in range(n):
    luku = int(num[:i + 1])
    print(f"{luku} {tv(luku)}")