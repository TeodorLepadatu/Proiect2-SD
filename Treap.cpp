#include <iostream>
struct Treap
{
    int key,p;
    Treap *left, *right;
};
void SRD(Treap* root)
{
    if(root!=NULL) {
        SRD(root->left);
        std::cout << root->key << " " << root->p;
        std::cout<<std::endl;
        SRD(root->right);
    }
}
void RSD(Treap* root)
{
    if(root!=NULL)
    {
        std::cout<<root->key<<" "<<root->p<<std::endl;
        if (root->left) RSD(root->left);
        if (root->right) RSD(root->right);
    }
}
Treap* New_node(int x, int val)
{
    Treap* nod= new Treap;
    nod->key=x;
    nod->p=val;
    nod->left=nod->right=NULL;
    return nod;
}
Treap* rot_right(Treap* y)
{
    Treap *x = y->left;
    Treap *temp = x->right;

    // rotatia propriu-zisa
    x->right = y;
    y->left = temp;
    return x;
}

Treap* rot_left(Treap* x)
{
    Treap *y = x->right;
    Treap *temp = y->left;

    // rotatia propriu-zisa
    y->left = x;
    x->right = temp;

    return y;
}
Treap* insert(Treap* root, int key,int val)
{
    if (!root)
        return New_node(key,val);

    if (key <= root->key)
    {
        root->left = insert(root->left, key, val);

        // proprietatea de heap
        if (root->left->p > root->p)
            root = rot_right(root);
    }
    else
    {
        root->right = insert(root->right, key, val);

        // proprietatea de heap se rezolva cu rotatie
        if (root->right->p > root->p)
            root = rot_left(root);
    }
    return root;
}

Treap* search(Treap* node, int key) {
    if(node == nullptr) 
        return nullptr;

    if(node->key == key)
        return node;
    if(key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

Treap* Delete(Treap* root, int key)
{

    if (root == NULL) return root;

    // daca cheia nu e la radacina
    if (key < root->key)
        root->left = Delete(root->left, key);
    else if (key > root->key)
        root->right = Delete(root->right, key);

    // daca cheia e la radacina (CODUL LUI DUMITRAN NU TRATEAZA CAZUL ASTA SI IMI DA SEGFAULT, DECI NICI NU STITI CAT M AM ENERVAT CU TREABA ASTA

    else if (root->left == NULL)
    {
        Treap *temp = root->right;
        delete root;
        root = temp; // copilul din dreapta va fi radacina
    }

    else if (root->right == NULL)
    {
        Treap *temp = root->left;
        delete root;
        root = temp; // copilul din stanga va fi radacina
    }

        // daca ambii copii sunt nuli
    else if (root->left->p < root->right->p)
    {
        root = rot_left(root);
        root->left = Delete(root->left, key);
    }
    else
    {
        root = rot_right(root);
        root->right = Delete(root->right, key);
    }

    return root;
}

int main() {
    Treap* root=NULL;
    root=insert(root,50,15);
    root=insert(root,30,5);
    root=insert(root,20,2);
    root=insert(root,40,4);
    root=insert(root,70,10);
    root=Delete(root,50);
    SRD(root);
    std::cout<<std::endl;
    RSD(root);
    return 0;
}
