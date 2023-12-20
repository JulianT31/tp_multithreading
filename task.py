import json
import logging
import uuid
from time import perf_counter

import numpy as np


class Task:
    def __init__(self, size=6, identifier=None, a=None, b=None, x=None, time=None):
        self.identifier = uuid.uuid4() if identifier is None else identifier
        self.size = size
        self.a = np.random.rand(self.size, self.size) if a is None else a
        self.b = np.random.rand(self.size) if b is None else b
        self.x = None if x is None else x
        self.time = None if time is None else time

    def work(self):
        start = perf_counter()
        self.x = np.linalg.solve(self.a, self.b)
        end = perf_counter()

        self.time = end - start
        logging.debug("Task ended in " + str(self.time))
        return self.x

    def to_json(self):
        # Warning attributs values need to be in string
        dictionnary = {
            "identifier": self.identifier.__str__(),
            "size": self.size,
            "a": self.a.tolist(),
            "b": self.b.tolist(),
            "x": self.x,
            "time": self.time,
        }
        json_string = json.dumps(dictionnary)
        return json_string

    @staticmethod
    def from_json(txt):
        dict_data = json.loads(txt)
        new_task = Task(
            dict_data["size"],
            identifier=dict_data["identifier"],
            a=np.array(dict_data["a"]),
            b=np.array(dict_data["b"]),
            x=dict_data["x"],
            time=dict_data["time"],
        )
        return new_task

    def __eq__(self, other):
        if not isinstance(other, Task):
            return False

        return (
            self.size == other.size
            and np.equal(self.a, other.a).all()
            and np.equal(self.b, other.b).all()
            and np.equal(self.x, other.x)
            and self.time == other.time
        )


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
