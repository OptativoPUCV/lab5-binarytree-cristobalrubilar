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
      } else if (comparacion < 0) {
        if (temp->right == NULL) {
          temp->right = nuevoNodo;
          break;
        }
        temp = temp->right;
      } else {
        if (temp->left == NULL) {
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
  while (x->left != NULL)
    x = x->left;
  return x;
}
void removeNode(TreeMap *tree, TreeNode *node) {
  if (node->left == NULL && node->right == NULL) {
    if (node->parent == NULL) {
      tree->root = NULL;
    } else if (node == node->parent->left) {
      node->parent->left = NULL;
    } else {
      node->parent->right = NULL;
    }
    free(node);
    return;
  }

  TreeNode *hijo = NULL;
  if (node->left != NULL) {
    hijo = minimum(node->left);
  } else {
    hijo = minimum(node->right);
  }

  if (node->parent == NULL) {
    tree->root = hijo;
  } else if (node == node->parent->left) {
    node->parent->left = hijo;
  } else {
    node->parent->right = hijo;
  }

  if (hijo != NULL) {
    hijo->parent = node->parent;
  }
  free(node);
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);

  if (tree->current == node)
    tree->current = NULL;
}

Pair *searchTreeMap(TreeMap *tree, void *key) {

  TreeNode *temp = tree->root;
  while (temp != NULL)
    if (is_equal(tree, temp->pair->key, key)) {
      tree->current = temp;
      return temp->pair;
    } else if (tree->lower_than(key, temp->pair->key)) {
      temp = temp->left;
    } else {
      temp = temp->right;
    }
  return NULL;
}

Pair *upperBound(TreeMap *tree, void *key) {

  TreeNode *temp = tree->root;
  TreeNode *padre = NULL;
  while (temp != NULL) {
    if (tree->lower_than(key, temp->pair->key)) {
      padre = temp;
      temp = temp->left;
    } else if (tree->lower_than(temp->pair->key, key)) {
      padre = temp;
      temp = temp->right;
    } else {
      return temp->pair;
    }
  }

  while (padre != NULL) {
    if (tree->lower_than(padre->pair->key, key)) {
      padre = padre->parent;
    } else {
      break;
    }
  }
  return padre->pair;
}

Pair *firstTreeMap(TreeMap *tree) {
  TreeNode *menorDato = minimum(tree->root);
  return menorDato->pair;
}

Pair *nextTreeMap(TreeMap *tree) { return NULL; }
