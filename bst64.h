#ifndef _BST64_H_
#define _BST64_H_
#include <cstdlib>

#define BST64_NODEBLOCKSIZE (256*1024)

typedef enum { BST64_TRAVERSAL_START, BST64_TRAVERSAL_ITEM, BST64_TRAVERSAL_END } BST64TraversalOp;

struct structBST64Node {
    int64_t key;
    void *pData;
    structBST64Node *pLeft, *pRight, *pParent;
    int size;
};

typedef structBST64Node BST64Node;

struct structBST64NodeBlock {
    BST64Node nodes[BST64_NODEBLOCKSIZE];
    int size;
    int length;
    structBST64NodeBlock *pNext;
};

typedef structBST64NodeBlock BST64NodeBlock;

class BST64 {
public:
    BST64(void);
    ~BST64(void);

    int Insert(int64_t key, void *pData);
    int Search(int64_t key, void **ppData);
    int InOrder(int fn(BST64TraversalOp, int, int64_t, void *));
    int PreOrder(int fn(BST64TraversalOp, int, int64_t, void *));
    int PostOrder(int fn(BST64TraversalOp, int, int64_t, void *));
    
private:
    BST64NodeBlock *pNodes;
    BST64Node *pRoot;
    
};

#endif