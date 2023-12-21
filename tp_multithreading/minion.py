from manager import QueueClient


class Minion(QueueClient):
    def __init__(self):
        super().__init__()
        self.current_task = None

    def run(self):
        while 1:
            is_queue_empty = self.task_queue.empty()
            if not is_queue_empty:
                self._exec_task()

    def _exec_task(self):
        self.current_task = self.task_queue.get()
        print("Beginning executing task " + str(self.current_task.identifier))
        self.current_task.work()
        print("Task finished!")
        self.result_queue.put(self.current_task)


if __name__ == "__main__":
    minion = Minion()
    minion.run()
