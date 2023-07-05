#define _CRT_SECURE_NO_WARNINGS

/*本程序暂时仅模拟整型数据(int)*/
/*由于整体架构不同，本程序暂不实现ensureCapacity函数*/
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>
#include<string.h>

#define SIZE_OF_ELEMENT sizeof(struct element)
#define NATURAL_ORDER 0
#define REVERSE_ORDER 1

struct element {
	int data;
	struct element* next;
	struct element* pre;
};

struct element* newArrayList();
void isNull(struct element* p);
void isNegativeNumber(int n);
struct element* hasCross(struct element* p, int n);
struct element* locate(struct element* head, int no_);
bool add(int num, ...);
bool addAll(int num, ...);
void clear(struct element* head);
struct element* javaClone(struct element* head);
bool contains(struct element* head, int obj);
int get(struct element* head, int no_);
int indexOf(struct element* head, int obj);
bool removeAll(struct element* head, struct element* c);
void set(struct element* head, int no_, int newData);
void javaRemove(struct element* head, int no_);
int javaSize(struct element* head);
bool isEmpty(struct element* head);
struct element* subList(struct element* head, int fromIndex, int toIndex);
void sort(struct element* head, int c);
int* toArray(int num, ...);
char* toString(struct element* head);
int lastIndexOf(struct element* head, int obj);
bool retainAll(struct element* head, struct element* c);

void main() {
	void println(struct element* head);

	struct element* head = newArrayList();

	add(2, head, 7);
	add(2, head, 18);
	add(2, head, 20);
	
	printf("ArrayList 1：");
	println(head);

	struct element* sites2 = newArrayList();

	add(2, sites2, 23);
	add(2, sites2, 18);
	add(2, sites2, 7);
	printf("ArrayList 2：");
	println(sites2);

	retainAll(head, sites2);
	printf("保留的元素：");
	println(head);
}

//打印集合(临时)
//效果：[element1, element2, element3, ...]
void println(struct element* head) {
	printf("[");

	struct element* p = head->next;

	while (1) {
		if (p != NULL) {
			//如果集合不为空
			printf("%d", p->data);
			
			if (p->next != NULL) {
				//如果后面还有元素
				printf(", ");
				p = p->next;
			}
			else {
				//如果是最后一个元素
				printf("]");
				break;
			}
		}
		else {
			//如果集合为空
			printf("]");
			break;
		}
	}

	printf("\n");
}

//初始化
struct element* newArrayList() {
	struct element* head = (struct element*)malloc(SIZE_OF_ELEMENT);
	
	if (head != NULL) {
		head->data = 0;
		head->next = NULL;
		head->pre = NULL;
	}
	
	return head;
}

//检查传入的指针是否为空
void isNull(struct element* p) {
	if (p == NULL) {
		fprintf(stderr, "指针未初始化\n");
		exit(1);
	}
}

//检查数据是否为负数
void isNegativeNumber(int n) {
	if (n < 0) {
		fprintf(stderr, "下标为负数\n");
		exit(1);
	}
}

//检查下标是否越界,未越界则返回下标所指向的结点
struct element* hasCross(struct element* p, int n) {
	for (int i = 0; i <= n; i++) {
		p = p->next;
		
		if (p == NULL) {
			fprintf(stderr, "访问元素时下标越界\n");
			exit(1);
		}
	}

	return p;
}

//添加元素
bool add(int num, ...) {
	va_list valist;
	va_start(valist, num);

	//判断传入的头指针是否为空
	struct element* p = va_arg(valist, struct element*);
	isNull(p);
	
	if (num == 2) {
		//找到链表的最后一个元素
		while (p->next != NULL) {
			p = p->next;
		}

		struct element* q = (struct element*)malloc(SIZE_OF_ELEMENT);

		if (q != NULL) {
			q->data = va_arg(valist, int);
			q->next = NULL;
			q->pre = p;
		}

		//添加元素
		p->next = q;

		return true;
	}
	else if (num == 3) {
		int no_ = va_arg(valist, int);
		int data = va_arg(valist, int);

		//判断传入的序号是否为自然数
		isNegativeNumber(no_);

		//检查下标是否越界
		struct element* q = hasCross(p, no_);
		
		struct element* r = (struct element*)malloc(SIZE_OF_ELEMENT);

		if (r != NULL) {
			r->data = data;
			r->next = q;
			r->pre = q->pre;
		}

		//添加元素
		q->pre->next = r;
		q->pre = r;

		return true;
	}
	else {
		fprintf(stderr, "参数传递错误\n");
		exit(1);
	}
}

//将给定集合中的所有元素添加到arraylist中
bool addAll(int num, ...) {
	va_list valist;
	va_start(valist, num);

	//判断传入的头指针是否为空
	struct element* p = va_arg(valist, struct element*);

	isNull(p);

	if (num == 2) {
		//找到链表的最后一个元素
		while (p->next != NULL) {
			p = p->next;
		}

		struct element* q = va_arg(valist, struct element*);

		isNull(q);

		if (q->next == NULL) {
			return true;
		}
		else {
			//进入时q为头结点(不要)
			p->next = q->next;
			q->next->pre = p;
		}

		return true;
	}
	else if (num == 3) {
		int no_ = va_arg(valist, int);
		struct element* r = va_arg(valist, struct element*);

		//判断传入的序号是否为自然数
		isNegativeNumber(no_);

		//检查下标是否越界
		struct element* q = hasCross(p, no_);

		//检查要添加的集合
		isNull(r);
		
		if (r->next == NULL) {
			return true;
		}
		else {
			q->pre->next = r->next;

			//找到r的最后一个结点
			while (r->next != NULL) {
				r = r->next;
			}

			r->next = q;

			q->pre = r;
		}

		return true;
	}
	else {
		fprintf(stderr, "参数传递错误\n");
		exit(1);
	}
}

//用于删除动态数组中的所有元素
void clear(struct element* head) {
	isNull(head);

	head->next = NULL;
}

//用于拷贝一份动态数组，属于浅拷贝
struct element* javaClone(struct element* head) {
	isNull(head);

	return head;
}

//用于判断元素是否在动态数组中
bool contains(struct element* head, int obj) {
	isNull(head);

	struct element* p = head;

	while (p->next != NULL) {
		p = p->next;

		if (p->data == obj) {
			return true;
		}
	}

	return false;
}

//通过下标定位元素
struct element* locate(struct element* head, int no_) {
	//判断传入的头指针是否为空
	isNull(head);

	//判断传入的序号是否为自然数
	isNegativeNumber(no_);

	return hasCross(head, no_);
}

//访问元素
int get(struct element* head, int no_) {
	//定位目标结点
	struct element* p = locate(head, no_);

	return p->data;
}

//返回动态数组中元素的索引值
int indexOf(struct element* head, int obj) {
	isNull(head);

	struct element* p = head;
	int index = -1;

	while (p->next != NULL) {
		p = p->next;
		index++;//第一次执行这一行后index的值为0

		if (p->data == obj) {
			return index;
		}
	}

	//如果动态数组中不存在指定的元素，则该方法返回-1
	return -1;
}

//用于删除存在于指定集合中的动态数组元素
bool removeAll(struct element* head, struct element* c) {
	isNull(head);
	isNull(c);

	struct element* q = c;

	while (q->next != NULL) {
		q = q->next;

		struct element* p = head;
		int index = indexOf(p, q->data);

		if (index != -1) {
			p = hasCross(p, index);

			//删除p指向的结点
			p->pre->next = p->next;
			if (p->next != NULL) {
				p->next->pre = p->pre;
			}
		}
	}

	return true;
}

//修改元素
void set(struct element* head, int no_, int newData) {
	//定位目标结点
	struct element* p = locate(head, no_);

	p->data = newData;
}

//删除元素
void javaRemove(struct element* head, int no_) {
	//判断传入的头指针是否为空
	isNull(head);

	//判断传入的序号是否为自然数
	isNegativeNumber(no_);
	
	//检查下标是否越界
	hasCross(head, no_);
	
	//如果要删除第一个元素(no_=0)
	if (no_ == 0) {
		struct element* p = head->next;
		
		head->next = p->next;
	}
	else {
		//因为是删除，所以定位到目标结点的前一个结点(no_-1)
		struct element* p = locate(head, no_ - 1);

		p->next = p->next->next;
	}
}

//用于返回动态数组中元素的数量
int javaSize(struct element* head) {
	isNull(head);

	int number = 0;

	struct element* p = head;

	while (p->next != NULL) {
		number++;
		p = p->next;
	}

	return number;
}

//用于判断动态数组是否为空
bool isEmpty(struct element* head) {
	isNull(head);

	if (head->next == NULL) {
		return true;
	}
	else {
		return false;
	}
}

//用于截取并返回动态数组中的一部分
struct element* subList(struct element* head, int fromIndex, int toIndex) {
	struct element* start = locate(head, fromIndex);

	if (fromIndex > toIndex) {
		fprintf(stderr, "下标冲突\n");
		exit(1);
	}

	struct element* newHead = (struct element*)malloc(SIZE_OF_ELEMENT);
	if (newHead != NULL) {
		newHead->data = 0;
		newHead->next = newHead->pre = NULL;
	}

	struct element* p = newHead;

	for (int i = fromIndex; i < toIndex; i++) {
		if (start != NULL) {
			struct element* q = (struct element*)malloc(SIZE_OF_ELEMENT);
			
			q->data = start->data;
			q->pre = p;
			q->next = NULL;

			p->next = q;
			p = q;

			start = start->next;
		}
		else {
			fprintf(stderr, "下标越界\n");
			exit(1);
		}
	}

	return newHead;
}

//根据指定的顺序对动态数组中的元素进行排序
void sort(struct element* head, int c) {
	isNull(head);

	struct element* p = head;
	
	if (c == NATURAL_ORDER) {
		//升序排序
		int i, j, key;
		for (i = 1; i < javaSize(head); i++) {
			key = get(head, i);
			j = i - 1;
			while ((j >= 0) && (get(head, j) > key)) {
				set(head, j + 1, get(head, j));
				j--;
			}
			set(head, j + 1, key);
		}
	}
	else if (c == REVERSE_ORDER) {
		//降序排序
		int i, j, key;
		for (i = 1; i < javaSize(head); i++) {
			key = get(head, i);
			j = i - 1;
			while ((j >= 0) && (get(head, j) < key)) {
				set(head, j + 1, get(head, j));
				j--;
			}
			set(head, j + 1, key);
		}
	}
	else {
		fprintf(stderr, "传入的排序参数有误\n");
		exit(1);
	}
}

//将ArrayList对象转换为数组
int* toArray(int num,...) {
	va_list valist;
	va_start(valist, num);
	
	struct element* head = va_arg(valist, struct element*);

	int size = javaSize(head);
	if (size <= 0) {
		fprintf(stderr, "程序错误\n");
		exit(1);
	}

	int* p = NULL;
	if (num == 2) {
		p = (int*)malloc(size * sizeof(int));
	}
	else if (num == 3) {
		p = va_arg(valist, int*);
	}

	if (p != NULL) {
		for (int i = 0; i < size; i++) {
			head = head->next;

			p[i] = head->data;
		}
	}

	return p;
}

//将ArrayList对象转换为字符串
char* toString(struct element* head) {
	int size = javaSize(head);
	int psize = 2;
	char* p = (char*)malloc(psize*sizeof(char));
	strcpy(p, "[");

	struct element* q = head;
	int i = 0;
	char* ptemp = NULL;
	for (; i < size; i++) {
		q = q->next;

		int temp = q->data;
		if (temp < 0) {
			//对于负数，将其变为正数，在前面加-
			temp = -temp;

			ptemp = (char*)realloc(p, ++psize);
			if (ptemp != NULL) {
				p = ptemp;
			}
			strcat(p, "-");
		}

		if (temp > 9) {
			int a = 10;
			while (a < temp) {
				a *= 10;
			}
			a /= 10;

			while (a != 0) {
				ptemp = (char*)realloc(p, ++psize);
				if (ptemp != NULL) {
					p = ptemp;
				}
				char c[2] = { temp / a + '0','\0' };
				strcat(p, c);

				int b = temp / a;
				temp = temp - b * a;
				a /= 10;
			}
		}
		else {
			ptemp = (char*)realloc(p, ++psize);
			if (ptemp != NULL) {
				p = ptemp;
			}
			char c[2] = { temp + '0','\0' };
			strcat(p, c);
		}
		if (i != size - 1) {
			//如果不是最后一个
			ptemp = (char*)realloc(p, ++psize);
			if (ptemp != NULL) {
				p = ptemp;
			}
			strcat(p, ",");
			ptemp = (char*)realloc(p, ++psize);
			if (ptemp != NULL) {
				p = ptemp;
			}
			strcat(p, " ");
		}
	}

	ptemp = (char*)realloc(p, ++psize);
	if (ptemp != NULL) {
		p = ptemp;
	}
	strcat(p, "]");
	
	return p;
}

//返回指定元素在动态数组中最后一次出现的位置
int lastIndexOf(struct element* head, int obj) {
	isNull(head);

	struct element* p = head;
	int result = -1;
	int index = -1;

	//遍历传入的动态数组
	while (p->next != NULL) {
		p = p->next;
		index++;

		if (p->data == obj) {
			result = index;
		}
	}

	return result;
}

//保留head中在指定集合中也存在的那些元素，也就是删除指定集合中不存在的那些元素
bool retainAll(struct element* head, struct element* c) {
	isNull(head);
	isNull(c);

	struct element* temp = head->next;
	while (temp != NULL) {
		int index = indexOf(c, temp->data);
		if (index == -1) {
			//如果该元素不在指定集合中
			struct element* p = temp;
			temp = temp->next;

			//删除p指向的结点
			p->pre->next = p->next;
			if (p->next != NULL) {
				p->next->pre = p->pre;
			}
		}
		else {
			temp = temp->next;
		}
	}

	return true;
}