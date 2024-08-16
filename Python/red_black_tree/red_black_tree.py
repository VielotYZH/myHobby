from enum import Enum, auto, unique

@unique
class Color(Enum):
    RED = auto()
    BLACK = auto()

class Node():
    def __init__(self, f_addr, value, l_addr, r_addr, color = Color.RED):
        self.f_addr = f_addr
        self.value = value
        self.l_addr = l_addr
        self.r_addr = r_addr
        self.color = color

    def turnLeft(self):
        r_addr = self.r_addr
        r_addr.f_addr = self.f_addr
        self.f_addr = r_addr
        self.r_addr = r_addr.l_addr
        r_addr.l_addr = self
        if self.r_addr != None:
            self.r_addr.f_addr = self
        if r_addr.f_addr != None:
            if r_addr.f_addr.l_addr == self:
                r_addr.f_addr.l_addr = r_addr
            else:
                r_addr.f_addr.r_addr = r_addr
            return None
        else:
            return r_addr
        
    def turn_left(self, tree):
        tmp = self.turnLeft()
        if tmp != None:
            tree.root = tmp
    
    def turnRight(self):
        l_addr = self.l_addr
        l_addr.f_addr = self.f_addr
        self.f_addr = l_addr
        self.l_addr = l_addr.r_addr
        l_addr.r_addr = self
        if self.l_addr != None:
            self.l_addr.f_addr = self
        if l_addr.f_addr != None:
            if l_addr.f_addr.l_addr == self:
                l_addr.f_addr.l_addr = l_addr
            else:
                l_addr.f_addr.r_addr = l_addr
            return None
        else:
            return l_addr
        
    def turn_right(self, tree):
        tmp = self.turnRight()
        if tmp != None:
            tree.root = tmp

    def balance(self, tree):
        if self.f_addr.color == Color.RED:
            father_node = self.f_addr
            grandfather_node = father_node.f_addr
            uncle_node = grandfather_node.r_addr if grandfather_node.l_addr == father_node else grandfather_node.l_addr
            if uncle_node != None and uncle_node.color == Color.RED:
                father_node.color = Color.BLACK
                uncle_node.color = Color.BLACK
                if grandfather_node.f_addr != None:
                    grandfather_node.color = Color.RED
                    grandfather_node.balance(tree)
            else:
                if self == father_node.r_addr:
                    father_node.turn_left(tree)
                    father_node.balance(tree)
                else:
                    father_node.color = Color.BLACK
                    grandfather_node.color = Color.RED
                    grandfather_node.turn_right(tree)

class Tree():
    def __init__(self, root = None):
        self.root = root

    def get_color(self, color):
        match color:
            case Color.RED:
                return "r"
            case Color.BLACK:
                return "b"
            
    def printTree(self, node, level):
        if node == None:
            return
        if node.r_addr != None:
            self.printTree(node.r_addr, level + 1)
        for i in range(level):
            print("\t", end="")
        print(f"{node.value}{self.get_color(node.color)}")
        if node.l_addr != None:
            self.printTree(node.l_addr, level + 1)
    
    def print_tree(self):
        level = 0
        self.printTree(self.root, level)

    def add_node_to_tree(self, value):
        node = Node(None, value, None, None)
        p = self.root
        while True:
            if value < p.value:
                if p.l_addr != None:
                    p = p.l_addr
                else:
                    p.l_addr = node
                    node.f_addr = p
                    return node
            elif value > p.value:
                if p.r_addr != None:
                    p = p.r_addr
                else:
                    p.r_addr = node
                    node.f_addr = p
                    return node
            else:
                return None

def main():
    tree = Tree()
    with open(r"Python\red_black_tree\text.txt") as f:
        value = f.readline()
        if value != "":
            value = int(value.rstrip())
            root_node = Node(None, value, None, None, Color.BLACK)
            tree.root = root_node
        rest_content = f.readlines()
    for line in rest_content:
        value = int(line.rstrip())
        new_node = tree.add_node_to_tree(value)
        if new_node != None:
           new_node.balance(tree)
    tree.print_tree()

if __name__ == "__main__":
    main()