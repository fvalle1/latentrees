from latentrees.models import model

class analyses():
    def __init__(self):
        self.models = []
        
    def run(self) -> None:
        def run_model(model):
            print("Running ", model)
            model.run()
        list(map(run_model, self.models))
    
    def __getitem__(self, item):
        return self.models[item]
    
    def __len__(self):
        return len(self.models)
    
    def __repr__(self):
        return f"analysis with {self.__len__()} models: {[model.__repr__() for model in self.models]}"