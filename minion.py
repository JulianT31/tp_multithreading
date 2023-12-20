import time

from manager import QueueClient


class Minion(QueueClient):
    def __init__(self):
        super().__init__()
        self.current_task = None

    def run(self):
        while 1:
            time.sleep(2)
            is_queue_empty = self.task_queue.empty()
            if not is_queue_empty:
                print("Task found")
                self._exec_task()
            else:
                print("Queue is empty")

    def _exec_task(self):
        self.current_task = self.task_queue.get()
        # self.current_task.work()
        time.sleep(0.5)

        self.result_queue.put("123")


if __name__ == "__main__":
    minion = Minion()
    minion.run()
