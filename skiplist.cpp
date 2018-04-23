/**
 * 跳表是一个随机化的数据，期待是log(n)的时间的复杂度
 * 性能近似红黑树，平衡树等数据结构
 * 编码简单，插入可以在跳表的不同部分并发进行
 * 不用全局的数据结构重新平衡(例如红黑树)
 * */
#include <stdio.h>
#include <stdlib.h>

#define maxLevel = 32
typedef int KeyType;
typedef int ValueType;

// 节点的类型
struct Node {
	KeyType key;
	ValueType value; 
	Node* next[1]; // 柔性数组，可变长度的数组
};

// 跳表
struct SkipList {
	int level; // 跳表的层数
	Node* head;
};

/**
 * 生成一个Node类型的节点
 * next数组设置为level大小
 * */
Node* make_node_of_level(int level)
{
	return (Node*)malloc(sizeof(Node) + level * sizeof(Node*));
}

/**
 * 创建一个跳表的节点
 * */
Node* create_node(int level, KeyType key, ValueType value)
{
	Node *p = NULL;
	if (!(p = make_node_of_level(level))) {
		return NULL;
	}
	p->key = key;
	p->value = value;
	return p;
}

int main()
{
	return 0;
}
