import time

from manager import QueueClient
from task import Task


class Boss(QueueClient):
    def __init__(self):
        super().__init__()

    def run(self, nb_task):
        # Add 30 tasks
        for i in range(nb_task):
            is_queue_full = self.task_queue.full()
            if not is_queue_full:
                self._add_task()
            else:
                print("Queue is full, can't add more task")
                break

        # Received data
        while 1:
            time.sleep(1)
            if not self.result_queue.empty():
                print("Received result task :")
                print(self.result_queue.get())

    def _add_task(self):
        task = Task(6000)
        self.task_queue.put(task)
        print("Task '" + str(task.identifier) + "' added!")


if __name__ == "__main__":
    boss = Boss()
    boss.run(15)
