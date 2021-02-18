import timeit
from typing import Callable, List
from dataclasses import dataclass


@dataclass
class Benchmark:
    name: str
    timer: Callable[[int], List[float]]


def time_regex(repeat: int) -> List[float]:
    return timeit.repeat(
        "re.match(pattern, 'a o! b, 123abc')",
        "import re; pattern = re.compile(r'a (.*?)! b, (.*?)')",
        number=repeat
    )


def time_macro(repeat: int) -> List[float]:
    return timeit.repeat(
        "pattern.check('a o! b, 123abc')",
        "from macro import Pattern; pattern = Pattern('a (.*?)! b, (.*?)')",
        number=repeat
    )


TIMERS = [
    Benchmark("Python Regex", time_regex),
    Benchmark("Macro CPython API", time_macro),
]

for benchmark in TIMERS:
    results = benchmark.timer(1000000)
    print(benchmark.name, round(sum(results)/len(results), 5))
