class node():
    def __init__(self, name: str):
        self.name = name
        self.cnt = None
    
    def get_value(self):
        return self.cnt
        
    def __repr__(self):
        return self.name