package my_util

func TernaryExpression(relationalExpression bool, expression1, expression2 interface{}) interface{} {
	if relationalExpression {
		return expression1
	} else {
		return expression2
	}
}
