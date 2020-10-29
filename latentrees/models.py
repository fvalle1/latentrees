import logging
logger = logging.getLogger("latentrees")
from latentrees.layers.layer import layer
from latentrees.layers.first_layer import first_layer
                
class model():
    """
    A model
    """
    def __init__(self, L=50, nl=3, distribution=None, first_mean=None, name="model"):
        self.L = L
        self.nl = nl
        self.levels = []
        self.distribution = distribution
        self.first_mean = first_mean
        self.name=name
        
    def run(self) -> None:
        """
        Run the model
        
        :return: None
        """
        self.levels = [first_layer("l0", self.nl, distribution=self.distribution)]
        self.levels[0].sample(self.first_mean)
        for il,l in enumerate(range(self.L)):
            if il % (self.L/10) == 0:
                logger.info(il)
            new_layer = layer(f"l{il+1}", self.nl, distribution=self.distribution)
            self.levels.append(new_layer)
            new_layer.sample(self.layers[il])
            
    @property
    def layers(self):
        """
        :return: layers
        """
        return self.levels
    
    def __repr__(self):
        return "{} with {} layers and {} splits".format(self.name,self.L, self.nl)