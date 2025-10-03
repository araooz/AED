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
public:
    void insert(int x){
        bool h = false; //si crecio la altura
        insertPriv(x, root, h);
    }
    avl() = default;
    

    

};




