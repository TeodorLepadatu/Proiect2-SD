#include <iostream>
#include <fstream>
#include <algorithm>
#define INTMIN -2147483648 
#define INTMAX 2147483647 
using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

class BTreeNode {
  int *keys;
  int t;
  BTreeNode **C;
  int n;
  bool leaf;

public:
  BTreeNode(int _t, bool _leaf);
  void traverse();

  int findKey(int k);
  void insertNonFull(int k);
  void splitChild(int i, BTreeNode *y);
  void deletion(int k);
  void removeFromLeaf(int idx);
  void removeFromNonLeaf(int idx);
  int getPredecessor(int idx);
  int getSuccessor(int idx);
  void fill(int idx);
  void borrowFromPrev(int idx);
  void borrowFromNext(int idx);
  void merge(int idx);
  BTreeNode* search(int k);
  int smaller(int k);
  int greater(int k);
  void range(int X, int Y);
  friend class BTree;
};

class BTree {
  BTreeNode *root;
  int t;

   public:
  BTree(int _t) {
    root = NULL;
    t = _t;
  }

  void traverse() {
    if (root != NULL)
      root->traverse();
  }

  void insertion(int k);
  void deletion(int k);
  BTreeNode* search(int k) {
     return (root == NULL) ? NULL : root->search(k);
  }
  
  int smaller(int k);
  int greater(int k);
  void range(int X, int Y);
};

void BTreeNode::range(int X, int Y) {
    for(int i = 0; i < n; ++i) {
        if(keys[i] >= X) {
            if(!leaf)
                C[i]->range(X, Y);
            if(keys[i] <= Y)
                fout << keys[i] << " ";
        }
    }

    if(!leaf) 
        C[n]->range(X, Y);
}

void BTree::range(int X, int Y) {
    root->range(X, Y);
}

int BTree::greater(int k) { // ret cea mai mica valoare >= k
    return root->greater(k);
}

int CB2(int* keys, int n, int k) { // cea mai din stanga poz >= k
    int left = 0, right = n - 1;
    int possible = -1;
    while(left <= right) {
        int middle = (left + right) / 2;
        if(keys[middle] >= k) {
            possible = middle;
            right = middle - 1;
        }
        else 
            left = middle + 1;
    }
    return possible;
}

int BTreeNode::greater(int k) {
    // cautam in nodul curent cea mai din stanga cheie >= k
    int p = CB2(keys, n, k);
    if(p == -1) {// toate cheile din nodul curent sunt < k
        if(leaf) 
            return INTMAX;
        else return C[n]->greater(k);
    }
    else {
        if(leaf)
            return keys[p];
        else return min(keys[p], C[p]->greater(k));
    }
}

int BTree::smaller(int k) { // returneaza cea mai mare valoare <= k
    return root->smaller(k);
}

int CB(int* keys, int n, int k) { // cea mai din dreapta valoare <= k
    int left = 0, right = n - 1;
    int possible = -1;
    while(left <= right) {
        int middle = (left + right) / 2;
        if(keys[middle] <= k) {
            possible = middle;
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }

    return possible;
}


int BTreeNode::smaller(int k) {
    // cautam in nodul curent cea mai din dreapta cheie  <= k
    int p = CB(keys, n, k);
    if(p == -1) {// toate cheile din nodul curent sunt mai mari 
        if(leaf) return INTMIN;
        else return C[0]->smaller(k);
    }
    else {
        if(leaf) return keys[p];
        return max(keys[p], C[p + 1]->smaller(k));
    }
}

BTreeNode::BTreeNode(int t1, bool leaf1) {
  t = t1;
  leaf = leaf1;

  keys = new int[2 * t - 1];
  C = new BTreeNode *[2 * t];

  n = 0;
}

BTreeNode *BTreeNode::search(int k) {
  int i = 0;
  while (i < n && k > keys[i])
    i++;

  if (i < n && keys[i] == k)
    return this;

  if (leaf == true)
    return NULL;

  return C[i]->search(k);
}

int BTreeNode::findKey(int k) { // se cauta pozitia lui k in lista de chei ale nodului curent
  int idx = 0;
  while (idx < n && keys[idx] < k)
    ++idx;
  return idx; // idx == n || keys[idx] >= k
}


void BTreeNode::deletion(int k) { // se sterge valoarea k din nodul curent
  int idx = findKey(k); // se cauta pozitia unde se afla k

  if (idx < n && keys[idx] == k) { // daca chiar e in nodul asta
    if (leaf) // daca nodul e frunza
      removeFromLeaf(idx);
    else
      removeFromNonLeaf(idx); 
  } 
  
  else { // valoarea k nu e in nodul curent
    if (leaf) {
      cout << "The key " << k << " is does not exist in the tree\n";
      return;
    }

    bool flag; // flag = true <=> k > keys[n - 1]
    if(idx == n) {
        flag = true;
    }

    else {
        flag = false;
    }

    if (C[idx]->n < t) // copilul din dreapta nu are numarul minim de chei
      fill(idx);

    if (flag && idx > n) // ?? nu mi dau seama cand e adevarata conditia asta cu idx > n
      C[idx - 1]->deletion(k);
    else
      C[idx]->deletion(k); // key[idx] > k => ma duc in copilul lui din stanga ca acolo sunt valorile < key[idx] si ma duc sa l sterg de acolo
  }
  return;
}

void BTreeNode::removeFromLeaf(int idx) { // stergem cheia cu indexul inx
  for (int i = idx + 1; i < n; ++i)
    keys[i - 1] = keys[i];

  n--;
  return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
  int k = keys[idx];

  if (C[idx]->n >= t) { // pot sa sterg din fiul din stanga
    int pred = getPredecessor(idx);
    keys[idx] = pred; // suprascriu valoarea cu predecesorul sau
    C[idx]->deletion(pred); // sterg predecesorul din nodul in care se afla
  }

  else if (C[idx + 1]->n >= t) { // pot sa sterg din fiul din stanga
    int succ = getSuccessor(idx); 
    keys[idx] = succ; // suprascriu cu succesorul
    C[idx + 1]->deletion(succ); // sterg succesorul
  }

  else { // nu pot sa sterg de nicaieri
    merge(idx); // reunesc fiul din stanga cu cel din dreapta
    C[idx]->deletion(k); // sterg valoarea pe care trebuia sa o sterg
  }
  return;
}

int BTreeNode::getPredecessor(int idx) {
  BTreeNode *cur = C[idx]; // trebuie sa accesam cea mai din dreapta valoare: un pas in stanga, dupa numai dreapta
  while (!cur->leaf)
    cur = cur->C[cur->n]; // ma duc mereu pe cel mai din dreapta copil

  return cur->keys[cur->n - 1]; // nu mai am unde sa cobor, arunc valoarea cea mai din dreapta
}

int BTreeNode::getSuccessor(int idx) { // un pas in dreapta, restul numai in stanga
  BTreeNode *cur = C[idx + 1]; // ma duc pe copilul din dreapta
  while (!cur->leaf) // cat mai pot coborî, ma duc in stanga
    cur = cur->C[0];

  return cur->keys[0]; // se ia cea mai din stanga valoare
}

void BTreeNode::fill(int idx) {
  if (idx != 0 && C[idx - 1]->n >= t) // ia de la fratele mai mic
    borrowFromPrev(idx);

  else if (idx != n && C[idx + 1]->n >= t) // ia de la fratele mai mare
    borrowFromNext(idx);

  else { // nu are cine sa il imprumute, deci parintele isi uneste cei 2 copii
    if (idx != n)
      merge(idx);
    else
      merge(idx - 1);
  }
  return;
}

void BTreeNode::borrowFromPrev(int idx) { // prelucram key[idx] pentru nodul curent
  BTreeNode *child = C[idx]; // imprumuta o cheie de la fratele lui
  BTreeNode *sibling = C[idx - 1];

  for (int i = child->n - 1; i >= 0; --i) // eliberam poz 0 din keys ca sa putem pune acolo ce imprumutam
    child->keys[i + 1] = child->keys[i];

  if (!child->leaf) { // daca are copii de mutat, ii mutam si pe aia cu o poz la dreapta
    for (int i = child->n; i >= 0; --i)
      child->C[i + 1] = child->C[i];
  }

  child->keys[0] = keys[idx - 1];

  if (!child->leaf)
    child->C[0] = sibling->C[sibling->n];

  keys[idx - 1] = sibling->keys[sibling->n - 1];

  child->n += 1;
  sibling->n -= 1;

  return;
}

void BTreeNode::borrowFromNext(int idx) {
  BTreeNode *child = C[idx];
  BTreeNode *sibling = C[idx + 1];

  child->keys[(child->n)] = keys[idx];

  if (!(child->leaf))
    child->C[(child->n) + 1] = sibling->C[0];

  keys[idx] = sibling->keys[0];

  for (int i = 1; i < sibling->n; ++i)
    sibling->keys[i - 1] = sibling->keys[i];

  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->n; ++i)
      sibling->C[i - 1] = sibling->C[i];
  }

  child->n += 1;
  sibling->n -= 1;

  return;
}

void BTreeNode::merge(int idx) {
  BTreeNode *child = C[idx];
  BTreeNode *sibling = C[idx + 1];

  child->keys[t - 1] = keys[idx];

  for (int i = 0; i < sibling->n; ++i)
    child->keys[i + t] = sibling->keys[i];

  if (!child->leaf) {
    for (int i = 0; i <= sibling->n; ++i)
      child->C[i + t] = sibling->C[i];
  }

  for (int i = idx + 1; i < n; ++i)
    keys[i - 1] = keys[i];

  for (int i = idx + 2; i <= n; ++i)
    C[i - 1] = C[i];

  child->n += sibling->n + 1;
  n--;

  delete (sibling);
  return;
}

void BTree::insertion(int k) {
  if (root == NULL) {
    root = new BTreeNode(t, true);
    root->keys[0] = k;
    root->n = 1;
  } 
  
  else {
    if (root->n == 2 * t - 1) { // trebuie sa dam split la radacina
      BTreeNode *s = new BTreeNode(t, false);

      s->C[0] = root;

      s->splitChild(0, root);

      int i = 0;
      if (s->keys[0] < k)
        i++;
      s->C[i]->insertNonFull(k);

      root = s;
    } else
      root->insertNonFull(k);
  }
}

void BTreeNode::insertNonFull(int k) {
  int i = n - 1;

  if (leaf == true) {
    while (i >= 0 && keys[i] > k) { // se trag spre dreapta cheile, cat sa poata fi inserat k
      keys[i + 1] = keys[i];
      i--;
    }

    keys[i + 1] = k;
    n = n + 1;
  } 

  else {
    while (i >= 0 && keys[i] > k)
      i--;

    // se iese din while cand i == --1 || keys[i] <= k

    if (C[i + 1]->n == 2 * t - 1) { // e full, nu se mai poate insera in el
      splitChild(i + 1, C[i + 1]);

      if (keys[i + 1] < k)
        i++;
    }
    C[i + 1]->insertNonFull(k);
  }
}

void BTreeNode::splitChild(int i, BTreeNode *y) { // indicele copilului pe care trebuie sa il rupem + copilul
  BTreeNode *z = new BTreeNode(y->t, y->leaf);
  z->n = t - 1;

  for (int j = 0; j < t - 1; j++)
    z->keys[j] = y->keys[j + t]; // z preia ultimele t - 1 chei din y

  if (y->leaf == false) { // daca y nu e frunza
    for (int j = 0; j < t; j++)
      z->C[j] = y->C[j + t]; // atunci z preia si ultimii t - 1 copii
  }

  y->n = t - 1;

  for (int j = n; j >= i + 1; j--) // ii facem loc lui z pe poz (i + 1)
    C[j + 1] = C[j];

  C[i + 1] = z; // il punem pe z pe poz (i + 1)

  for (int j = n - 1; j >= i; j--) // eliberam pozitia i din keys
    keys[j + 1] = keys[j];

  keys[i] = y->keys[t - 1]; // punem pe i mediana din y

  n = n + 1; // am mai adaugat o cheie
}

void BTreeNode::traverse() {
  int i;
  for (i = 0; i < n; i++) { // nodul are n chei, n + 1 copii (0...n)
    if (leaf == false)
      C[i]->traverse();
    cout << " " << keys[i];
  }

  if (leaf == false)
    C[i]->traverse();
}

// Delete Operation
void BTree::deletion(int k) {
  if (!root) {
    cout << "The tree is empty\n";
    return;
  }

  root->deletion(k);

  if (root->n == 0) { // nu am nicio cheie in radacina
    BTreeNode *tmp = root;
    if (root->leaf) // daca nici nu are copii
      root = NULL; // putem scapa de ea
    else
      root = root->C[0]; // devine radacina primul ei copil

    delete tmp;
  }
  return;
}

void solve(BTree& t) {

    int no_queries, type, X;
    fin >> no_queries;
    for(int i = 0; i < no_queries; ++i) {
        fin >> type >> X;
        if(type == 1) {
            t.insertion(X);
        }
        else if(type == 2) {
            t.deletion(X);
        }
        else if(type == 3) {
            BTreeNode* ans = t.search(X);
            if(ans != NULL) {
                fout << 1 << "\n";
            }
            else fout << 0 << "\n";
        }
        
        else if(type == 4) {
            fout << t.smaller(X) << "\n"; 
        }

        else if(type == 5) {
            fout << t.greater(X) << "\n";
        }
        
        else {
            int Y;
            fin >> Y;
            if(X > Y) swap(X, Y);
            t.range(X, Y);
            fout << "\n";
        }
        

    }


    fin.close();
    fout.close();
}

int main() {
  BTree t(4);
  solve(t);
}