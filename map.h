/***************************************************************************
 * 
 * Copyright (c) 2016 zkdnfcf, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 /**
 * @file hash.h
 * @author zk(zkdnfc@163.com)
 * @date 2016/05/31 18:26:01
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef _MAP_H
#define _MAP_H

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define mapIterator(node)\
    map_t *node;\
    for (node = map_first(&tree); node; node=map_next(&(node->node)))

 

struct map {
    struct rb_node node;
    char *key;
    void *val;
};

typedef struct map map_t;
typedef struct rb_root root_t;
typedef struct rb_node rb_node_t;

map_t *get(root_t *root, char *str) {
   rb_node_t *node = root->rb_node; 
   while (node) {
        map_t *data = container_of(node, map_t, node);

        //compare between the key with the keys in map
        int cmp = strcmp(str, data->key);
        if (cmp < 0) {
            node = node->rb_left;
        }else if (cmp > 0) {
            node = node->rb_right;
        }else {
            return data;
        }
   }
   return NULL;
}

int set(root_t *root, char* key, void* val) {
    map_t *data = (map_t*)malloc(sizeof(map_t));
    data->key = (char*)malloc((strlen(key)+1)*sizeof(char));
    strcpy(data->key, key);
    data->val = val;
    rb_node_t **new_node = &(root->rb_node), *parent = NULL;
    while (*new_node) {
        map_t *this_node = container_of(*new_node, map_t, node);
        int result = strcmp(key, this_node->key);
        parent = *new_node;

        if (result < 0) {
            new_node = &((*new_node)->rb_left);
        }else if (result > 0) {
            new_node = &((*new_node)->rb_right);
        }else {
            this_node->val = val;
            free(data);
            return 0;
        }
    }

    rb_link_node(&data->node, parent, new_node);
    rb_insert_color(&data->node, root);

    return 1;
}

map_t *map_first(root_t *tree) {
    rb_node_t *node = rb_first(tree);
    return (rb_entry(node, map_t, node));
}

map_t *map_next(rb_node_t *node) {
    rb_node_t *next =  rb_next(node);
    return rb_entry(next, map_t, node);
}

void map_free(map_t *node,bool freeVal){
    if (node != NULL) {
        if (node->key != NULL) {
            free(node->key);
            node->key = NULL;
            if(freeVal){
                free(node->val);
                node->val = NULL;
            }
        }
        free(node);
        node = NULL;
    }
}
void map_clear(root_t *tree,bool freeVal){
    map_t *nodeFree = NULL;
    for (nodeFree = map_first(tree); nodeFree; nodeFree = map_first(tree)) {
        if (nodeFree) {
            rb_erase(&(nodeFree->node), tree);
            map_free(nodeFree,freeVal);
        }
    }
}

#endif  //_MAP_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
