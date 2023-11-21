package my_util

func TernaryExpression(relationalExpression bool, expression1 int, expression2 int) int {
	if relationalExpression {
		return expression1
	} else {
		return expression2
	}
}
