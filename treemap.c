#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {

  TreeMap *nuevoArbol = (TreeMap *)malloc(sizeof(TreeMap));
  if (nuevoArbol == NULL) {
    return NULL;
  }
  nuevoArbol->root = NULL;
  nuevoArbol->current = NULL;
  nuevoArbol->lower_than = lower_than;
  // new->lower_than = lower_than;
  return nuevoArbol;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  TreeNode *nuevoNodo = createTreeNode(key, value);
  if (tree->root == NULL) {
    tree->root = nuevoNodo;
    tree->current = nuevoNodo;
    return;
  } else {
    TreeNode *temp = tree->root;
    while (1) {
      int comparacion = tree->lower_than(key, temp->pair->key);
      if (comparacion == 0) {
        return;
      }
      else if(comparacion < 0){
        if(temp->right == NULL){
          temp->right = nuevoNodo;
          break;
        }
        temp = temp->right;
      }
      else{
        if(temp->left== NULL){
        temp->left = nuevoNodo;
          break;
        }
        temp = temp->left;
      }
      
      nuevoNodo->parent = temp;
      tree->current = nuevoNodo;
    }
  }
}

TreeNode *minimum(TreeNode *x) { 
while(x->left != NULL)
  x = x->left;
return x;
}
void transplant(TreeMap *tree, TreeNode *oldNode, TreeNode *newNode) {
    if (oldNode->parent == NULL) {
        tree->root = newNode;
    } else if (oldNode == oldNode->parent->left) {
        oldNode->parent->left = newNode;
    } else {
        oldNode->parent->right = newNode;
    }
    if (newNode != NULL) {
        newNode->parent = oldNode->parent;
    }
}

void removeNode(TreeMap *tree, TreeNode *node) {
    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            // Nodo raíz sin hijos
            tree->root = NULL;
        } else if (node == node->parent->left) {
            // Nodo a la izquierda de su padre
            node->parent->left = NULL;
        } else {
            // Nodo a la derecha de su padre
            node->parent->right = NULL;
        }
        free(node);
        return;
    }

    // Caso 2: Nodo con un solo hijo
    if (node->left == NULL) {
        // Nodo solo con hijo derecho
        TreeNode *hijo = node->right;
        transplant(tree, node, hijo);
        free(node);
    } else if (node->right == NULL) {
        // Nodo solo con hijo izquierdo
        TreeNode *hijo = node->left;
        transplant(tree, node, hijo);
        free(node);
    } else {
        // Caso 3: Nodo con dos hijos
        // Encontrar el sucesor
        TreeNode *sucesor = minimum(node->right);
        // Si el sucesor no es hijo directo del nodo
        if (sucesor->parent != node) {
            transplant(tree, sucesor, sucesor->right);
            sucesor->right = node->right;
            sucesor->right->parent = sucesor;
        }
        // Reemplazar el nodo por su sucesor
        transplant(tree, node, sucesor);
        sucesor->left = node->left;
        sucesor->left->parent = sucesor;
        free(node);
    }
}


void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) { return NULL; }

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) { return NULL; }

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
