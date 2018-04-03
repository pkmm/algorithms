/*
* 简单的红黑树实现。学习其原理
* 知识来源: 算法导论第三版，STL源码剖析
*/
#include <stdio.h>
#include <queue>
using namespace std;

#define RED 0
#define BLACK 1
#define debug(x) printf("\n==[debug: %d]\n", x);

struct RBnode {
    int data;
    RBnode *left,*right,*p; // p is parent.
    int color; // red:0, black:1
    RBnode(){data=0; color=BLACK; left=right=p=NULL;}
    RBnode(int _data, int _color):data(_data),color(_color),left(NULL),right(NULL),p(NULL){}
};

RBnode * nil = new RBnode(0, BLACK); // global NULL point.


// 对树T中的x节点做左旋操作
void left_rotate(RBnode *&T, RBnode *x)
{
    RBnode *y = x->right;
    x->right = y->left;
    if (y->left != nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == nil) { // T is empty.
        T = y;
    } else if (x->p->left == x) { // x is left children of parent.
        x->p->left = y;
    } else {
        x->p->right = y;
    }

    y->left = x;
    x->p = y;
}

void right_rotate(RBnode *&T, RBnode *x)
{
    RBnode *y = x->left;
    x->left = y->right;
    if (y->right != nil) {
        y->right->p = x;
    }
    y->p = x->p;

    if (x->p == nil) {
        T = y;
    } else if(x->p->left == x) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }

    y->right = x;
    x->p = y;

}

// 修复插入导致的不平衡（这里由下到上, 一般考虑效率会使用由上到下的插入）
void rb_insert_fixup(RBnode *&T, RBnode *z)
{
    RBnode *y = nil;
    while (z->p->color == RED) {

        if (z->p == z->p->p->left) { // left children.
            y = z->p->p->right;
            if (y->color == RED) { // 第一种情况，叔叔(父亲的兄弟)的颜色是红色的
                z->p->color = BLACK;
                z->p->p->color = RED;
                y->color = BLACK;
                z = z->p->p;
            }
            else {
                if(z == z->p->right) { // case 2.
                    z = z->p;
                    left_rotate(T, z);
                }
                z->p->color = BLACK; // case 3.
                z->p->p->color = RED;
                right_rotate(T, z->p->p);
            }
        }
        else { // right children.
            y = z->p->p->left;
            if (y->color == RED) {
                y->color = BLACK;
                z->p->p->color = RED;
                z->p->color = BLACK;
                z = z->p->p;
            }
            else {
                if(z == z->p->left) {
                    z = z->p;
                    right_rotate(T, z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate(T, z->p->p);
            }
        }
    }

    T->color = BLACK;
}

// 在树T中插入z节点
void rb_insert(RBnode*&T, RBnode *z)
{
    RBnode *y = nil;
    RBnode *x = T;

    while (x != nil)
    {
        y = x;
        if (z->data < x->data) x = x->left;
        else x = x->right;
    }

    z->p = y;
    if (y == nil) {
        T = z;
    }
    else if(z->data < y->data) {
        y->left = z;
    }
    else {
        y->right = z;
    }
    z->left = z->right = nil;
    z->color = RED;
    rb_insert_fixup(T, z);
}

// 中序输出rb树的节点值
void inorder_print(RBnode *T)
{
    if(T != nil) {
        inorder_print(T->left);
        printf("%d ", T->data);
        inorder_print(T->right);
    }
}

// bfs层次遍历rb树， 根据结果可以手动验证
void bfs_rb_tree(RBnode *T)
{
    if (T == nil) return ;
    queue<RBnode*> que;
    que.push(T);
    while (!que.empty()) {
        RBnode *rbNode = que.front();
        que.pop();
        printf("\ncolor: %s, data: %d \n\
               leftChildren: [color: %s, data: %d], \n\
               rightChildren: [color: %s, data: %d] \n",
               rbNode->color == RED ? "red" : "black",
               rbNode->data,
               rbNode->left->color == RED ? "red" : "black",
               rbNode->left->data,
               rbNode->right->color == RED ? "red" : "black",
               rbNode->right->data
        );
        if (rbNode->left != nil)  que.push(rbNode->left);
        if (rbNode->right != nil) que.push(rbNode->right);
    }
}



int main()
{
    // test insert
    RBnode *T = nil;
    int nums[] = {8,4,3,6,7,1,2,5};
    for (int i=0;i<8;i++){
        RBnode * tmp = new RBnode(nums[i], RED);
        rb_insert(T, tmp);
    }

    inorder_print(T);

    bfs_rb_tree(T);

    return 0;
}
