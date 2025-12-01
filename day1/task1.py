def count_zero_visits(lines, start=50):
    pos = start % 100
    zeros = 0
    for raw in lines:
        line = raw.strip()
        if not line:
            continue
        d = line[0].upper()
        try:
            dist = int(line[1:])
        except ValueError:
            raise ValueError(f"Bad line: {line!r}")
        if d == 'R':
            pos = (pos + dist) % 100
        elif d == 'L':
            pos = (pos - dist) % 100
        else:
            raise ValueError(f"Unknown direction {d!r} in line {line!r}")
        if pos == 0:
            zeros += 1
    return zeros

with open("input.txt") as f:
    lines = f.readlines()
print("Count:", count_zero_visits(lines))
