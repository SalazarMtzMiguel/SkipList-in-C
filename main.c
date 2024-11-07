#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LEVEL 4
#define P 0.5

typedef struct Node {
    void *row;
    int key;
    int sz;
    struct Node **next;
} Node;

typedef struct SkipList {
    int level;
    Node *header;
} SkipList;

// Crear un nuevo nodo
Node* createNode(int level, int key, void *row) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->row = row;
    node->sz = level;
    node->next = (Node **)malloc((level + 1) * sizeof(Node *));
    for (int i = 0; i <= level; i++) {
        node->next[i] = NULL;
    }
    return node;
}

// Inicializar la lista de salto
SkipList* createSkipList() {
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(MAX_LEVEL, INT_MAX, NULL);
    return list;
}

// Generar un nivel aleatorio
int randomLevel() {
    int level = 0;
    while ((rand() / (double)RAND_MAX) < P && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

// Insertar un nodo en la lista de salto
void insert(SkipList *list, int key, void *row) {
    Node *update[MAX_LEVEL + 1];
    Node *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->next[i] != NULL && x->next[i]->key < key) {
            x = x->next[i];
        }
        update[i] = x;
    }

    x = x->next[0];

    if (x == NULL || x->key != key) {
        int lvl = randomLevel();
        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++) {
                update[i] = list->header;
            }
            list->level = lvl;
        }

        x = createNode(lvl, key, row);
        for (int i = 0; i <= lvl; i++) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
}

// Buscar un nodo en la lista de salto
Node* search(SkipList *list, int key) {
    Node *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->next[i] != NULL && x->next[i]->key < key) {
            x = x->next[i];
        }
    }
    x = x->next[0];
    if (x != NULL && x->key == key) {
        return x;
    }
    return NULL;
}

// Eliminar un nodo de la lista de salto
void delete(SkipList *list, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->next[i] != NULL && x->next[i]->key < key) {
            x = x->next[i];
        }
        update[i] = x;
    }

    x = x->next[0];

    if (x != NULL && x->key == key) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->next[i] != x) break;
            update[i]->next[i] = x->next[i];
        }
        free(x->next);
        free(x);

        while (list->level > 0 && list->header->next[list->level] == NULL) {
            list->level--;
        }
    }
}

// Función de prueba
void testSkipList() {
    SkipList *list = createSkipList();

    int data1 = 10, data2 = 20, data3 = 30;
    insert(list, 1, &data1);
    insert(list, 2, &data2);
    insert(list, 3, &data3);

    Node *node = search(list, 2);
    if (node) {
        printf("Encontrado: %d\n", *(int*)node->row);
    } else {
        printf("No encontrado\n");
    }

    delete(list, 2);
    node = search(list, 2);
    if (node) {
        printf("Encontrado: %d\n", *(int*)node->row);
    } else {
        printf("No encontrado\n");
    }

    free(list);
}

int main() {
    testSkipList();
    return 0;
}
