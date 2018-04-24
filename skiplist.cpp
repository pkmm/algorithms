/**
 * 跳表是一个随机化的数据，期待是log(n)的时间的复杂度
 * 性能近似红黑树，平衡树等数据结构
 * 编码简单，插入可以在跳表的不同部分并发进行
 * 不用全局的数据结构重新平衡(例如红黑树)
 *
 * 跳表是实现在有序链表上的快速查找 删除等操作
 * 简单实现，假设key是唯一的
 * */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 32
typedef int KeyType;
typedef int ValueType;

// 节点的类型
struct Node
{
	KeyType key;
	ValueType value; // 表示数据资料
	Node **next;	 // 指向后继节点的next数组
	Node() { next = NULL; }
	Node(KeyType k, ValueType v, int level) : key(k), value(v)
	{
		next = (Node **)malloc(sizeof(Node *) * level);
	}
};

// 跳表
struct SkipList
{
	int level; // 跳表的层数
	Node *head;
};

/**
 * 创建一个跳表的节点
 * */
Node *createNode(int level, KeyType key, ValueType value)
{
	Node *p = new Node(key, value, level);
	if (!p)
		return NULL;
	return p;
}

// 创建整个链表的首部
SkipList *createCapital()
{
	SkipList *capital = (SkipList *)malloc(sizeof(SkipList));
	if (!capital)
		return NULL;
	Node *head = new Node(0, 0, MAX_LEVEL);
	if (!head)
	{
		free(capital);
		return NULL;
	}
	capital->level = 0;
	capital->head = head;
	for (int i = 0; i < MAX_LEVEL; i++)
	{
		head->next[i] = NULL;
	}
	return capital;
}

int randomInt()
{
	int level = 1;
	while (rand() % 2)
	{
		level++;
	}
	return level > MAX_LEVEL ? MAX_LEVEL : level;
}

Node *find(SkipList *capital, KeyType key)
{
	Node *pCurrent = capital->head;
	for (int i = capital->level - 1; i >= 0; --i)
	{
		while (pCurrent->next[i] && pCurrent->next[i]->key < key)
		{
			pCurrent = pCurrent->next[i];
		}
	}
	if (pCurrent->next[0] && pCurrent->next[0]->key == key)
	{
		return pCurrent->next[0];
	}
	return NULL;
}

bool insert(SkipList *&capital, KeyType key, ValueType value)
{
	Node *updates[MAX_LEVEL]; // updates[i]保存第i层需要更新的节点
	// 首先找到应该放在的位置
	Node *pCurrent = capital->head; // 从头节点开始(note: capital 是头结点的前一个节点)
	for (int i = capital->level - 1; i >= 0; i--)
	{ // 从最高层开始遍历
		while (pCurrent->next[i] && pCurrent->next[i]->key < key)
		{
			pCurrent = pCurrent->next[i];
		}
		updates[i] = pCurrent; // 记录每一层需要更新的点，因为待插入的节点，从这里进入下一层
	}
	if (pCurrent->next[0] && pCurrent->next[0]->key == key)
	{
		pCurrent->next[0]->value = value;
		return true;
	}

	// 随机一个层次
	int level = randomInt();
	if (level > capital->level)
	{
		for (int i = capital->level; i < level; ++i)
		{
			updates[i] = capital->head;
		}
		capital->level = level;
	}

	// 创建节点 并插入
	Node *tmp = new Node(key, value, level);
	if (!tmp)
		return false;
	for (int i = level - 1; i >= 0; --i)
	{
		tmp->next[i] = updates[i]->next[i];
		updates[i]->next[i] = tmp;
	}

	return true;
}

bool erase(SkipList *capital, KeyType key)
{
	Node *updates[MAX_LEVEL];
	Node *pCurrent = capital->head;
	for (int i = capital->level - 1; i >= 0; --i)
	{
		while (pCurrent->next[i] && pCurrent->next[i]->key < key)
		{
			pCurrent = pCurrent->next[i];
		}
		updates[i] = pCurrent;
	}
	if (!pCurrent->next[0] || pCurrent->next[0]->key != key)
	{
		return false; // not found.
	}
	for (int i = capital->level - 1; i >= 0; --i)
	{
		if (updates[i]->next[i] == pCurrent->next[i])
		{
			updates[i]->next[i] = pCurrent->next[i]->next[i];
			if (capital->head->next[i] == NULL)
				capital->level--;
		}
	}
	return true;
}

void printSkipList(SkipList *capital)
{
	// display skip list
	printf("\n\ntotal level: %d\n", capital->level);
	Node *pcur = capital->head;
	for (int i = capital->level - 1; i >= 0; --i)
	{
		pcur = capital->head;
		while (pcur)
		{
			printf("%d---->", pcur->key);
			pcur = pcur->next[i];
		}
		printf("NULL\n");
	}
}

int main()
{
	// test insert function.
	SkipList *capital = createCapital();
	for (int i = 1; i <= 10; i++)
	{
		printf("insert [%d] status: %d\n", i, insert(capital, i, i + 100));
	}

	printSkipList(capital);

	// Node *tmp = capital->head;
	// while(tmp) {
	//     printf("%d ", tmp->key);
	//     tmp = tmp->next[0];
	// }

	return 0;
}
