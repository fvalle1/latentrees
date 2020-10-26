from latentrees.nodes import node
import numpy as np

rng = np.random.default_rng()

class layer():
    def __init__(self, name : str, nl: int, distribution = None):
        global rng
        self.nl = nl
        self.nodes = []
        self.name = name
        self.rng = rng
        self.max_N_leaves = 10000
        def default_distribution(node:node):
            m = node + 2
            n = m/(m-1)
            p = 1/m
            return self.rng.negative_binomial(n,p)
        if distribution is None:
            self.distribution = default_distribution
        else:
            self.distribution =  distribution
    
    def sample(self, previous_layer) -> None:
        if len(previous_layer) < self.max_N_leaves:
            previous_nodes = previous_layer.nodes
        else:
            previous_nodes = np.random.choice(previous_layer.nodes, size=self.max_N_leaves, replace=False)
        for node in previous_nodes:
            for _ in range(self.nl):
                self.nodes.append(self.distribution(node))
    @property
    def sorted_nodes(self):
        return np.sort(self.nodes)[::-1]
    
    @property
    def average(self):
        return np.average(self.nodes)
    
    @property
    def median(self):
        return np.median(self.nodes)
      
    def __len__(self):
        return len(self.nodes)
   
    def __repr__(self):
        return f"{self.name}, nodes: {self.nodes}"