#include <memory>

struct structBSTNode {
    int key;
    structBSTNode *pLeft, *pRight, *pPrev;
    void *pData;
    int n;
};

typedef structBSTNode BSTNode;
typedef enum { BST_TRAVERSAL_START, BST_TRAVERSAL_ITEM, BST_TRAVERSAL_END } BST_TRAVERSAL_OP;
    
class BST {
    
public:
    BST(void);
    ~BST(void);
    int Insert(int key, void *pData);
    void * Search(int key);
    void * Min();
    void * Max();
    int InOrder(int fn(BST_TRAVERSAL_OP, int, int, void *));
    int PreOrder(int fn(BST_TRAVERSAL_OP, int, int, void *));
    int PostOrder(int fn(BST_TRAVERSAL_OP, int, int, void *));
    void Scope(void fn(BSTNode *));
private:
    BSTNode *pRoot;
};