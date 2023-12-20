import time

from manager import QueueClient


class Boss(QueueClient):
    def __init__(self):
        super().__init__()

    def run(self):
        while 1:
            time.sleep(5)
            is_queue_full = self.task_queue.full()
            if not is_queue_full:
                print("Task added!")
                self._add_task()
            else:
                print("Queue is full, can't add task")

    def _add_task(self):
        self.task_queue.put("task")


if __name__ == "__main__":
    boss = Boss()
    boss.run()
