/*************************************************************************
	> File Name: tree_test.c
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Mar 2018 07:46:32 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef char TELemType; 
//二叉树
typedef struct BiTNode{
    TELemType data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

//前序遍历
void PreOrderTree(BiTree T)
{
    if(T == NULL)
        return;
    printf("%c", T->data);
    PreOrderTree(T->lchild);
    PreOrderTree(T->rchild);
}

//中序遍历
void InOrderTree(BiTree T)
{
    if(T == NULL)
        return;
    InOrderTree(T->lchild);
    printf("%c", T->data);
    InOrderTree(T->rchild);
}

//后续遍历
void PostOrderTree(BiTree T)
{
    if(T == NULL)
        return;
    PostOrderTree(T->lchild);
    PostOrderTree(T->rchild);

    printf("%c", T->data);
}

void CreateBitTree(BiTree *T)
{
    TELemType ch;
    scanf("%c", &ch);

    if(ch = '#'){
        *T = NULL;
    }else{
        *T = (BiTree)malloc(sizeof(BiTree));
        if(!*T){
            perror("malloc");
            exit(1);
        }
        (*T)->data = ch;
        CreateBitTree(&(*T)->lchild);
        CreateBitTree(&(*T)->rchild);
    }
}
