rng = np.random.default_rng()

class node():
    def __init__(self, name: str):
        self.name = name
        self.cnt = None
    
    def get_value(self):
        return self.cnt
        
    def __repr__(self):
        return self.name
    
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
    
class first_layer(layer): 
    def sample(self, previous_layer):
        raise NotImplementedError("First layer cannot have a previous layer")
            
    def sample(self) -> None:
        for node in range(self.nl):
            self.nodes.append(self.distribution(1))
            
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