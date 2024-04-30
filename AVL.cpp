/*
--AVL-ul este un arbore in care diferenta dintre subarborii determinati de orice nod este maxim -1, 0 sau 1.
*/
#include <iostream>
struct Node{
    int key;
    Node *left, *right;
    int height;
};
void print(Node* root) {
    if (root->left != NULL)
        print(root->left);
    std::cout << root->key << " ";
    if (root->right != NULL)
        print(root->right);
}
void RSD(Node *root)
{
    if(root!=NULL)
    {
        std::cout<<root->key<<" ";
        RSD(root->left);
        RSD(root->right);
    }
}
int find_height(Node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}
int balance(Node *node)
{
    if (node == NULL)
        return 0;
    return find_height(node->left) - find_height(node->right);
}
/*
--Acum urmeaza functiile care transforma din BST in AVL!!
--Presupunand ca pana in momentul inserarii avem deja un AVL, diferenta balance-ul este deja maxim 1.
--Pentru a rebalansa arborele, avem nevoie de rotatii. O astfel de rotatie ne da cealalta varianta de asezare a cheilor in BST.
--De exemplu, daca avem:
    3
   /
  2
 /
1,
transformam in:
        2
       / \
      1   3
--Rotatia de mai sus este o rotatie la dreapa. Analog definim rotatia la stanga.
--Identificam 4 moduri in care putem face rotatiile pentru a rebalansa arborele (T1, T2, T3, T4 sunt toti subarbori):

1. LEFT-LEFT:
         z                                      y
        / \                                   /   \
       y   T4      rotatie_dreapta(z)        x      z
      / \          - - - - - - - - ->      /  \    /  \
     x   T3                               T1  T2  T3  T4
    / \
  T1   T2

2. LEFT-RIGHT:
     z                               z                           x
    / \                            /   \                        /  \
   y   T4  rotatie_stanga(y)      x    T4  rotatie_dreapta(z) y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2   T3                    T1   T2

3. RIGHT-RIGHT:
  z                                y
 /  \                            /   \
T1   y     roatie_stanga(z)     z      x
    /  \   - - - - - - - ->    / \    / \
   T2   x                     T1  T2 T3  T4
       / \
     T3  T4

4. RIGHT-LEFT:
   z                            z                            x
  / \                          / \                          /  \
T1   y   rotatie_dreapta(y)  T1   x   rotatie_stanga(z)   z      y
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   x   T4                      T2   y                  T1  T2  T3  T4
  / \                              /  \
T2   T3                           T3   T4
*/
Node *rotatie_dreapta(Node *y)
{
    Node *x = y->left;
    Node *temp = x->right;

    // rotatia propriu-zisa
    x->right = y;
    y->left = temp;

    // iau inaltimile
    y->height = std::max(find_height(y->left),find_height(y->right)) + 1;
    x->height = std::max(find_height(x->left),find_height(x->right)) + 1;

    return x;
}


Node *rotatie_stanga(Node *x)
{
    Node *y = x->right;
    Node *temp = y->left;

    // rotatia
    y->left = x;
    x->right = temp;

    //inaltimile
    x->height = std::max(find_height(x->left),find_height(x->right)) + 1;
    y->height = std::max(find_height(y->left),find_height(y->right)) + 1;

    // radacina
    return y;
}
Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}
Node* insert(int n, Node* nod)
{
    if (nod == NULL)
        return(newNode(n));

    if(n<nod->key)
        nod->left= insert(n,nod->left);
    else if(n>nod->key)
        nod->right= insert(n,nod->right);
    else
        return nod;
    nod->height = 1 + std::max(find_height(nod->left),find_height(nod->right));
    int b= balance(nod);
    // Left Left Case
    if (b > 1 && n < nod->left->key)
        return rotatie_dreapta(nod);

    // Right Right Case
    if (b < -1 && n > nod->right->key)
        return rotatie_stanga(nod);

    // Left Right Case
    if (b > 1 && n > nod->left->key)
    {
        nod->left = rotatie_stanga(nod->left);
        return rotatie_dreapta(nod);
    }

    // Right Left Case
    if (b < -1 && n < nod->right->key)
    {
        nod->right = rotatie_dreapta(nod->right);
        return rotatie_stanga(nod);
    }
    return nod;
}

Node* search(int n, Node* root) {
    while (root != nullptr && root->key != n) {
        if (n > root->key)
            root = root->right;
        else
            root = root->left;
    }
    if (root != nullptr && root->key == n)
        return root;
    else
        return nullptr;
}

Node * succesor(Node* node)
{
    Node* current = node;
    while (current->left != NULL)
        current = current->left;

    return current;
}
Node* erase(int n, Node* root)
{

    if (root == NULL)
        return root;
    if ( n < root->key )
        root->left = erase(n, root->left);
    else if( n > root->key )
        root->right = erase(n, root->right);
    else
    {
        // daca nodul are un singur fiu sau niciunul
        if( (root->left == NULL) ||
            (root->right == NULL) )
        {
            Node *temp = root->left ? root->left : root->right;

            // daca nodul  nu are fii
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // un singur  fiu
                *root = *temp;
            free(temp);
        }
        else
        {
            //daca nodul are doi  fii
            Node* temp = succesor(root->right);
            root->key = temp->key;
            root->right = erase(temp->key, root->right);
        }
    }
    //daca am ramas cu un singur nod, well
    if (root == NULL)
        return root;

    // iau inaltimea
    root->height = 1 + std::max(find_height(root->left),find_height(root->right));

    //fac balance ca sa vad daca e unbalaced AVL-ul
    //daca este, trec la cele 4 cazuri de rotatie
    int b = balance(root);

    // Left Left Case
    if (b > 1 &&
        balance(root->left) >= 0)
        return rotatie_dreapta(root);

    // Left Right Case
    if (b > 1 &&
        balance(root->left) < 0)
    {
        root->left = rotatie_stanga(root->left);
        return rotatie_dreapta(root);
    }

    // Right Right Case
    if (b < -1 &&
        balance(root->right) <= 0)
        return rotatie_stanga(root);

    // Right Left Case
    if (b < -1 &&
        balance(root->right) > 0)
    {
        root->right = rotatie_dreapta(root->right);
        return rotatie_stanga(root);
    }
    return root;
}
int main() {
    Node* root=NULL;
    root=insert(12, root);
    root=insert(8,root);
    root=insert(11,root);
    root=insert(5,root);
    root=insert(4,root);
    root=insert(18,root);
    root=insert(17,root);
    RSD(root);
    std::cout<<std::endl;
    print(root);
    std::cout<<std::endl;
    erase(12,root);
    RSD(root);
    std::cout<<std::endl;
    std::cout<<balance(root);
    return 0;
}
