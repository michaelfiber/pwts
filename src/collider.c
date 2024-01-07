#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "collider.h"

typedef struct Node
{
    Location *loc;
    int type;
    struct Node *next;
} Node;

Node *root = NULL;

void add_collider(Location *loc, int type)
{
    Node *new_node = MemAlloc(sizeof(Node));
    new_node->loc = loc;
    new_node->type = type;
    new_node->next = NULL;

    if (root == NULL)
    {
        root = new_node;
        return;
    }

    Node *target = root;
    while (target != NULL)
    {
        if (target->next == NULL)
        {
            target->next = new_node;
            return;
        }
        target = target->next;
    }
}

void remove_collider(Location *loc)
{
    Node *current = root;
    Node *prev = NULL;

    while (current != NULL)
    {
        if (current->loc == loc)
        {
            if (prev == NULL)
            {
                root = current->next;
            }
            else 
            {
                prev->next = current->next;
            }

            MemFree(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

void draw_colliders()
{
    Node *current = root;
    int count = 0;
    while (current != NULL)
    {
        Color c = PURPLE;
        if (current->loc->is_hitting_type > -1)
        {
            c = RED;
        }
        DrawRectangleLinesEx(current->loc->dest, 1.0f, c);
        current = current->next;
        count++;
    }
}

void check_colliders()
{
    // reset collisions for this tick. Ugly? So ugly.
    Node *current = root;
    while (current != NULL)
    {
        current->loc->is_hitting_type = -1;
        current = current->next;
    }
    current = root;

    while (current != NULL)
    {
        Node *secondary = root;
        while (secondary != NULL)
        {
            if (current->type != secondary->type) 
            {
                if (CheckCollisionRecs(current->loc->dest, secondary->loc->dest)) {
                    current->loc->is_hitting_type = secondary->type;
                    secondary->loc->is_hitting_type = current->type;
                }
            }
            
            secondary = secondary->next;
        }
        current = current->next;
    }
}