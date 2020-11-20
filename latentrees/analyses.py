import logging,sys
logger = logging.getLogger("latentrees")
hdl = logging.StreamHandler(sys.stdout)
hdl.setFormatter(logging.Formatter('(%(module)s) - [%(levelname)s]: %(message)s'))
hdl.setLevel(logging.INFO)
logger.addHandler(hdl)
logger.setLevel(logging.INFO)
from latentrees.models import model

class analyses():
    """
    Class to perform analysis on multiple models
    """
    def __init__(self):
        """
        
        Example
        =========
        
        runtime = analyses()
        runtime.append_model(model())
        runtime.run()
        """
        self.models = []
        self.iterindex = -1
        
    def run(self) -> None:
        """
        Run all the models stored
        """
        def run_model(model):
            try:
                logger.info("Running {}".format(model))
                model.run()
                return True
            except:
                print(*sys.exc_info())
                return False
            
        list(map(run_model, self.models))
        
    def model_generator(self):
        for model in self.models:
            yield model
            
    def append_model(self, *args, **kwargs)->None:
        """
        Add a model to the analyses
        Parameters
        
        :param model: a model object
        :param *args: to create a new model and append it
        :return: None
        """
        if len(args) > 1:
            if isinstance(args[0], model):
                self.models.append(args[0])
        else:
            self.models.append(model(*args, **kwargs))
        
        
    def __iter__(self):
        self.iterindex = -1
        return self
    
    def __next__(self):
        self.iterindex += 1
        if self.iterindex < len(self.models):
            return self.models[self.iterindex]
        raise StopIteration
    
    def __getitem__(self, item):
        if isinstance(item, int):
            return self.models[item]
        elif isinstance(item, str):
            for model in self.models:
                if model.name == item:
                    return model
                
    def __contains__(self, item):
        return item in [m.name for m in self.models]
    
    def __len__(self):
        return len(self.models)
    
    def __repr__(self):
        return f"analysis with {self.__len__()} models: {[model.__repr__() for model in self.models]}"