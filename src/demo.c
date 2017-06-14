

#define Demo_ID 4

#if Demo_ID == 0
#include "bargraph.c"

#elif Demo_ID == 8
#include "button.c"

#elif Demo_ID == 9
#include "imgui.c"

#elif Demo_ID == 1
#include "house.c"

#elif Demo_ID == 2
#include "kochsnow.c"

#elif Demo_ID == 3
#include "igp.c"

#elif Demo_ID == 4
#include "igp_char.c"

#elif Demo_ID == 5
#include "igp_keyboard.c"

#elif Demo_ID == 6
#include "igp_mouse.c"

#elif Demo_ID == 7
#include "igp_timer.c"

#endif 

void merge(int a1[], int n1, int a2[], int n2, int m[])
{
	int p = 0, k1 = 0, k2 = 0;
	while (k1 < n1 && k2 < n2) {
		if (a1[k1] < a2[k2])
			m[p++] = a1[k1++];
		else
			m[p++] = a2[k2++];
	}
	while (k1 < n1)
		m[p++] = a1[k1++];
	while (k2 < n2)
		m[p++] = a2[k2++];
}

typedef struct DL_Node {
	int data;
	struct DL_Node * prev;
	struct DL_Node * next;
} DLNode;

DLNode * insert(DLNode * head, int v)
{
	DLNode * newnode = (DLNode*)malloc(sizeof(DLNode));
	if (newnode == NULL) return NULL;
	newnode->data = v;
	if (head == NULL) { // 空链表
		newnode->prev = newnode->next = NULL;
		head = newnode; // 新的链表头
	}
	else if (head->data <= v) {
		// 插入在表头
		newnode->next = head;
		newnode->prev = NULL;
		head->prev = newnode;
		head = newnode; // 新的链表头
	} else {
		DLNode * p = head;
		while (p->next && p->next->data > v)
			p = p->next; 
		// 插入到p的后面
		newnode->prev = p;
		newnode->next = p->next;
		if( newnode->next )
			newnode->next->prev = newnode;
		p->next = newnode;
	}

	return head;	
}

int search(int a[], int N, int key)
{
	int low = 0, high = N - 1, middle;
	while (low <= high)
	{
		middle = (low + high) / 2;
		if (key == a[middle])
			return middle;
		else  if (key <a[middle])
			high = middle - 1;     /* lower half */
		else
			low = middle + 1;     /* upper half */
	}
	return -1; /* not found */
}

typedef struct {
	float x; // real part
	float y; // imaging part
} Complex;

Complex Add( Complex *a, Complex *b)
{
	Complex result;
	result.x = a->x + b->x;
	result.y = a->y + b->y;
	return result;
}

