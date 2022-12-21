#include <stdio.h>
#include <stdlib.h>

typedef enum {RED, BLACK} color_t;

typedef struct TreeNode {
    int val;
    color_t color;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    int height;
}Tree_Node_t;

Tree_Node_t *newNode(int key) {
    Tree_Node_t *node = (Tree_Node_t *) malloc(sizeof(Tree_Node_t));
    node->val = key;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->color = RED;
    return node;
}

Tree_Node_t *Search(Tree_Node_t *root, int key) {
    if (root == NULL) {
        return NULL;
    }
    if (root->val == key) {
        return root;
    }
    else if (key < root->val) {
        return Search(root->left, key);
    }
    else if(key > root->val){
        return Search(root->right, key);
    }
    else{
        return NULL;
    }

}

/*AVL*/

int Max(int a, int b) {
  return (a > b) ? a : b;
}

int height(Tree_Node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return node->height;
}

Tree_Node_t *rightRotate(Tree_Node_t *y) {
  Tree_Node_t *x = y->left;
  Tree_Node_t *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = Max(height(y->left), height(y->right)) + 1;
  x->height = Max(height(x->left), height(x->right)) + 1;

  return x;
}

Tree_Node_t *leftRotate(Tree_Node_t *x) {
  Tree_Node_t*y = x->right;
  Tree_Node_t *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = Max(height(x->left), height(x->right)) + 1;
  y->height = Max(height(y->left), height(y->right)) + 1;

  return y;
}

int getBalance(Tree_Node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return height(node->left) - height(node->right);
}

Tree_Node_t *AVL_insert(Tree_Node_t *node, int key) {
  if (node == NULL) {
    return newNode(key);
  }
  if (key < node->val) {
    node->left = AVL_insert(node->left, key);
  } else if (key > node->val) {
    node->right = AVL_insert(node->right, key);
  } else {
    return node;
  }

  node->height = 1 + Max(height(node->left), height(node->right));

  int balance = getBalance(node);

  // left left case
  if (balance > 1 && key < node->left->val) {
    return rightRotate(node);
  }

  // right right case
  if (balance < -1 && key > node->right->val) {
    return leftRotate(node);
  }

  // left right case
  if (balance > 1 && key > node->left->val) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }

  // right left case
  if (balance < -1 && key < node->right->val) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

Tree_Node_t *AVL_delete(Tree_Node_t *root, int key) {
  if (root == NULL) {
    return root;
  }
  if (key < root->val) {
    root->left = AVL_delete(root->left, key);
  } else if (key > root->val) {
    root->right = AVL_delete(root->right, key);
  } else {
    // node with only one child or no child
    if (root->left == NULL || root->right == NULL) {
      Tree_Node_t *temp = root->left ? root->left : root->right;
      if (temp == NULL) {
        temp = root;
        root = NULL;
      } else {
        *root = *temp;
      }
      free(temp);
    } else {
      // node with two children: get the inorder successor
      // (smallest in the right subtree)
      Tree_Node_t  *temp = root->right;
      while (temp->left != NULL) {
        temp = temp->left;
      }
      root->val = temp->val;
      root->right = AVL_delete(root->right, temp->val);
    }
  }

  if (root == NULL) {
    return root;
  }

  root->height = 1 + Max(height(root->left), height(root->right));

  int balance = getBalance(root);

  // left left case
  if (balance > 1 && getBalance(root->left) >= 0) {
    return rightRotate(root);
  }

  // left right case
  if (balance > 1 && getBalance(root->left) < 0) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  // right right case
  if (balance < -1 && getBalance(root->right) <= 0) {
    return leftRotate(root);
  }

  // right left case
  if (balance < -1 && getBalance(root->right) > 0) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

void preOrder(Tree_Node_t *root) {
  if (root != NULL) {
    printf("%d ", root->val);
    preOrder(root->left);
    preOrder(root->right);
  }
}


/*Red-Black Tree*/

Tree_Node_t *minimum(Tree_Node_t *node) {
  while (node->left != NULL) {
      node = node->left;
  }
  return node;
}

Tree_Node_t *successor(Tree_Node_t *node) {
    if (node->right != NULL) {
        return minimum(node->right);
    }
    Tree_Node_t *y = node->parent;
    while (y != NULL && node == y->right) {
        node = y;
        y = y->parent;
    }
    return y;
}

void rotate_left(Tree_Node_t **root, Tree_Node_t *node) {
    Tree_Node_t *right = node->right;
    node->right = right->left;
    if (right->left != NULL) {
        right->left->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == NULL) {
        *root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
}

void rotate_right(Tree_Node_t **root, Tree_Node_t *node) {
    Tree_Node_t *left = node->left;
    node->left = left->right;
    if (left->right != NULL) {
        left->right->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == NULL) {
        *root = left;
    } else if (node == node->parent->left) {
        node->parent->left = left;
    } else {
        node->parent->right = left;
    }
    left->right = node;
    node->parent = left;
}

void delete_fixup(Tree_Node_t **root, Tree_Node_t *node) {
    while (node != *root && node->color == BLACK) {
        if (node == node->parent->left) {
            Tree_Node_t *sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotate_left(root, node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } 
            else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotate_right(root, sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotate_left(root, node->parent);
                node = *root;
            }
        } 
        else {
            Tree_Node_t *sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotate_right(root, node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } 
            else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotate_left(root, sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotate_right(root, node->parent);
                node = *root;
            }
        }
    }
    node->color = BLACK;
}

void RB_insert(Tree_Node_t **root, int value) {
    Tree_Node_t *node = newNode(value);
    Tree_Node_t *x = *root;
    Tree_Node_t *y = NULL;
    
    /*insert node*/
    while (x != NULL) {
        y = x;
        if (node->val < x->val) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == NULL) {
        *root = node;
    } else if (node->val < y->val) {
        y->left = node;
    } else {
        y->right = node;
    }

    /*If parent node is RED*/
    while (node != *root && node->parent->color == RED) {
        /*know uncle position*/
        if (node->parent == node->parent->parent->left) {
            Tree_Node_t *uncle = node->parent->parent->right;
            /*If uncle is RED*/
            if (uncle != NULL && uncle->color == RED) {
                /*make uncle is BLACK*/
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } 
            else {
                /*If right node is RED*/
                if (node == node->parent->right) {
                    node = node->parent;
                    rotate_left(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_right(root, node->parent->parent);
            }
        } 
        else {
            Tree_Node_t *uncle = node->parent->parent->left;
            /*If uncle is RED*/
            if (uncle != NULL && uncle->color == RED) {
                /*make uncle is BLACK*/
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } 
            else {
                /*If left node is RED*/
                if (node == node->parent->left) {
                    node = node->parent;
                    rotate_right(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotate_left(root, node->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void RB_delete(Tree_Node_t **root, int value) {
    Tree_Node_t *node = Search(*root, value);
    if (node == NULL) {
        return;
    }

    Tree_Node_t *x = NULL;
    Tree_Node_t *y = NULL;

    if (node->left == NULL || node->right == NULL) {
        y = node;
    } else {
        y = successor(node);
    }

    if (y->left != NULL) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != NULL) {
        x->parent = y->parent;
    }

    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    if (y != node) {
        node->val = y->val;
    }

    if (y->color == BLACK) {
        delete_fixup(root, x);
    }

    free(y);
}

int main() {
    char method[10];
    char MakeTreeWay[10];
    int data;
    Tree_Node_t *root = NULL;
    Tree_Node_t *find = NULL;

    scanf("%s", MakeTreeWay);
    switch (MakeTreeWay[0])
    {
    case 'A':
      while (1){
        scanf("%s", method);
        switch (method[0])
        {
        case 'i':
            scanf("%d", &data);
            root = AVL_insert(root, data);
            break;
        case 's':
            scanf("%d", &data);
            find = Search(root, data);
            if(find != NULL){
                printf("%d\n", getBalance(find));
            }
            else{
                printf("Not found\n");
            }
            break;
        case 'd':
            scanf("%d", &data);
            root = AVL_delete(root, data);
            break;
        case 'q':
            //printf("Preorder traversal of the constructed AVL tree is \n");
            //preOrder(root);
            return 0;
        }
      }
      break;    
    case 'r':
      while (1){
        scanf("%s", method);
        switch (method[0])
        {
        case 'i':
            scanf("%d", &data);
            RB_insert(&root, data);
            break;
        case 's':            
            scanf("%d", &data);
            find = Search(root, data);
            if(find != NULL){
                if(find->color == BLACK){
                    printf("black\n");
                }
                else{
                    printf("red\n");
                }
            }
            else{
                printf("Not found\n");
            }
            break;
        case 'd':
            scanf("%d", &data);
            RB_delete(&root, data);
            break;
        case 'q':
            //printf("Preorder traversal of the constructed AVL tree is \n");
            //preOrder(root);
            return 0;
        }
      }
      break;
    }
    
}


