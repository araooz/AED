#include <bits/stdc++.h>
using namespace std;

struct node{
    int data;
    node* left;
    node* right;
    int bal; //factor de balance
    int count;

    node(int x):data(x), left(nullptr), right(nullptr), bal(0), count(1) {}
};


//LL (A = +2; B = +1) RR (A = -2; B = -1)
//LR (A = +2; B = -1) RL (A = -2; B = +1)
class avl{
private:
    node* root = nullptr;

    void LL(node* &a){      //a = +2; b = +1
        node* b = a->left;
        a->left = b->right;
        b->right = a;

        a->bal = 0;
        b->bal = 0;
        a = b; //nuevo root
    }
    void RR(node* &a){      //a = -2; b = -1
        node* b = a->right;
        a->right = b->left;
        b->left = a;

        a->bal = 0;
        b->bal = 0;
        a = b; //nuevo root
    }
    void LR(node* &a){      //a = +2; b = -1
        node* b = a->left;
        node* c = b->right;

        b->right = c->left;
        c->left = b;
        a->left = c->right;
        c->right = a;

        if(c->bal == 1){ //c estaba a la izquierda
            a->bal = -1;
            b->bal = 0;
        }
        else if(c->bal == 0){ //c estaba en 0
            a->bal = 0;
            b->bal = 0;
        }
        else{ //c estaba a la derecha
            a->bal = 0;
            b->bal = 1;
        }
        c->bal = 0;
        a = c;//nuevo root
    }
    void RL(node* &a){      //a = -2; b = +1
        node* b = a->right;
        node* c = b->left;

        b->left = c->right;
        c->right = b;
        a->right = c->left;
        c->left = a;

        if(c->bal == 1){ //c estaba a la izquierda
            a->bal = 0;
            b->bal = -1;
        }
        else if(c->bal == 0){ //c estaba en 0
            a->bal = 0;
            b->bal = 0;
        }
        else{ //c estaba a la derecha
            a->bal = 1;
            b->bal = 0;
        }
        c->bal = 0;
        a = c;//nuevo root
    }

    void insertPriv(int x, node* &a, bool &h){
        if(a == nullptr){
            a = new node(x);
            h = true;
            return;
        }
        
        if(x < a->data){                //izquierda
            insertPriv(x, a->left, h);
            if(h){ //si crecio la altura
                switch(a->bal){
                    case -1: a->bal = 0; h = false; break;
                    case  0: a->bal = +1;           break;
                    case  1: //antes estaba +1
                        if(a->left->bal == 1) {      LL(a); }
                        else {                       LR(a); }
                        a->bal = 0; //ahora esta 0
                        h = false; //no crecio la altura
                        break;
                    }
            }
        }else if(x > a->data){          //derecha
            insertPriv(x, a->right, h);
            if(h){ //si crecio la altura
                switch(a->bal){
                    case  1: a->bal = 0; h = false; break;
                    case  0: a->bal = -1;           break;
                    case -1: //antes estaba -1
                        if(a->right->bal == -1) {      RR(a); }
                        else {                         RL(a); }
                        a->bal = 0; //ahora esta 0
                        h = false; //no crecio la altura
                        break;
                    }
            }
        }
        else{ //x == a->data
            a->count++;
            h = false; //no crecio la altura
        }
                
    }

    
    // --- Borrado ---
    node* delMin(node* &r, bool &h){           // extrae y retorna el mínimo de r
        if(r->left){
            node* m = delMin(r->left, h);
            if(h) balanceL(r, h);              // encogió izquierda de r
            return m;
        }else{
            node* m = r; r = r->right; h = true;
            return m;
        }
    }

    void eraseRec(int x, node* &a, bool &h){
        if(!a){ h = false; return; }
        if(x < a->data){
            eraseRec(x, a->left, h);
            if(h) balanceL(a, h);              // encogió izquierda
        }else if(x > a->data){
            eraseRec(x, a->right, h);
            if(h) balanceR(a, h);              // encogió derecha
        }else{
            if(a->count > 1){ a->count--; h = false; return; }
            if(!a->left || !a->right){
                node* q = a;
                a = a->left ? a->left : a->right;
                delete q; h = true;
            }else{
                bool h2=false;
                node* m = delMin(a->right, h2);   // quitamos mínimo del derecho
                a->data = m->data; a->count = m->count; delete m;
                if(h2) balanceR(a, h2);           // encogió derecha
                h = h2;
            }
        }
    }

    // --- Rebalanceo tras encogimiento ---
    // balanceL: subárbol izquierdo de A encogió
    void balanceL(node* &A, bool &h){
        switch(A->bal){
            case +1: A->bal = 0;             break;
            case  0: A->bal = -1; h = false; break;
            case -1:{
                node* B = A->right;
                int balB = B->bal;
                if(balB <= 0){               // RR (simple)
                    A->right = B->left; B->left = A;
                    if(balB == 0){ A->bal = -1; B->bal = +1; h = false; }
                    else           { A->bal =  0; B->bal =  0; }
                    A = B;
                }else{                        // RL (doble)
                    node* C = B->left; int balC = C->bal;
                    B->left = C->right;  C->right = B;
                    A->right = C->left; C->left  = A;
                    A->bal = (balC == -1) ? +1 : 0;
                    B->bal = (balC == +1) ? -1 : 0;
                    C->bal = 0; A = C;
                }
            } break;
        }
    }

    // balanceR: subárbol derecho de A encogió
    void balanceR(node* &A, bool &h){
        switch(A->bal){
            case -1: A->bal = 0;             break;
            case  0: A->bal = +1; h = false; break;
            case +1:{
                node* B = A->left;
                int balB = B->bal;
                if(balB >= 0){               // LL (simple)
                    A->left = B->right; B->right = A;
                    if(balB == 0){ A->bal = +1; B->bal = -1; h = false; }
                    else           { A->bal =  0; B->bal =  0; }
                    A = B;
                }else{                        // LR (doble)
                    node* C = B->right; int balC = C->bal;
                    B->right = C->left; C->left  = B;
                    A->left  = C->right; C->right = A;
                    A->bal = (balC == +1) ? -1 : 0;
                    B->bal = (balC == -1) ? +1 : 0;
                    C->bal = 0; A = C;
                }
            } break;
        }
    }

    // --- utilidades ---
    void clearRec(node* a){
        if(!a) return;
        clearRec(a->left);
        clearRec(a->right);
        delete a;
    }
    int heightRec(node* a) const{
        if(!a) return 0;
        return 1 + max(heightRec(a->left), heightRec(a->right));
    }
    long long sizeRec(node* a) const{
        if(!a) return 0;
        return a->count + sizeRec(a->left) + sizeRec(a->right);
    }
    bool findRec(node* a, int x) const{
        while(a){
            if(x < a->data) a = a->left;
            else if(x > a->data) a = a->right;
            else return true;
        }
        return false;
    }
    node* minNode(node* a) const{
        if(!a) return nullptr;
        while(a->left) a = a->left;
        return a;
    }
    node* maxNode(node* a) const{
        if(!a) return nullptr;
        while(a->right) a = a->right;
        return a;
    }
    void inorderRec(node* a) const{
        if(!a) return;
        inorderRec(a->left);
        cout << a->data << "(" << a->bal << "," << a->count << ") ";
        inorderRec(a->right);
    }
    void preorderRec(node* a) const{
        if(!a) return;
        cout << a->data << "(" << a->bal << "," << a->count << ") ";
        preorderRec(a->left);
        preorderRec(a->right);
    }
    void postorderRec(node* a) const{
        if(!a) return;
        postorderRec(a->left);
        postorderRec(a->right);
        cout << a->data << "(" << a->bal << "," << a->count << ") ";
    }
    bool validateRec(node* a, int lo, int hi, bool &ok) const{
        if(!a) return true;
        if(!(lo < a->data && a->data < hi)) ok=false;
        // bal ∈ {-1,0,1}
        if(a->bal < -1 || a->bal > 1) ok=false;
        bool L = validateRec(a->left,  lo, a->data, ok);
        bool R = validateRec(a->right, a->data, hi, ok);
        return L && R;
    }

public:
    void insert(int x){ bool h = false; insertPriv(x, root, h);}
    avl() = default;
    ~avl(){ clear(); }


    void erase(int x){ bool h=false; eraseRec(x, root, h); }
    bool contains(int x) const{ return findRec(root, x); }

    int mini() const{ auto p = minNode(root); if(!p) throw runtime_error("vacio"); return p->data; }
    int maxi() const{ auto p = maxNode(root); if(!p) throw runtime_error("vacio"); return p->data; }

    int height() const{ return heightRec(root); }
    long long size() const{ return sizeRec(root); }
    bool empty() const{ return root==nullptr; }

    void clear(){ clearRec(root); root=nullptr; }

    void inorder()  const{ inorderRec(root);  cout << "\n"; }
    void preorder() const{ preorderRec(root); cout << "\n"; }
    void postorder()const{ postorderRec(root);cout << "\n"; }

    bool validate() const{
        bool ok=true; validateRec(root, INT_MIN, INT_MAX, ok); return ok;
    }
    

};



int main(){
    avl T;

    auto show = [&](const string& tag){
        cout << "\n[" << tag << "]\n";
        cout << "inorder:  "; T.inorder();
        cout << "preorder: "; T.preorder();
        cout << "postorder:"; T.postorder();
        cout << "height=" << T.height() << "  size=" << T.size()
             << "  valid=" << (T.validate() ? "ok" : "FAIL") << "\n";
        if(!T.empty()){
            cout << "min=" << T.mini() << "  max=" << T.maxi() << "\n";
        }
    };

    // ----- Rotaciones de inserción -----
    // LL: 30,20,10
    T.clear();
    T.insert(30); T.insert(20); T.insert(10);
    show("LL (30,20,10)");

    // LR: 30,10,20
    T.clear();
    T.insert(30); T.insert(10); T.insert(20);
    show("LR (30,10,20)");

    // RR: 10,20,30
    T.clear();
    T.insert(10); T.insert(20); T.insert(30);
    show("RR (10,20,30)");

    // RL: 10,30,20
    T.clear();
    T.insert(10); T.insert(30); T.insert(20);
    show("RL (10,30,20)");

    // ----- Duplicados -----
    T.clear();
    T.insert(10); T.insert(10); T.insert(10);
    show("Duplicados (10 x3)");

    // ----- Prueba completa -----
    T.clear();
    vector<int> v = {30,20,40,10,25,35,50,5,15,45,60};
    for(int x: v) T.insert(x);
    show("Arbol completo inicial");
    cout << boolalpha
         << "contains(25)=" << T.contains(25)
         << "  contains(99)=" << T.contains(99) << "\n";

    // Borrar hoja: 5
    T.erase(5);
    show("erase hoja 5");

    // Borrar hoja: 60
    T.erase(60);
    show("erase hoja 60");

    // Borrar nodo con un hijo: 50 (ahora solo tiene 45)
    T.erase(50);
    show("erase un hijo 50");

    // Borrar nodo con dos hijos: 30 (raíz original)
    T.erase(30);
    show("erase dos hijos 30");

    // Stress pequeño: números 1..20 y borrar pares
    T.clear();
    for(int i=1;i<=20;++i) T.insert(i);
    show("insert 1..20");
    for(int i=2;i<=20;i+=2) T.erase(i);
    show("erase pares 2..20");

    return 0;
}


