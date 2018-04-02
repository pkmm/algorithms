/*
* 简单的红黑树实现。学习其原理
*/
#include <stdio.h>

#define RED 0
#define BLACK 1

struct RBnode {
    int data;
    RBnode *left,*right,*p; // p is parent.
    int color; // red:0, black:1
    RBnode(){data=0; color=BLACK; left=right=p=NULL;}
    RBnode(int _data, int _color):data(_data),color(_color),left(NULL),right(NULL),p(NULL){}
};


// 对树T中的x节点做左旋操作
void left_rotate(RBnode **T, RBnode *x)
{
    RBnode *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == NULL) { // T is empty.
        *T = y;
    } else if (x->p->left == x) { // x is left children of parent.
        x->p->left = y;
    } else {
        x->p->right = y;
    }

    y->left = x;
    x->p = y;
}

void right_rotae(RBnode **T, RBnode *x)
{
    RBnode *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->p = x;
    }
    y->p = x->p;

    if (x->p == NULL) {
        *T = y;
    } else if(x->p->left == x) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }

    y->right = x;
    x->p = y;

}

int main()
{
    return 0;
}
