#include <iostream>
#include <fstream>
#include <climits>
std::ifstream fin("abce.in");
std::ofstream fout("abce.out");
struct Treap {
    long long key, p; //p=pritoriy
    Treap *left, *right;
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
Treap* New_node(long long x)
{
    Treap* nod= new Treap;
    nod->key=x;
    nod->p= rand();
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
Treap* insert(Treap* root, long long key)
{
    if (!root)
        return New_node(key);

    if (key <= root->key)
    {
        root->left = insert(root->left, key);

        // proprietatea de heap
        if (root->left->p >= root->p)
            root = rot_right(root);
    }
    else
    {
        root->right = insert(root->right, key);

        // proprietatea de heap se rezolva cu rotatie
        if (root->right->p > root->p)
            root = rot_left(root);
    }
    return root;
}
Treap* search(Treap* node, long long key) {
    if(node == nullptr)
        return nullptr;
    if(node->key == key)
        return node;
    if(key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

Treap* Delete(Treap* root, long long key)
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
    else if (root->left->p <= root->right->p)
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
long long fix_mai_mare(long long n, Treap* root) {
    if (!root)
        return LLONG_MAX;

    long long dif = LLONG_MAX;
    long long closest = LLONG_MAX;
    Treap* candidat = root;

    while (candidat) {
        long long current_dif = candidat->key - n;
        if (current_dif >= 0 && current_dif <= dif) {
            dif = current_dif;
            closest = candidat->key;
        }

        if (candidat->key > n) {
            candidat = candidat->left;
        } else {
            candidat = candidat->right;
        }
    }

    return closest;
}

long long fix_mai_mic(long long n, Treap* root) {
    if (!root)
        return LLONG_MIN;

    long long dif = -LLONG_MAX;
    long long closest = LLONG_MAX; // e doar limita, do not panic
    Treap* candidat = root;
    while (candidat) {
        long long current_dif = candidat->key - n;
        if(current_dif <= 0 && std::abs(current_dif) <= std::abs(dif))
        {
            closest=candidat->key;
            dif=current_dif;
        }
        if (candidat->key > n) {
            candidat = candidat->left;
        } else {
            candidat = candidat->right;
        }
    }

    return closest;
}
void afis_interval(long long m, long long n, Treap* root)
{
    if(root->left!=NULL)
        afis_interval(m,n,root->left);
    if(m <= root->key && root->key <= n)
        fout<<root->key<<" ";
    if(root->right!=NULL)
        afis_interval(m,n,root->right);
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
            long long x;
            fin>>x;
            root=insert(root,x);
        }
        else if(type==2)
        {
            long long x;
            fin>>x;
            root=Delete(root,x);
        }
        else if(type==3)
        {
            long long x;
            fin>>x;
            Treap* afis=search(root,x);
            if(afis == nullptr)
                fout<<0<<std::endl;
            else
                fout<<1<<std::endl;
        }
        else if(type==4)
        {
            long long x;
            fin>>x;
            fout<<fix_mai_mic(x,root)<<std::endl;
        }
        else if(type==5)
        {
            long long x;
            fin>>x;
            fout<<fix_mai_mare(x,root)<<std::endl;
        }
        else if(type==6)
        {
            long long x,y;
            fin>>x>>y;
            afis_interval(x,y,root);
            fout<<std::endl;
        }
    }
    return 0;
}
