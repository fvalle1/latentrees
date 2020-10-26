import unittest

class latenttests(unittest.TestCase):
	def test_import(self):
		import latentrees
		self.assertIsNotNone(latentrees)
	
	def test_version(self):
		import latentrees
		self.assertEqual(latentrees.__version__[0],"0")
if __name__ == "__main__":
	unittest.main()
