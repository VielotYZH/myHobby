package red_black_tree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class red_black_tree {
    private enum Color {
        RED,
        BLACK
    }

    private class Node {
        public Node f_addr;
        public int value;
        public Node l_addr;
        public Node r_addr;
        public Color color = Color.RED;

        Node(Node f_addr, int value, Node l_addr, Node r_addr) {
            this.f_addr = f_addr;
            this.value = value;
            this.l_addr = l_addr;
            this.r_addr = r_addr;
        }

        private Node turnLeft() {
            Node r_addr = this.r_addr;
            r_addr.f_addr = this.f_addr;
            this.f_addr = r_addr;
            this.r_addr = r_addr.l_addr;
            r_addr.l_addr = this;
            if (this.r_addr != null) {
                this.r_addr.f_addr = this;
            }
            if (r_addr.f_addr != null) {
                if (r_addr.f_addr.l_addr == this) {
                    r_addr.f_addr.l_addr = r_addr;
                } else {
                    r_addr.f_addr.r_addr = r_addr;
                }
                return null;
            } else {
                return r_addr;
            }
        }

        public void turn_left(Tree tree) {
            Node tmp = turnLeft();
            if (tmp != null) {
                tree.root = tmp;
            }
        }

        private Node turnRight() {
            Node l_addr = this.l_addr;
            l_addr.f_addr = this.f_addr;
            this.f_addr = l_addr;
            this.l_addr = l_addr.r_addr;
            l_addr.r_addr = this;
            if (this.l_addr != null) {
                this.l_addr.f_addr = this;
            }
            if (l_addr.f_addr != null) {
                if (l_addr.f_addr.l_addr == this) {
                    l_addr.f_addr.l_addr = l_addr;
                } else {
                    l_addr.f_addr.r_addr = l_addr;
                }
                return null;
            } else {
                return l_addr;
            }
        }

        public void turn_right(Tree tree) {
            Node tmp = turnRight();
            if (tmp != null) {
                tree.root = tmp;
            }
        }

        public void balance(Tree tree) {
            if (this.f_addr.color == Color.RED) {
                Node father_node = this.f_addr;
                Node grandfather_node = father_node.f_addr;
                Node uncle_node = grandfather_node.l_addr == father_node ? grandfather_node.r_addr : grandfather_node.l_addr;
                if (uncle_node != null && uncle_node.color == Color.RED) {
                    father_node.color = Color.BLACK;
                    uncle_node.color = Color.BLACK;
                    if (grandfather_node.f_addr != null) {
                        grandfather_node.color = Color.RED;
                        grandfather_node.balance(tree);
                    }
                } else {
                    if (this == father_node.r_addr) {
                        father_node.turn_left(tree);
                        father_node.balance(tree);
                    } else {
                        father_node.color = Color.BLACK;
                        grandfather_node.color = Color.RED;
                        grandfather_node.turn_right(tree);
                    }
                }
            }
        }
    }

    /*
     * 由于Java没有指针
     * 故采用一个包装类Tree来封装特殊Node节点——根节点
     * 主要用于解决左右旋转时根节点改变的问题
     * */
    private class Tree {
        public Node root;

        Tree() {
        }

        private String get_color(Color color) {
            return switch (color) {
                case RED -> "r";
                case BLACK -> "b";
            };
        }

        private void printTree(Node node, int level) {
            if (node == null) {
                return;
            }
            if (node.r_addr != null) {
                printTree(node.r_addr, level + 1);
            }
            for (int i = 0; i < level; i++) {
                System.out.print("\t");
            }
            System.out.println(node.value + get_color(node.color));
            if (node.l_addr != null) {
                printTree(node.l_addr, level + 1);
            }
        }

        public void print_tree() {
            int level = 0;
            printTree(root, level);
        }

        public Node add_node_to_tree(int value) {
            Node node = new Node(null, value, null, null);
            Node p = this.root;
            while (true) {
                if (value < p.value) {
                    if (p.l_addr != null) {
                        p = p.l_addr;
                    } else {
                        p.l_addr = node;
                        node.f_addr = p;
                        return node;
                    }
                } else if (value > p.value) {
                    if (p.r_addr != null) {
                        p = p.r_addr;
                    } else {
                        p.r_addr = node;
                        node.f_addr = p;
                        return node;
                    }
                } else {
                    return null;
                }
            }
        }
    }

    public static void main(String[] args) throws FileNotFoundException {
        Tree tree = new red_black_tree().new Tree();
        File f = new File("src/red_black_tree/test.txt");
        Scanner sc = new Scanner(f);
        int value;
        if (sc.hasNextInt()) {
            value = sc.nextInt();
            Node root_node = new red_black_tree().new Node(null, value, null, null);
            root_node.color = Color.BLACK;
            tree.root = root_node;
        }
        while (sc.hasNextInt()) {
            value = sc.nextInt();
            Node new_node = tree.add_node_to_tree(value);
            if (new_node != null) {
                new_node.balance(tree);
            }
        }
        sc.close();
        tree.print_tree();
    }
}