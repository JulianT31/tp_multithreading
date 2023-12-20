import logging
import uuid
from time import perf_counter

import numpy as np


class Task:
    def __init__(self, size):
        self.identifier = uuid.uuid4()
        self.size = size
        self.a = np.random.rand(self.size, self.size)
        self.b = np.random.rand(self.size)
        self.x = None
        self.time = None

    def work(self):
        start = perf_counter()
        self.x = np.linalg.solve(self.a, self.b)
        end = perf_counter()

        self.time = end - start
        logging.debug("Task ended in " + str(self.time))


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)

    Task(6000).work()
