#include <memory>
#include <cstring>
#include "debug.h"
#include "bst64.h"

enum BST64TraversalType { BST64_INORDER, BST64_PREORDER, BST64_POSTORDER };

static int Traversal( BST64Node *p, int i, BST64TraversalType tt, int fn(BST64TraversalOp, int, int64_t, void * ))
{
    if (p)
    {
        if ( tt == BST64_PREORDER )
        {
            fn(BST64_TRAVERSAL_ITEM, i, p->key, p->pData);
            i++;
        }
        
        i = Traversal(p->pLeft, i, tt, fn);
        
        if ( tt == BST64_INORDER )
        {
            fn(BST64_TRAVERSAL_ITEM, i, p->key, p->pData);
            i++;
        }
        
        i = Traversal(p->pRight, i, tt, fn);
        
        if (tt == BST64_POSTORDER)
        {
            fn(BST64_TRAVERSAL_ITEM, i, p->key, p->pData);
            i++;
        }
        
    }
    
    return i;
}
static BST64NodeBlock * AllocNodeBlock()
{    
    BST64NodeBlock *p = (BST64NodeBlock *)malloc(sizeof(BST64NodeBlock));
    ASSERT(p, "Failed memory allocation in BST64::AllocNodeBlock()");
    if (p)
    {
        memset((void *)p, 0, sizeof(BST64NodeBlock));
        p->size = BST64_NODEBLOCKSIZE;
    }
    return p;
}

static BST64Node * AllocNode(BST64NodeBlock **pNB)
{
    ASSERT(*pNB, "NULL pNB in BST::AllocNode()");
    BST64NodeBlock *pNodes = *pNB;
    
    if (pNodes->length >= pNodes->size)
    {
        // block full, allocate new one
        BST64NodeBlock *p = AllocNodeBlock();
        
        ASSERT(p, "Failed allocation of new NodeBlock in BST::AllocNode()");
        
        if (p)
        {
            // link at the front for easy access
            p->pNext = pNodes;
            *pNB = p;
        }
        else
        {
            return (BST64Node *)0;
        }
    }
    
    BST64Node *pNode = &(pNodes->nodes[pNodes->length]);
    pNodes->length++;
    return pNode;
}

BST64::BST64(void)
{
    pNodes = AllocNodeBlock();
    pRoot = (BST64Node *) 0;
}

BST64::~BST64(void)
{
    BST64NodeBlock *p, *q;
    for( p = pNodes, q = (BST64NodeBlock *)0; p ; p = q )
    {
        q = p->pNext;
        free(p);
    }
}

int BST64::InOrder(int fn(BST64TraversalOp, int, int64_t, void *))
{
    int i = 0;
    fn(BST64_TRAVERSAL_START,0,0,(void *)0);
    i = Traversal(pRoot, 0, BST64_INORDER, fn);
    fn(BST64_TRAVERSAL_END,i,0,(void *)0);
    return i;
}

int BST64::PreOrder(int fn(BST64TraversalOp, int, int64_t, void *))
{
    int i = 0;
    fn(BST64_TRAVERSAL_START,0,0,(void *)0);
    i = Traversal(pRoot, 0, BST64_PREORDER, fn);
    fn(BST64_TRAVERSAL_END,i,0,(void *)0);
    return i;
}

int BST64::PostOrder(int fn(BST64TraversalOp, int, int64_t, void *))
{
    int i = 0;
    fn(BST64_TRAVERSAL_START,0,0,(void *)0);
    i = Traversal(pRoot, 0, BST64_POSTORDER, fn);
    fn(BST64_TRAVERSAL_END,i,0,(void *)0);
    return i;
}
    
int BST64::Insert(int64_t key, void *pData)
{
    BST64Node *u, *v, *p;

    p = AllocNode(&pNodes);
    ASSERT(p, "Failed to allocate node in BST::Insert()");
    
    if (!p)
    {
        return 0;
    }
    
    p->key = key;
    p->pData = pData;
    p->size = 1;
    
    if (!pRoot)
    {
        pRoot = p;
    }
    else
    {
        for( v = pRoot; v != (BST64Node *)0; )
        {
            u = v;
            v = ((key <= v->key) ? v->pLeft : v->pRight );
        }
        
        if ( key <= u->key )
        {
            u->pLeft = p;
        }
        else
        {
            u->pRight = p;
        }
        
        p->pParent = u;
        
        // traverse up to root, incrementing size of subtrees rooted along the path
        ASSERT(pRoot->pParent == (BST64Node *)0, "pRoot has non-NULL pParent, in BST::Insert()");
        
        do {
            u->size++;
            u = u->pParent;
        } while (u != (BST64Node *)0);
    }
    return 1;
}

int BST64::Search(int64_t key, void **ppData)
{
    BST64Node *p = pRoot;
    
    while ( p )
    {
        if ( p->key == key )
        {
            if (ppData)
            {
                *ppData = p->pData;
            }
            return 1;
        }
        
        p = ((key < p->key) ? p->pLeft : p->pRight );
    }
    
    return 0;
}

