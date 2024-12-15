from typing import List

class Prod:
    def __init__(self, left: int, right: List[int]):
        self.left = left
        self.right = right
        
    def __str__(self):
        return f"{self.right} -> {', '.join(map(str, self.left))}"
    
    def __repr__(self):
        return str(self)
    
    
        