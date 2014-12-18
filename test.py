import unittest
import festival

class FestivalTest(unittest.TestCase):

    def test_say_file(self):

        # test wrapper
        self.assertTrue(festival.sayFile("tests/test_file.txt"))
        self.assertRaises(ValueError, festival.sayFile, "invalid_file.extension")

        # test C module
        self.assertTrue(festival._festival.sayFile("tests/test_file.txt"))
        self.assertFalse(festival._festival.sayFile("invalid_file.extension"))


if __name__ == '__main__':
    unittest.main()
