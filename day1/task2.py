def count_zeros_during_movement(pos: int, dist: int, direction: int) -> int:
    # first step until the next time dial reaches 0
    if direction == 1:  # R
        first_step = (100 - pos) % 100
    else:  # L
        first_step = pos % 100

    # count how many hits occur at steps k where 1 <= k <= dist
    if first_step == 0:
        # next hit is at step 100
        if dist < 100:
            return 0
        return 1 + (dist - 100) // 100
    else:
        if dist < first_step:
            return 0
        return 1 + (dist - first_step) // 100

def main():
    with open("input.txt", "r") as f:
        lines = [line.strip() for line in f if line.strip()]

    pos = 50
    count_zero = 0

    for line in lines:
        dir_char = line[0]
        dist = int(line[1:])
        direction = 1 if dir_char == 'R' else -1

        # count hits during movement (includes a hit at the final click if it occurs)
        count_zero += count_zeros_during_movement(pos, dist, direction)

        # move to final position
        pos = (pos + direction * dist) % 100

    print(count_zero)

if __name__ == "__main__":
    main()