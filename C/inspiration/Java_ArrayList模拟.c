#define _CRT_SECURE_NO_WARNINGS

/*��������ʱ��ģ����������(int)*/
/*��������ܹ���ͬ���������ݲ�ʵ��ensureCapacity����*/
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
	
	printf("ArrayList 1��");
	println(head);

	struct element* sites2 = newArrayList();

	add(2, sites2, 23);
	add(2, sites2, 18);
	add(2, sites2, 7);
	printf("ArrayList 2��");
	println(sites2);

	retainAll(head, sites2);
	printf("������Ԫ�أ�");
	println(head);
}

//��ӡ����(��ʱ)
//Ч����[element1, element2, element3, ...]
void println(struct element* head) {
	printf("[");

	struct element* p = head->next;

	while (1) {
		if (p != NULL) {
			//������ϲ�Ϊ��
			printf("%d", p->data);
			
			if (p->next != NULL) {
				//������滹��Ԫ��
				printf(", ");
				p = p->next;
			}
			else {
				//��������һ��Ԫ��
				printf("]");
				break;
			}
		}
		else {
			//�������Ϊ��
			printf("]");
			break;
		}
	}

	printf("\n");
}

//��ʼ��
struct element* newArrayList() {
	struct element* head = (struct element*)malloc(SIZE_OF_ELEMENT);
	
	if (head != NULL) {
		head->data = 0;
		head->next = NULL;
		head->pre = NULL;
	}
	
	return head;
}

//��鴫���ָ���Ƿ�Ϊ��
void isNull(struct element* p) {
	if (p == NULL) {
		fprintf(stderr, "ָ��δ��ʼ��\n");
		exit(1);
	}
}

//��������Ƿ�Ϊ����
void isNegativeNumber(int n) {
	if (n < 0) {
		fprintf(stderr, "�±�Ϊ����\n");
		exit(1);
	}
}

//����±��Ƿ�Խ��,δԽ���򷵻��±���ָ��Ľ��
struct element* hasCross(struct element* p, int n) {
	for (int i = 0; i <= n; i++) {
		p = p->next;
		
		if (p == NULL) {
			fprintf(stderr, "����Ԫ��ʱ�±�Խ��\n");
			exit(1);
		}
	}

	return p;
}

//���Ԫ��
bool add(int num, ...) {
	va_list valist;
	va_start(valist, num);

	//�жϴ����ͷָ���Ƿ�Ϊ��
	struct element* p = va_arg(valist, struct element*);
	isNull(p);
	
	if (num == 2) {
		//�ҵ���������һ��Ԫ��
		while (p->next != NULL) {
			p = p->next;
		}

		struct element* q = (struct element*)malloc(SIZE_OF_ELEMENT);

		if (q != NULL) {
			q->data = va_arg(valist, int);
			q->next = NULL;
			q->pre = p;
		}

		//���Ԫ��
		p->next = q;

		return true;
	}
	else if (num == 3) {
		int no_ = va_arg(valist, int);
		int data = va_arg(valist, int);

		//�жϴ��������Ƿ�Ϊ��Ȼ��
		isNegativeNumber(no_);

		//����±��Ƿ�Խ��
		struct element* q = hasCross(p, no_);
		
		struct element* r = (struct element*)malloc(SIZE_OF_ELEMENT);

		if (r != NULL) {
			r->data = data;
			r->next = q;
			r->pre = q->pre;
		}

		//���Ԫ��
		q->pre->next = r;
		q->pre = r;

		return true;
	}
	else {
		fprintf(stderr, "�������ݴ���\n");
		exit(1);
	}
}

//�����������е�����Ԫ����ӵ�arraylist��
bool addAll(int num, ...) {
	va_list valist;
	va_start(valist, num);

	//�жϴ����ͷָ���Ƿ�Ϊ��
	struct element* p = va_arg(valist, struct element*);

	isNull(p);

	if (num == 2) {
		//�ҵ���������һ��Ԫ��
		while (p->next != NULL) {
			p = p->next;
		}

		struct element* q = va_arg(valist, struct element*);

		isNull(q);

		if (q->next == NULL) {
			return true;
		}
		else {
			//����ʱqΪͷ���(��Ҫ)
			p->next = q->next;
			q->next->pre = p;
		}

		return true;
	}
	else if (num == 3) {
		int no_ = va_arg(valist, int);
		struct element* r = va_arg(valist, struct element*);

		//�жϴ��������Ƿ�Ϊ��Ȼ��
		isNegativeNumber(no_);

		//����±��Ƿ�Խ��
		struct element* q = hasCross(p, no_);

		//���Ҫ��ӵļ���
		isNull(r);
		
		if (r->next == NULL) {
			return true;
		}
		else {
			q->pre->next = r->next;

			//�ҵ�r�����һ�����
			while (r->next != NULL) {
				r = r->next;
			}

			r->next = q;

			q->pre = r;
		}

		return true;
	}
	else {
		fprintf(stderr, "�������ݴ���\n");
		exit(1);
	}
}

//����ɾ����̬�����е�����Ԫ��
void clear(struct element* head) {
	isNull(head);

	head->next = NULL;
}

//���ڿ���һ�ݶ�̬���飬����ǳ����
struct element* javaClone(struct element* head) {
	isNull(head);

	return head;
}

//�����ж�Ԫ���Ƿ��ڶ�̬������
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

//ͨ���±궨λԪ��
struct element* locate(struct element* head, int no_) {
	//�жϴ����ͷָ���Ƿ�Ϊ��
	isNull(head);

	//�жϴ��������Ƿ�Ϊ��Ȼ��
	isNegativeNumber(no_);

	return hasCross(head, no_);
}

//����Ԫ��
int get(struct element* head, int no_) {
	//��λĿ����
	struct element* p = locate(head, no_);

	return p->data;
}

//���ض�̬������Ԫ�ص�����ֵ
int indexOf(struct element* head, int obj) {
	isNull(head);

	struct element* p = head;
	int index = -1;

	while (p->next != NULL) {
		p = p->next;
		index++;//��һ��ִ����һ�к�index��ֵΪ0

		if (p->data == obj) {
			return index;
		}
	}

	//�����̬�����в�����ָ����Ԫ�أ���÷�������-1
	return -1;
}

//����ɾ��������ָ�������еĶ�̬����Ԫ��
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

			//ɾ��pָ��Ľ��
			p->pre->next = p->next;
			if (p->next != NULL) {
				p->next->pre = p->pre;
			}
		}
	}

	return true;
}

//�޸�Ԫ��
void set(struct element* head, int no_, int newData) {
	//��λĿ����
	struct element* p = locate(head, no_);

	p->data = newData;
}

//ɾ��Ԫ��
void javaRemove(struct element* head, int no_) {
	//�жϴ����ͷָ���Ƿ�Ϊ��
	isNull(head);

	//�жϴ��������Ƿ�Ϊ��Ȼ��
	isNegativeNumber(no_);
	
	//����±��Ƿ�Խ��
	hasCross(head, no_);
	
	//���Ҫɾ����һ��Ԫ��(no_=0)
	if (no_ == 0) {
		struct element* p = head->next;
		
		head->next = p->next;
	}
	else {
		//��Ϊ��ɾ�������Զ�λ��Ŀ�����ǰһ�����(no_-1)
		struct element* p = locate(head, no_ - 1);

		p->next = p->next->next;
	}
}

//���ڷ��ض�̬������Ԫ�ص�����
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

//�����ж϶�̬�����Ƿ�Ϊ��
bool isEmpty(struct element* head) {
	isNull(head);

	if (head->next == NULL) {
		return true;
	}
	else {
		return false;
	}
}

//���ڽ�ȡ�����ض�̬�����е�һ����
struct element* subList(struct element* head, int fromIndex, int toIndex) {
	struct element* start = locate(head, fromIndex);

	if (fromIndex > toIndex) {
		fprintf(stderr, "�±��ͻ\n");
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
			fprintf(stderr, "�±�Խ��\n");
			exit(1);
		}
	}

	return newHead;
}

//����ָ����˳��Զ�̬�����е�Ԫ�ؽ�������
void sort(struct element* head, int c) {
	isNull(head);

	struct element* p = head;
	
	if (c == NATURAL_ORDER) {
		//��������
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
		//��������
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
		fprintf(stderr, "����������������\n");
		exit(1);
	}
}

//��ArrayList����ת��Ϊ����
int* toArray(int num,...) {
	va_list valist;
	va_start(valist, num);
	
	struct element* head = va_arg(valist, struct element*);

	int size = javaSize(head);
	if (size <= 0) {
		fprintf(stderr, "�������\n");
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

//��ArrayList����ת��Ϊ�ַ���
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
			//���ڸ����������Ϊ��������ǰ���-
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
			//����������һ��
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

//����ָ��Ԫ���ڶ�̬���������һ�γ��ֵ�λ��
int lastIndexOf(struct element* head, int obj) {
	isNull(head);

	struct element* p = head;
	int result = -1;
	int index = -1;

	//��������Ķ�̬����
	while (p->next != NULL) {
		p = p->next;
		index++;

		if (p->data == obj) {
			result = index;
		}
	}

	return result;
}

//����head����ָ��������Ҳ���ڵ���ЩԪ�أ�Ҳ����ɾ��ָ�������в����ڵ���ЩԪ��
bool retainAll(struct element* head, struct element* c) {
	isNull(head);
	isNull(c);

	struct element* temp = head->next;
	while (temp != NULL) {
		int index = indexOf(c, temp->data);
		if (index == -1) {
			//�����Ԫ�ز���ָ��������
			struct element* p = temp;
			temp = temp->next;

			//ɾ��pָ��Ľ��
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