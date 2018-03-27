

#include <stdio.h>
// 暂时只考虑是整数的数据类型, 不能有重复的元素


// 二叉搜索树
struct BTNode {
    int data;
    BTNode* left, *right, *p; // p is parent
    BTNode(int _data):data(_data),left(NULL),right(NULL),p(NULL){};
    BTNode(){}
};

// 查找
BTNode* tree_search(BTNode *root, int x)
{
    while (root && root->data != x) {
        if (x < root->data) root = root->left;
        else root = root->right;
    }
    return root;
}

// 最小的节点
BTNode* tree_minimum(BTNode *root)
{
    while (root->left) root = root->left;
    return root;
}

// 查找最大的节点
BTNode* tree_maximum(BTNode *root)
{
    while(root->right) root = root->right;
    return root;
}

// 插入
BTNode* tree_insert(BTNode* root, int key)
{
    BTNode *z = new BTNode(key);
    BTNode *p = NULL, *x = root;
    while(x) {
        p = x;
        if (x->data < key) x = x->right;
        else x = x->left;
    }
    z->p = p;
    if (p == NULL) {
        root = z;
    } else if (p->data > key) {
        p->left = z;
    } else {
        p->right = z;
    }

    return root;
}

// 用v为根的子树 替换u为根的子树
void transplant(BTNode *root, BTNode *u, BTNode *v)
{
    if (u->p == NULL) {
        root = v;
    } else if(u->p->left == u) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }
    if (v != NULL)
        v->p = u->p;
}

// 从树root删除Z节点
void tree_delete(BTNode *root, BTNode *z)
{
    if (z->left== NULL)
        transplant(root, z, z->right);
    else if(z->right==NULL)
        transplant(root, z, z->left);
    else {
        BTNode* y = tree_minimum(z->right);
        if (y->p != z){ // z有2个孩子，而且y 不是z的直接后继
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        transplant(root, z, y);
        y->left = z->left;
        y->left->p = y;
    }
}

// 中序遍历
void tree_inorder_walk(BTNode *root)
{
    if (root) {
        tree_inorder_walk(root->left);
        printf("%d ", root->data);
        tree_inorder_walk(root->right);
    }
}

int main(){

    int num[] = {2,3,4,1,8,7,6,5};

    // create binary tree.
    BTNode *T = NULL;
    for (int i=0;i<8;i++){
        T = tree_insert(T, num[i]);
    }

    // get min node in tree.
    BTNode *mi = tree_minimum(T);
    printf("\n %d \n", mi->data);
    // delete the min value in tree.
    tree_delete(T, mi);
    // in order walk of tree.
    tree_inorder_walk(T);

    BTNode*mx = tree_maximum(T);
    printf("\n %d \n", mx->data);
    tree_delete(T, mx);
    tree_inorder_walk(T);

    // search
    BTNode *stmp = tree_search(T, 4);
    printf("\n %d\n", stmp->data);

    //insert
    tree_insert(T, 45);
    tree_insert(T, -1);
    tree_insert(T, 66);
    tree_inorder_walk(T);

    return 0;
}
