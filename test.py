import unittest

class latenttests(unittest.TestCase):
	def test_import(self):
		import latentrees
		self.assertIsNotNone(latentrees)
	
	def test_version(self):
		import latentrees
		self.assertEqual(latentrees.__version__[0],"0")
        
	def test_stop_iter(self):
		from latentrees import analyses
		runtime=analyses()
		with self.assertRaises(StopIteration):
			next(runtime) 

	def test_model_load(self):
		from latentrees import analyses
		from latentrees import model
		runtime=analyses()
		runtime.append_model(model())
		runtime.append_model(L=10)

		self.assertIsInstance(runtime[0],model)
		self.assertIsInstance(runtime[1],model)


if __name__ == "__main__":
	unittest.main()
