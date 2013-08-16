#include "debug.h"
#include "bst.h"
#include "queue.h"

typedef enum enumBST_TRAVERSAL_TYPE{ BST_PREORDER, BST_INORDER, BST_POSTORDER } BST_TRAVERSAL_TYPE;

static void BSTScopeR( void fn(BSTNode *), Queue *q )
{
    BSTNode *p;
    
    while( q->Size() > 0 )
    {
        p = (BSTNode *)q->Remove();
        
        if (p != (BSTNode *)0)
        {
            fn(p);
            
            if (p->pLeft != (BSTNode *)0)
            {
                q->Add((void *) (p->pLeft));
            }
            if (p->pRight != (BSTNode *)0)
            {
                q->Add((void *) (p->pRight));
            }
        }
    }
}

static void BSTScope( BSTNode *p, void fn(BSTNode *) )
{
    Queue *q = new(Queue);
    q->Add((void *)p);    
    BSTScopeR(fn,q);
}

static BSTNode *BSTMax(BSTNode *p)
{
    if (p == (BSTNode *)0)
        return p;
    
    while( p->pRight != (BSTNode *)0 )
    {
        p = p->pRight;
    }
    return p;
}

static BSTNode *BSTMin(BSTNode *p)
{  
    if (p == (BSTNode *)0)
        return p;
 
    while (p->pLeft != (BSTNode *)0)
    {
        p = p->pLeft;
    }
    
    return p;
}

static int BSTSize(BSTNode *p)
{
    return ((p == (BSTNode *)0) ? 0 : p->n);
}

static void BSTSwap(BSTNode *p, BSTNode *q)
{
    BSTNode t;

    ASSERT(p, "NULL p in BSTSwap()");
    ASSERT(q, "NULL q in BSTSwap()");
    
    t = *p;
    *p = *q;
    *q = t;
}

    
static int BSTTraversal(BSTNode *p, int i, int fn(BST_TRAVERSAL_OP,int,int, void *), BST_TRAVERSAL_TYPE tt)
{
    int r;
    
    if (p == (BSTNode *)0)
    {
        return i;
    }
    
    if (tt == BST_PREORDER)
    {
        i++;
        r = fn(BST_TRAVERSAL_ITEM,i,p->key,p->pData);
    }
    
    i = BSTTraversal(p->pLeft, i, fn, tt);
    
    if ( tt == BST_INORDER )
    {
        i++;
        r = fn(BST_TRAVERSAL_ITEM, i, p->key, p->pData);
    }
    
    i = BSTTraversal(p->pRight, i, fn, tt);
    
    if ( tt == BST_POSTORDER )
    {
        i++;
        r = fn(BST_TRAVERSAL_ITEM, i, p->key, p->pData);
    }
    
    return i;
}

// returns:
// NULL: if empty tree
// pointer to node that contains the desired key if found, or
// pointer to insertion spot for key, if key is not found in the tree
static BSTNode * BSTSearch(BSTNode *p, int key)
{
    BSTNode *pNext;
    
    if ( p == (BSTNode *) 0 )
    {
        return p;
    }
    
    while ( p->key != key )
    {
        if ( key < p->key )
        {
            if ( p->pLeft == (BSTNode *) 0)
            {
                return p;
            }
            
            p = p->pLeft;
        }
        else
        {
            if ( p->pRight == (BSTNode *) 0)
            {
                return p;
            }
            
            p = p->pRight;
        }
    }
    
    return p;
}

BST::BST(void)
{
    pRoot = (BSTNode *) 0;
}

BST::~BST(void)
{
}

void * BST::Max()
{
    BSTNode *p = BSTMax(pRoot);
    if (p != (BSTNode *)0)
    {
        return p->pData;
    }
    return (BSTNode *)0;
}

void * BST::Min()
{
    BSTNode *p = BSTMin(pRoot);
    if (p != (BSTNode *)0)
    {
        return p->pData;
    }
    return (BSTNode *)0;
}

void * BST::Search(int key)
{
    BSTNode *p = BSTSearch(pRoot,key);
    
    if ((p == (BSTNode *)0) || (p->key != key))
        return (void *) 0;
        
    return p->pData;
}

int BST::InOrder(int fn(BST_TRAVERSAL_OP, int, int, void *))
{
    int n;
    // signal start of traversal
    fn(BST_TRAVERSAL_START,0,0,(void *)0);
    // execute traversal
    n = BSTTraversal(pRoot,0,fn,BST_INORDER);
    // signal end of traversal
    fn(BST_TRAVERSAL_END,n,0,(void *)0);
    return 1;
}

int BST::PreOrder(int fn(BST_TRAVERSAL_OP, int, int, void *))
{
    int n;
    // signal start of traversal
    fn(BST_TRAVERSAL_START,0,0,(void *)0);
    // execute traversal
    n = BSTTraversal(pRoot,0,fn,BST_PREORDER);
    // signal end of traversal
    fn(BST_TRAVERSAL_END,n,0,(void *)0);
    return 1;
}

int BST::PostOrder(int fn(BST_TRAVERSAL_OP, int, int, void *))
{
    int n;
    // signal start of traversal
    fn(BST_TRAVERSAL_START,0,0,(void *)0);
    // execute traversal
    n = BSTTraversal(pRoot,0,fn,BST_POSTORDER);
    // signal end of traversal
    fn(BST_TRAVERSAL_END,n,0,(void *)0);
    return 1;
}    
    
int BST::Insert(int key, void *pData)
{
    BSTNode *p, *pNew;
    int n, nLeft, nRight;
    
    pNew = (BSTNode *) new(BSTNode);
    if ( ! pNew )
    {
        return 0;
    }
    
    pNew->key = key;
    pNew->n = 1;
    pNew->pLeft = pNew->pRight = pNew->pPrev = (BSTNode *)0;
    pNew->pData = pData;
    
    if ( pRoot == (BSTNode *) 0)
    {
        pRoot = pNew;
        return 1;
    }
    
    p = BSTSearch(pRoot, key);
    
    if ( !p )
    {
        return 0;
    }
    
    if ( p->key == key )
    {
        // found a node with same key value, insert new node between p and its left child
        pNew->pLeft = p->pLeft;
        if (pNew->pLeft != (BSTNode *)0)
        {
            pNew->pLeft->pPrev = pNew;
            pNew->n += pNew->pLeft->n;
        }
        p->pLeft = pNew;
    } 
    else if (key < p->key)
    {
        // must insert as p's left child
        p->pLeft = pNew;
    }
    else
    {
        // must insert as p's right child
        p->pRight = pNew;
    }
    pNew->pPrev = p;
    
    // now traverse up to the root, incrementing the count of children along the way
    p = pNew;
    do {
        p = p->pPrev;
        p->n += 1;
    } while ( p != pRoot );
    
}


        
void BST::Scope(void fn(BSTNode *))
{
    BSTScope(pRoot,fn);
}
    
