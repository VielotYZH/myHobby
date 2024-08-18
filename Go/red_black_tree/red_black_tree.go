package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"

	"Go/my-util" // 我自己的工具包
)

type Color int

const (
	RED Color = iota
	BLACK
)

type Node struct {
	fAddr *Node
	value int
	lAddr *Node
	rAddr *Node
	color Color
}

func (node *Node) turnLeft() *Node {
	rAddr := node.rAddr
	rAddr.fAddr = node.fAddr
	node.fAddr = rAddr
	node.rAddr = rAddr.lAddr
	rAddr.lAddr = node
	if node.rAddr != nil {
		node.rAddr.fAddr = node
	}
	if rAddr.fAddr != nil {
		if rAddr.fAddr.lAddr == node {
			rAddr.fAddr.lAddr = rAddr
		} else {
			rAddr.fAddr.rAddr = rAddr
		}
		return nil
	} else {
		return rAddr
	}
}

func (node *Node) turnLEFT(tree *Tree) {
	tmp := node.turnLeft()
	if tmp != nil {
		tree.root = tmp
	}
}

func (node *Node) turnRight() *Node {
	lAddr := node.lAddr
	lAddr.fAddr = node.fAddr
	node.fAddr = lAddr
	node.lAddr = lAddr.rAddr
	lAddr.rAddr = node
	if node.lAddr != nil {
		node.lAddr.fAddr = node
	}
	if lAddr.fAddr != nil {
		if lAddr.fAddr.lAddr == node {
			lAddr.fAddr.lAddr = lAddr
		} else {
			lAddr.fAddr.rAddr = lAddr
		}
		return nil
	} else {
		return lAddr
	}
}

func (node *Node) turnRIGHT(tree *Tree) {
	tmp := node.turnRight()
	if tmp != nil {
		tree.root = tmp
	}
}

func (node *Node) balance(tree *Tree) {
	if node.fAddr.color == RED {
		fatherNode := node.fAddr
		grandfatherNode := fatherNode.fAddr
		uncleNode := my_util.TernaryExpression(grandfatherNode.lAddr == fatherNode, grandfatherNode.rAddr, grandfatherNode.lAddr).(*Node) // 这里调用了我自己编写的一个用来替代三元运算符的函数
		if uncleNode != nil && uncleNode.color == RED {
			fatherNode.color = BLACK
			uncleNode.color = BLACK
			if grandfatherNode.fAddr != nil {
				grandfatherNode.color = RED
				grandfatherNode.balance(tree)
			}
		} else {
			if node == fatherNode.rAddr {
				fatherNode.turnLEFT(tree)
				fatherNode.balance(tree)
			} else {
				fatherNode.color = BLACK
				grandfatherNode.color = RED
				grandfatherNode.turnRIGHT(tree)
			}
		}
	}
}

// Tree
/*虽然go中有和C中一样的指针的指针
但go要求同一结构体的接收器必须有相同的命名
为了满足该条件又使得命名有意义
依然定义另一个结构体来封装红黑树的根节点
*/
type Tree struct {
	root *Node
}

func (tree *Tree) addNodeToTree(value int) *Node {
	node := new(Node)
	node.value = value
	node.lAddr = nil
	node.rAddr = nil
	node.color = RED
	p := tree.root
	for {
		if value < p.value {
			if p.lAddr != nil {
				p = p.lAddr
			} else {
				p.lAddr = node
				node.fAddr = p
				return node
			}
		} else if value > p.value {
			if p.rAddr != nil {
				p = p.rAddr
			} else {
				p.rAddr = node
				node.fAddr = p
				return node
			}
		} else {
			// 本来return nil可以写在这里
			// 但go要求每个代码路径都要有返回值，即使是不可达的
			// 故在这里写作break，而将return写在for循环外面
			break
		}
	}
	return nil
}

func getColor(color Color) string {
	switch color {
	case RED:
		return "r"
	case BLACK:
		return "b"
	default:
		return "!" // 不可能
	}
}

func printTree(node *Node, level int) {
	if node == nil {
		return
	}
	if node.rAddr != nil {
		printTree(node.rAddr, level+1)
	}
	for i := 0; i < level; i++ {
		fmt.Print("\t")
	}
	fmt.Println(node.value, getColor(node.color))
	if node.lAddr != nil {
		printTree(node.lAddr, level+1)
	}
}

func (tree *Tree) printTree() {
	level := 0
	printTree(tree.root, level)
}

func main() {
	f, _ := os.Open("red_black_tree/test.txt")
	sc := bufio.NewScanner(f)
	tree := new(Tree)
	if sc.Scan() {
		value, _ := strconv.Atoi(sc.Text())
		rootNode := new(Node)
		rootNode.fAddr = nil
		rootNode.value = value
		rootNode.lAddr = nil
		rootNode.rAddr = nil
		rootNode.color = BLACK
		tree.root = rootNode
	}
	for sc.Scan() {
		value, _ := strconv.Atoi(sc.Text())
		newNode := tree.addNodeToTree(value)
		if newNode != nil {
			newNode.balance(tree)
		}
	}
	_ = f.Close()
	tree.printTree()
}
