import math

alkuluvut = [2]
ei_alkuluvut = [1]
def on_alkuluku(luku):
    if luku in alkuluvut:
        return True
    if luku in ei_alkuluvut:
        return False
    for i in range(2, int(math.sqrt(luku) + 1)):
        if luku % i == 0:
     #       ei_alkuluvut.append(luku)
            return False
    alkuluvut.append(luku)
    return True

def permutaatiocheck(luku_str, start):
    luku_list = list(luku_str)
    for i in range(start, len(luku_list)):
        for j in range(i + 1, len(luku_list)):
            if luku_list[i] == luku_list[j]:
                continue
            luku_list[i], luku_list[j] = luku_list[j], luku_list[i]
            luku_str = ''.join(luku_list)
            if on_alkuluku(int(luku_str)):
                return luku_str
            check = permutaatiocheck(luku_str, start + 1)
            if check != 0:
                return check
            luku_list[i], luku_list[j] = luku_list[j], luku_list[i]
    return 0


def tee_alkuluku(luku, numerot):
    if int(luku) > 10 ** 16:
        return 0
    if on_alkuluku(int(luku)):
        return luku

    for i in range(len(numerot)):
        check = permutaatiocheck(luku, 0)
        if check != 0:
            return check;
        luku = luku + numerot[i]
        ret = tee_alkuluku(luku, numerot)
        if ret != 0:
            return ret;
        luku = luku[:-1]
    return 0

def main():
    t = int(input())
    tuloste = ""
    for i in range(t):
        k = int(input())
        rivi = input()
        numerot = rivi.replace(" ", "");
        alkuluku = int(tee_alkuluku(numerot, numerot))
        if alkuluku == 0:
            tuloste = tuloste + "NO\n"
        else:
            tuloste = tuloste + "YES\n"
            tuloste = tuloste + str(alkuluku) + "\n";
    print(tuloste)

main()
