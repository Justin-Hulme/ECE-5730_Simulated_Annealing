from dataclasses import asdict, fields, dataclass
import subprocess
import csv
import re
import time

input_files = [
    "input",
    "input2",
    "input3",
]


@dataclass
class Row:
    file: str
    cooling_rate: float
    score: int
    num_iterations: int
    duration: time

def float_range(start, stop, step):
    n = 0
    while True:
        value = start + n * step
        if value >= stop:
            break
        yield round(value, 10)
        n += 1

data: list[Row] = []

for cooling_rate in float_range(0.5, 1, 0.00005):
    for file in input_files:
        print (f"Running {file:6} with cooling rate {cooling_rate:.5f}")

        start_time = time.time()
        result = subprocess.run(
            ["./automate", f"examples_inputs/{file}.txt", "crap.txt", str(10_000_000.0), str(cooling_rate), str(0.000_01)],
            capture_output=True,
            text=True,
            check=True,
        )
        end_time = time.time()

        output = result.stdout
        match = re.match(r'(\d+)\s+(\d+)', output)

        if match:
            score = int(match.group(1))
            iterations = int(match.group(2))
            grid = output[match.end():].strip()

        row = Row(file, cooling_rate, score, iterations, end_time-start_time)
        data.append(row)

        with open(f"grids/{file}/{cooling_rate:.5f}.txt", "w") as file:
            file.write(grid)

with open("output.csv", "w", newline="") as f:

    writer = csv.DictWriter(
        f,
        fieldnames=[field.name for field in fields(Row)]
    )

    writer.writeheader()

    for row in data:
        writer.writerow(asdict(row))