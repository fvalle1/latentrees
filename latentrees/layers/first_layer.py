from latentrees.layers.layer import layer

class first_layer(layer): 
    def sample(self, previous_layer):
        raise NotImplementedError("First layer cannot have a previous layer")
            
    def sample(self, first_mean = 1) -> None:
        for node in range(self.nl):
            self.nodes.append(self.distribution(first_mean))
