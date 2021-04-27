#include <fstream>
using namespace std;
ifstream fin("abce.in");
ofstream fout("abce.out");

//splay trees are BSTs that bring the recently accessed item to root of the tree
//ZIG/ZAG = Node is child of the root (has no grand-parent) and is a right child (we do left rotation) or left child (we do right rotation)
//ZIG-ZIG/ZAG-ZAG = Node is left child of parent and parent is left child of grand parent (2 right rotation) or right child of parent and parent is right child of grand parent (2 left rotation)
//ZIG-ZAG/ZAG-ZIG = Node is left child of parent and parent is right child of grand parent (left rotation + right rotation) or right child of parent and parent is left child of grand parent (right rotation+left rotation)

struct node
{
    int val;
    node* left, * right, * parent;
};

class SplayTree    //class for whole Splay Tree
{
    node* root = NULL;
    node* newNode(int val, node* p) 
    {
        node* aux = new node;
        aux->val = val;
        aux->left = NULL;
        aux->right = NULL;
        aux->parent = p;
        return aux;
    }
    public:
    node* getroot()
    {
        return root;
    }

    node* minval(node* c)  //searching for the minimum val of splaytree
    {
        while (c->left != NULL) c = c->left;
        return c;
    }

    void rightRotation(node* x)   //right rotation for subtree that has x as root, and then y as root
    {
        node* y = x->left;
        if (x->left != NULL)  x->left = y->right;
        if (y->right != NULL) y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == NULL) root = y;
        else if (x->parent->left == x)  x->parent->left = y;
             else  x->parent->right = y;
        y->right = x;
        x->parent = y;
    }

    void leftRotation(node* x)   //left rotation for subtree that has x as root, and then y as root
    {
        node* y = x->right;
        if (x->right != NULL)  x->right = y->left;
        if (y->left != NULL)   y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == NULL) root = y;
        else if (x->parent->left == x)  x->parent->left = y;
        else     x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    //splaying
    void splay(node* x)    //brings the wanted node as root or the last accessed item as root
    {
        if (x == NULL)  return;
        while (x->parent != NULL)
            if (x->parent->parent == NULL)   //zig or zag
                if (x->parent->left == x)  rightRotation(x->parent); // zig 
                else   leftRotation(x->parent); //zag
            else
                if (x->parent->left == x)    //zig-zig or zig-zag
                    if (x->parent->parent->left == x->parent)   //zig-zig
                    {
                        rightRotation(x->parent->parent);
                        rightRotation(x->parent);
                    }
                    else    //zig-zag
                    {
                        rightRotation(x->parent);
                        leftRotation(x->parent);
                    }
                else    //zag-zig or zag-zig
                {
                    if (x->parent->parent->right == x->parent)   //zag-zag
                    {
                        leftRotation(x->parent->parent);
                        leftRotation(x->parent);
                    }
                    else   //zag-zig
                    { 
                        leftRotation(x->parent);
                        rightRotation(x->parent);
                    }
                }
            }
    //insertion
    void insert(int val)
    {
        if (root == NULL)  //splaytree is empty
        {
            root = newNode(val, NULL); 
            return;
        }
        node* c = root;     // start with root node
        node* parent = NULL;   // parent of root
        while (c != NULL)   //searching for parent of the new node
        {
            parent = c;
            if (c->val > val) c = c->left;   
            else c = c->right;
        }
        node* newnode = newNode(val, parent);   //creating the new node
        //linking the new node
        if (val < parent->val)  parent->left = newnode;   
        else parent->right =  newnode;
        splay(newnode); //making it the root
    }

    node* find(int val)
    {
        node* c = root;  // start with root node
        while (c != NULL && c->val != val)  // searching the val
        {
            if (val < c->val)   // if given val is less than the cent node
                c = c->left;    // go to left subtree
            else
                c = c->right;   // else go to right subtree
        }
        if (c == NULL)  return NULL;
        else return c;
    }
    void delete_node(int val)
    {
        node* c = find(val);
        splay(c);
        node* parent = c->parent;
        if (c == NULL)  return;
        if (c->left == NULL && c->right == NULL)     //node has no children
        {
            if (c != root)  //if the wanted node is not the root
                if (parent->left == c) parent->left = NULL;
                else  parent->right = NULL;
            else  root = NULL; //if it is the root, the splaytree become empty
            delete c;
        }
        else if (c->left != NULL && c->right != NULL)  //node has two children
        {
            node* successor = minval(c->right);
            int Val = successor->val;
            delete_node(successor->val);   // recursively delete the successor (it has at mot one child-the right one)
            c->val = Val;    // Copy the val of successor to cent node
        }
        else   //node has one children
        {
            node* child;
            if (c->left != NULL) child = c->left;
            else   child = c->right;
            if (c != root)   // if node to be deleted is not a root node then set its parent to its child
                if (c == parent->left) parent->left = child;
                else  parent->right = child;
            else   // if node to be deleted is root node, then set the root to child
            {
                root = child;
                child->parent = NULL;
            }
            delete c;
        }
    }

    //the largest value, smaller than or equal with given value
    int maxi(int val)
    {
        node* c = root, * parent = NULL;
        while (c != NULL)
        {
            if (val == c->val) { splay(c); return c->val; }
            if (val < c->val)  c = c->left;
            else  parent = c, c = c->right;
        }
        splay(parent);
        return parent->val;
    }

    //the smallest value, larger than or equal with given value
    int mini(int val)
    {
        node* c = root, * parent = NULL;
        while (c != NULL)
        {
            if (val == c->val) { splay(c); return c->val; }
            if (val < c->val)  parent = c,  c = c->left;
            else  c = c->right; 
        }
        splay(parent);
        return parent->val;
    }

    //inOrder Traversion
    void inOrder(node* root)   
    {
        if (root == NULL) return;
        inOrder(root->left);
        fout << root->val << " ";
        inOrder(root->right);
    }

    //printing values in range [x, y]
    void print(node* root, int x, int y)
    {
        if (root == NULL)  return;
        if (x < root->val)  print(root->left, x, y);
        if (x <= root->val && y >= root->val)  fout << root->val << " ";
        if (y > root->val) print(root->right, x, y);
    }

   //replacing node x with node y
   void replace(node* x, node* y)
    {
        if (x->parent == NULL) root = y; 
        else if (x == x->parent->left)  x->parent->left = y;   //if x left child
             else  x->parent->right = y;   //if x right child
        if (y) y->parent = x->parent;  
    }


    void deletion(const int& x)
    {
        node* Node = find(x);
        if (Node == NULL)  return; // not in the tree
        splay(Node);   //we bring the wanted node to the root position
        if (Node->left == NULL) replace(Node, Node->right);  //if the root (the node we want to delete) has only one child (the right one)
        else if (Node->right == NULL)  replace(Node, Node->left);  //     -//- with the other child
             else   //if it has both children
                  {
                    node* m = get_mini(Node->right);     //we search for the smallest value
                    if (m->parent != Node)     //we transform the minimum node in leaf if is not child of root
                    {
                        replace(m, m->right);      
                        m->right = Node->right;   //we link it to the right child of node
                        m->right->parent = m;
                    }
                    replace(Node, m);     //we exchange places between Node and the minimum
                    m->left = Node->left;  //we link the left child to it
                    m->left->parent = m;
                   }

        delete Node;
    }
    node* get_mini(node* x) const
    {
        if (x == NULL)  return NULL;
        node* Node = x;
        while (Node->left != NULL) Node = Node->left;  //we get the smallest value (it is in the most "left" part of the tree
        return Node;
    }
};

int main()
{
    int Q, task, x, y;
    SplayTree ST;
    fin >> Q;
    for(int i = 1; i <= Q; i++)
    {
        fin >> task >> x;
        if (task == 1)   ST.insert(x);
        if (task == 2)   ST.deletion(x);
        if (task == 3)
            if (ST.find(x) == NULL) fout << "0\n";
            else  fout << "1\n";
        if (task == 4) fout << ST.maxi(x) << "\n";
        if (task == 5) fout << ST.mini(x) << "\n";
        if (task == 6)
        {
            fin >> y;
            ST.print(ST.getroot(), x, y);
            fout << "\n";
        }
    }
    return 0;
}