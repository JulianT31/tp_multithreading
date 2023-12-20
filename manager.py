from multiprocessing import Queue
from multiprocessing.managers import BaseManager


class QueueManager(BaseManager):
    pass


class QueueClient:
    def __init__(self):
        QueueManager.register("task_queue")
        QueueManager.register("result_queue")
        manager = QueueManager(address=("localhost", 50000), authkey=b"abracadabra")
        manager.connect()
        print("Connected")

        self.task_queue = manager.task_queue()
        self.result_queue = manager.result_queue()


if __name__ == "__main__":
    task_queue = Queue()
    result_queue = Queue()
    QueueManager.register("task_queue", callable=lambda: task_queue)
    QueueManager.register("result_queue", callable=lambda: result_queue)
    m = QueueManager(address=("localhost", 50000), authkey=b"abracadabra")
    s = m.get_server()
    print("Server launched!")
    s.serve_forever()
