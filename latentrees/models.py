from latentrees.layers.layer import layer
from latentrees.layers.first_layer import first_layer
                
class model():
    def __init__(self, L=50, nl=3, distribution=None):
        self.L = L
        self.nl = nl
        self.levels = []
        self.distribution = distribution
        
    def run(self) -> None:
        self.levels = [first_layer("l0", self.nl, distribution=self.distribution)]
        self.levels[0].sample()
        for il,l in enumerate(range(self.L)):
            if il % (self.L/10) == 0:
                print(il)
            new_layer = layer(f"l{il+1}", self.nl, distribution=self.distribution)
            self.levels.append(new_layer)
            new_layer.sample(self.layers[il])
            
    @property
    def layers(self):
        return self.levels
    
    def __repr__(self):
        return "model with {} layers and {} splits".format(self.L, self.nl)