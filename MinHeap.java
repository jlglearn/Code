public class MinHeap<Key extends Comparable<Key>> 
{
    private Key[] items;
    private int nSize;
    private int nElements;
    
    private final int MINSIZE = 512;
    
    public MinHeap()
    {
        nSize = 0;
        nElements = 0;
        resize();
    }
    
    public MinHeap(Key[] v)
    {
        resize(v.length);  
        
        for (int i = 0; i < v.length; i++)
            items[i] = v[i];
            
        if (v.length > 1)
            heapifyUp(v.length / 2);
    }
    
    public void insert(Key v)
    {
        if (nElements >= nSize)
            resize(nSize * 2);
        items[nElements++] = v;
        heapifyUp(nElements);
    }
    
    public int size()
    {   return nElements;    }
    
    public Key delMin()
    {
        Key k = min();    
        swap(1, nElements);
        items[--nElements] = null;
        heapifyDown(1);
        return k;
    }
    
    public Key min()
    {
        if (nElements == 0)
            throw new java.lang.IndexOutOfBoundsException();
        return items[0];
    }
    
    public boolean isEmpty()
    {   return nElements == 0;  }
    
    private void resize()
    {   resize(nSize * 2);  }
    
    private void resize(int newsize)
    {
        int k = nSize;
        if (k == 0) k = MINSIZE;
        if (newsize < MINSIZE) newsize = MINSIZE;
        
        while (k < newsize) k *= 2;
        
        Key[] newitems = (Key[]) new Comparable[k];
        
        if (nElements > 0)
        {
            for (int i = 0; i < nElements; i++)
            {
                newitems[i] = items[i];
                items[i] = null;
            }
            items = null;
        }
        
        items = newitems;
        nSize = k;
    }
    
    /* heapifyUp(int index):  NOTE: index expected to be 1-based */
    private void heapifyUp(int index)
    {   
        int i = index;
        while (i > 1)
        {
            int parent = heapParent(i);
            if (less(heapValue(i), heapValue(parent)))
                swap(i,parent);
            else
                break;
            i = parent;
        }
    }
    
    /* heapifyDown(int index): NOTE: index expected to be 1-based */
    private void heapifyDown(int index)
    {
        int left = heapLeft(index);
        int right = heapRight(index);
        int m = index;
        
        if ((left <= nElements) && less(heapValue(left), heapValue(index)))
            m = left;
            
        if ((right <= nElements) && less(heapValue(right), heapValue(m)))
            m = right;
            
        if (m == index)
            return;
            
        swap(m, index);
        heapifyDown(m);
    }
    
    /* swap(int i, int j): NOTE: indices i, j are expected to be 1-based */
    private void swap(int i, int j)
    {
        if ((i <= 0) || (i > nElements) || (j <= 0) || (j > nElements))
            throw new java.lang.IndexOutOfBoundsException();
        
        if (i != j)
        {
            Key t = items[i-1];
            items[i-1] = items[j-1];
            items[j-1] = t;
            t = null;
        }
    }
    
    /* heapValue(int index): NOTE: index expected to be 1-based */
    private Key heapValue(int index)
    {
        if ((index <= 0) || (index > nElements))
            throw new java.lang.IndexOutOfBoundsException();
            
        return items[index-1];
    }
    
    private static boolean less(Comparable v, Comparable w) { return v.compareTo(w) < 0; }
    private static int heapLeft(int i) { return i * 2; }
    private static int heapRight(int i) { return i * 2 + 1; }
    private static int heapParent(int i) { return i / 2; }
    private void dump()
    {
        for (int i = 0; i < nElements; i++)
            StdOut.println("[" + (i+1) + "]: " + items[i]);
    }
    
    
    public static void main(String[] argv)
    {
    }
    
 }
