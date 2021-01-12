class node():
    def __init__(self, name: str):
        """
        :param str name: name of the node
        """
        self.name = name
        self.cnt = None
    
    def get_value(self):
        """
        :return: count of the node
        """
        return self.cnt
        
    def __repr__(self):
        return self.name