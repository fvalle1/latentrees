from latentrees.layers.layer import layer

class first_layer(layer): 
    """
    Use this as first layer of your model
    """
    def sample(self, previous_layer: layer):
        raise NotImplementedError("First layer cannot have a previous layer")
            
    def sample(self, first_mean = None) -> None:
        """
        Run the first sampling
        
        :param first_mean: center of distribution at first layer
        
        :return: None
        """
        if first_mean is None:
            first_mean = 1
        for node in range(self.nl):
            self.nodes.append(self.distribution(first_mean))
