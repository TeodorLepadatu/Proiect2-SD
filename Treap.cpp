#include <iostream>
#include <fstream>
std::ifstream fin("date.in");
std::ofstream fout("date.out");
struct Treap {
    int key, p; //p=pritoriy
    Treap *left, *right;
    Treap() {}
    Treap(int key, int p, Treap* left, Treap* right) {
        this->key = key;
        this->p = p;
        this->left = left, this->right = right;
    }
} *R, *nil; // nil indica un nod 'gol'
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
        if (root->left)
            RSD(root->left);
        if (root->right)
            RSD(root->right);
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
void split(Treap* &R, Treap* &Ts, Treap* &Tg, int key) {    //face split dupa radacina
    const long long infinity = 9223372036854775807;
    R=insert(R, key, infinity);
    Ts = R->left, Tg = R->right;
    delete R, R = nil;
}
void join(Treap* &R, Treap* Ts, Treap* Tg, int key) {   //face join cu o cheie convenabila, ca altfel nu merge
    R = new Treap(key, 0, Ts, Tg);
    Delete(R, R->key);
}
int main() {
    Treap* root=NULL;
    long long n;
    fin>>n;
    for(long long i=1;i<=n;i++)
    {
        int type;
        fin>>type;
        if(type==1)
        {
            int x,y;
            fin>>x>>y;
            root=insert(root,x,y);
        }
        else if(type==2)
        {
            int x;
            fin>>x;
            root=Delete(root,x);
        }
        else if(type==3)
        {
            int x;
            fin>>x;
            Treap* afis=search(root,x);
            if(afis == nullptr)
                fout<<0<<std::endl;
            else
                fout<<1<<std::endl;
        }
    }
    return 0;
}
