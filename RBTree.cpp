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

typedef int RBTreeColor;

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

// 以v为根的子树，替换以u为根的子树
void rb_transplant(RBnode* &T, RBnode *u, RBnode *v)
{
    if(u->p == nil)
        T = v;
    else if(u->p->left == u)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

RBnode* minimum(RBnode *T)
{
    if (T == nil) return nil;
    while (T->left != nil) {
        T = T->left;
    }
    return T;
}

RBnode* maximum(RBnode*T)
{
    if (T == nil) return nil;
    while(T->right != nil) {
        T = T->right;
    }
    return T;
}

// 修复删除节点导致的树性质不满足问题

void rb_delete_fixup(RBnode* &T, RBnode* x)
{
    RBnode *w; // 指向x的兄弟节点
    while (x != T && x->color == BLACK) {
        if (x == x->p->left) { // x 是父亲的左儿子
            w = x->p->right;
            if (w->color == RED) {
                // case 1.) x的兄弟结点w颜色是红色的, 那么改变x父亲(x->p)和w的颜色
                // 再对x父亲结点(x->p)做一次左旋操作
                w->color = BLACK;
                x->p->color = RED;
                left_rotate(T, x->p);
            }
            else if (w->left->color == BLACK && w->right->color == BLACK) {
                // case 2.) w颜色是黑色的，同时w的2个儿子结点的颜色也都是黑色的
                // 因为w,x都是黑色的, 可以同时减掉w,x身上的一层黑色, 然后
                // 在二者的父节点(x->p)增加额外的黑色,那么这个父节点就会成为新的x结点(双重颜色的节点)
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->right->color == BLACK) {
                    // case 3.) w是黑色的 w左儿子是红色的 右儿子是黑色的
                    // 交换左儿子和w的颜色 右旋w节点 更新w位置
                    w->color = RED;
                    w->left->color = BLACK;
                    right_rotate(T, w);
                    w = x->p->right;
                }
                // case 4.) w是黑色的 右儿子是红色的
                //
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                left_rotate(T, x->p);
                x = T;
            }
        }
        else { // 是父亲的右儿子
            w = x->p->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->p->color = RED;
                right_rotate(T, x->p);
            }
            else if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->p;
            }
            else {
                if (w->left->color == BLACK) {
                    w->color = RED;
                    w->right->color = BLACK;
                    left_rotate(T, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                right_rotate(T, x->p);
                x = T;
            }
        }
    }
    x->color = BLACK;
}


// 删除操作,删除T中的z结点
void rb_delete(RBnode* &T, RBnode* z)
{
    RBnode *y = z, *x;// if z has less than two child then y point z, other y point successor of z.
    RBTreeColor y_origin_color = y->color;
    if (z->left == nil) {
        x = z->right;
        rb_transplant(T, z, z->right);
    }
    else if (z->right == nil) {
        x = z->left;
        rb_transplant(T, z, z->left);
    }
    else {
        y = minimum(z->right);
        y_origin_color = y->color;
        x = y->right;
        if (y->p == z) { // y是z的直接后继
            x->p = y;
        }
        else { // y是z右子树的最左节点，因此y的做儿子是空的
            rb_transplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rb_transplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color; // y的颜色没了，可能会导致破坏树的性质
    }
    if (y_origin_color == BLACK) {
        rb_delete_fixup(T, x);
    }
}

// 查找一个元素
RBnode *rb_search(RBnode*T, int val)
{
    if (T == nil) return nil;
    while (T != nil && T->data != val) {
        if (T->data > val) T = T->left;
        else T = T->right;
    }
    return T;
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

    // test rb_search function.
    int key = 4; // 78
    RBnode *rest = rb_search(T, key);
    if (rest == nil) {
        printf("\nkey [%d] not found.\n", key);
    }
    else {
        printf("\nkey [%d] found, color: %s, parent_color: %s, parent_data: %d\n",
               key, rest->color == BLACK ? "black" : "red",
               rest->p->color == RED ? "red" : "black",
               rest->data);
    }

    // test delete function.
    rest = rb_search(T, 7);
    rb_delete(T, rest);
    bfs_rb_tree(T);

    // 2018-04-18 完结撒花，终于是把红黑树给简单的实现了，时间跨度比较久 = =
    // 结合《算法导论》的几个图形进行添加，删除比较好理解一些，仍然需要时间消化
    // 继续前进~~~
    return 0;
}
