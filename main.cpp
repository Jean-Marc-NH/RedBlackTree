#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* pad;
    Node* l;
    Node* r;
    int color;
};


class RedBlackTree {
private:
    Node* root;
    Node* nil;

    Node* find(Node* node, int val) {
        if (node == nil || val == node->data) {
            return node;
        }

        if (val < node->data) {
            return find(node->l, val);
        }
        return find(node->r, val);
    }

    void deleteFix(Node* x) {
        Node* s;
        while (x != root && x->color == 0) {
            if (x == x->pad->l) {
                s = x->pad->r;
                if (s->color == 1) {
                    s->color = 0;
                    x->pad->color = 1;
                    lRotate(x->pad);
                    s = x->pad->r;
                }

                if (s->l->color == 0 && s->r->color == 0) {
                    s->color = 1;
                    x = x->pad;
                }
                else {
                    if (s->r->color == 0) {
                        s->l->color = 0;
                        s->color = 1;
                        rRotate(s);
                        s = x->pad->r;
                    }

                    s->color = x->pad->color;
                    x->pad->color = 0;
                    s->r->color = 0;
                    lRotate(x->pad);
                    x = root;
                }
            }
            else {
                s = x->pad->l;
                if (s->color == 1) {
                    s->color = 0;
                    x->pad->color = 1;
                    rRotate(x->pad);
                    s = x->pad->l;
                }

                if (s->r->color == 0) {
                    s->color = 1;
                    x = x->pad;
                }
                else {
                    if (s->l->color == 0) {
                        s->r->color = 0;
                        s->color = 1;
                        lRotate(s);
                        s = x->pad->l;
                    }

                    s->color = x->pad->color;
                    x->pad->color = 0;
                    s->l->color = 0;
                    rRotate(x->pad);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->pad == nullptr) {
            root = v;
        }
        else if (u == u->pad->l) {
            u->pad->l = v;
        }
        else {
            u->pad->r = v;
        }
        v->pad = u->pad;
    }

    void deleteNode(Node* node, int val) {
        Node* z = nil;
        Node* x, * y;
        while (node != nil) {
            if (node->data == val) {
                z = node;
            }

            if (node->data <= val) {
                node = node->r;
            }
            else {
                node = node->l;
            }
        }

        if (z == nil) {
            cout << "no se encontro" << endl;
            return;
        }

        y = z;
        int y_original_color = y->color;
        if (z->l == nil) {
            x = z->r;
            rbTransplant(z, z->r);
        }
        else if (z->r == nil) {
            x = z->l;
            rbTransplant(z, z->l);
        }
        else {
            y = minimum(z->r);
            y_original_color = y->color;
            x = y->r;
            if (y->pad == z) {
                x->pad = y;
            }
            else {
                rbTransplant(y, y->r);
                y->r = z->r;
                y->r->pad = y;
            }

            rbTransplant(z, y);
            y->l = z->l;
            y->l->pad = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0) {
            deleteFix(x);
        }
    }

    void insertFix(Node* nodo) {
        Node* u;
        while (nodo->pad->color == 1) {
            if (nodo->pad == nodo->pad->pad->r) {
                u = nodo->pad->pad->l;
                if (u->color == 1) {
                    u->color = 0;
                    nodo->pad->color = 0;
                    nodo->pad->pad->color = 1;
                    nodo = nodo->pad->pad;
                }
                else {
                    if (nodo == nodo->pad->l) {
                        nodo = nodo->pad;
                        rRotate(nodo);
                    }
                    nodo->pad->color = 0;
                    nodo->pad->pad->color = 1;
                    lRotate(nodo->pad->pad);
                }
            }
            else {
                u = nodo->pad->pad->r;

                if (u->color == 1) {
                    u->color = 0;
                    nodo->pad->color = 0;
                    nodo->pad->pad->color = 1;
                    nodo = nodo->pad->pad;
                }
                else {
                    if (nodo == nodo->pad->r) {
                        nodo = nodo->pad;
                        lRotate(nodo);
                    }
                    nodo->pad->color = 0;
                    nodo->pad->pad->color = 1;
                    rRotate(nodo->pad->pad);
                }
            }
            if (nodo == root) {
                break;
            }
        }
        root->color = 0;
    }

    void print(Node* root, bool last) {
        if (root != nil) {
            cout << root->data << " ";
            print(root->l, false);
            print(root->r, true);
        }
    }

public:
    RedBlackTree() {
        nil = new Node;
        nil->color = 0;
        nil->l = nullptr;
        nil->r = nullptr;
        root = nil;
    }

    Node* find(int k) {
        return find(root, k);
    }

    Node* minimum(Node* node) {
        while (node->l != nil) {
            node = node->l;
        }
        return node;
    }

    Node* maximum(Node* node) {
        while (node->r != nil) {
            node = node->r;
        }
        return node;
    }

    Node* successor(Node* x) {
        if (x->r != nil) {
            return minimum(x->r);
        }

        Node* y = x->pad;
        while (y != nil && x == y->r) {
            x = y;
            y = y->pad;
        }
        return y;
    }

    Node* predecessor(Node* x) {
        if (x->l != nil) {
            return maximum(x->l);
        }

        Node* y = x->pad;
        while (y != nil && x == y->l) {
            x = y;
            y = y->pad;
        }

        return y;
    }

    void lRotate(Node* x) {
        Node* y = x->r;
        x->r = y->l;
        if (y->l != nil) {
            y->l->pad = x;
        }
        y->pad = x->pad;
        if (x->pad == nullptr) {
            root = y;
        }
        else if (x == x->pad->l) {
            x->pad->l = y;
        }
        else {
            x->pad->r = y;
        }
        y->l = x;
        x->pad = y;
    }

    void rRotate(Node* x) {
        Node* y = x->l;
        x->l = y->r;
        if (y->r != nil) {
            y->r->pad = x;
        }
        y->pad = x->pad;
        if (x->pad == nullptr) {
            root = y;
        }
        else if (x == x->pad->r) {
            x->pad->r = y;
        }
        else {
            x->pad->l = y;
        }
        y->r = x;
        x->pad = y;
    }

    void insert(int val) {
        Node* node = new Node;
        node->pad = nullptr;
        node->data = val;
        node->l = nil;
        node->r = nil;
        node->color = 1;

        Node* y = nullptr;
        Node* x = root;

        while (x != nil) {
            y = x;
            if (node->data < x->data) {
                x = x->l;
            }
            else {
                x = x->r;
            }
        }

        node->pad = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->data < y->data) {
            y->l = node;
        }
        else {
            y->r = node;
        }

        if (node->pad == nullptr) {
            node->color = 0;
            return;
        }

        if (node->pad->pad == nullptr) {
            return;
        }

        insertFix(node);
    }

    void deleteNode(int data) {
        deleteNode(root, data);
    }

    void printTree() {
        if (root) {
            print(root, true);
        }
    }
};

int main() {
    RedBlackTree bst;
    bst.insert(55);
    bst.insert(40);
    bst.insert(65);
    bst.insert(60);
    bst.insert(75);
    bst.insert(59);

    bst.printTree();
    cout << endl;
    bst.deleteNode(40);
    bst.printTree();
}
