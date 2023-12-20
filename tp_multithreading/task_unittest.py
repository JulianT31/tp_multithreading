import unittest

from task import Task


class TestStringMethods(unittest.TestCase):
    def test_unitaire_task(self):
        print("----test equal Task----")

        print("Init de la task a")
        a = Task()
        self.assertTrue(isinstance(a, Task))
        self.assertEqual(a.size, 6)
        self.assertEqual(a.a.dtype, "float64")
        self.assertEqual(a.b.dtype, "float64")
        self.assertEqual(a.x, None)
        self.assertEqual(a.time, None)

        print("Creation d'un text json à partir de la class a")
        txt = a.to_json()
        self.assertTrue(isinstance(txt, str))

        print("Creation d'une task b à partir du text json")
        b = Task.from_json(txt)
        self.assertEqual(type(b), Task)

        print("Verification de l'égalité des deux tasks")
        self.assertTrue(a == b)

        print("----test equal Task----")


if __name__ == "__main__":
    unittest.main()
