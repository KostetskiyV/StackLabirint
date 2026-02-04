import sys

GENERATION_FILE = "labirint.txt"
N = 1000

n = N if len(sys.argv) == 1 else int(sys.argv[1])
m = n if len(sys.argv) <= 2 else int(sys.argv[2])
if (n < 5 or m < 5):
    exit()

with open(GENERATION_FILE, 'w') as file:
    file.write(f"{n} {m}\n")
    file.write("1" + " 0" * (m-1) + "\n")
    direction = False
    for i in range(n-2):
        if i % 2 == 0:
            if direction:
                file.write("0" + " -1" * (m-1) + "\n")
            else:
                file.write("-1 " * (m-1) + "0\n")
        else:
            file.write("0 " * (m-1) + "0\n")
            direction = not direction
    if n % 2 == 0:
        if direction:
            file.write("2" + " -1" * (m-1) + "\n")
        else:
            file.write("-1 " * (m-1) + "2\n")
    else:
        if direction:
            file.write("0 " * (m-1) + "2\n")
        else:
            file.write("2" + " 0" * (m-1) + "\n")