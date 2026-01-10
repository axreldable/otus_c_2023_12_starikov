from collections import defaultdict

if __name__ == '__main__':
    with open("text.txt", "r") as f:
        text = f.read()
        words = text.split()
        d = defaultdict(lambda: 0)
        for w in words:
            d[w] += 1

    print(d)

    assert d["wisdom"] == 6
    assert d["river"] == 3
    assert d["lightning"] == 1
    assert d["snow"] == 2
    assert d["alpha"] == 3
