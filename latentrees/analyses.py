from latentrees.models import model

class analyses():
    def __init__(self):
        self.models = []
        self.iterindex = -1
        
    def run(self) -> None:
        import multiprocessing as mp
        def run_model(model):
            try:
                print("Running ", model)
                model.run()
                return True
            except:
                return False
            
        list(map(run_model, self.models))
        
    def model_generator(self):
        for model in self.models:
            yield model
        
    def __iter__(self):
        self.iterindex = -1
        return self
    
    def __next__(self):
        self.iterindex += 1
        if self.iterindex < len(self.models):
            return self.models[self.iterindex]
        raise StopIteration
    
    def __getitem__(self, item):
        return self.models[item]
    
    def __len__(self):
        return len(self.models)
    
    def __repr__(self):
        return f"analysis with {self.__len__()} models: {[model.__repr__() for model in self.models]}"