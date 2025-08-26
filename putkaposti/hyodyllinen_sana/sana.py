def read_words():
    words = []
    with open("kotus_sanat.txt", encoding="latin-1") as file:
        for line in file:
            words.append(line.strip())
    return sorted(words, key=len, reverse=True)

def main():
    words = read_words()
    groups = {}
    grouped_ix = set()
    for i in range(0, len(words)):
        if i in grouped_ix:
            continue
        groups[words[i]] = [words[i]]
        for j in range(i + 1, len(words)):
            if len(words[i]) == len(words[j]):
                continue;
            if words[j] in words[i]:
                groups[words[i]].append(words[j])
                grouped_ix.add(j);
    longest = []
    for base, sub in groups.items():
        if len(longest) < len(sub):
            longest = groups[base]
    for word in longest:
        print(word)

main()

