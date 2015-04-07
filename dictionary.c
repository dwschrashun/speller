/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 *
 * Cleaned up version of dictionary-2.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

int dicwords = 0;
struct dicnode root;

bool freeChildren (struct dicnode* crawler, struct dicnode* previous, int lastc, int depth)
{
    struct dicnode* parent = crawler;
    struct dicnode* freed;
    crawler = NULL;
    int i = 0;
    for (i = 0; i < 27 && crawler == NULL; i++)
    {       
       crawler = parent->children[i];
    }
    if (crawler != NULL)
    {
        ++depth;
        freeChildren(crawler, parent, i-1, depth);
    }        
    else if (depth > 1)
    {
        free(parent);
        freed = parent;
        previous->children[lastc] = NULL;
        dicwords--;
        return false;   
    }
    else
    {
        previous->children[lastc] = NULL;
        return true;
    }
    return false;
}


struct dicnode* createNode (int i, struct dicnode* parent)
{
    struct dicnode* newNode = malloc(sizeof(struct dicnode));
    for (i = 0; i < 27; i++)
    {
        newNode->children[i] = NULL;
    }
    newNode->a = false;
    return newNode;
}


int convertChar (char a)
{
    int x;
    a = tolower(a);
    if (a == '\'')
    {
        x = 26;
    }
    else
    {
        x = a - 'a';
    }
    return x;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    struct dicnode* crawler;
    int wordlen = strlen(word);
    char c = '1';
    int index;
    crawler = &root;
    
    for (int k = 0; k < wordlen; k++)
    {
        c = word[k];
        c = tolower(c);
        index = convertChar(c);
        crawler = crawler->children[index];
        if (crawler == NULL)
        {
            return false;
        }   
    }
    if (crawler->a == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}



/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    char charbuf = 'a';
    struct dicnode* crawler = &root;
    int index = charbuf - '\'';
    int end = 1;

    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Failed to load dictionary");
        return false;
    }
    else
    {   
        //initialize null pointers for root
        for (int i = 0; i < 27; i++)
        {
            root.children[i] = NULL;
        }

        while ((fread(&charbuf, sizeof(char), 1, dict)) == 1)
        {          
            index = convertChar(charbuf);
            struct dicnode* parentNode;

            if (root.children[index] == NULL && charbuf != '\n')
            {
                
                parentNode = &root;
                while (charbuf != '\n')
                {                
                    parentNode->children[index] = createNode(index, parentNode);
                    parentNode = parentNode->children[index];
                    end = fread(&charbuf, sizeof(char), 1, dict);
                    if (end != 1)
                    {
                        break;
                    }
                    index = convertChar(charbuf);          
                }
                if (charbuf == '\n')
                {
                    parentNode->a = true;
                    dicwords++;
                }
            }
            else if (root.children[index] != NULL && charbuf != '\n')
            {
                crawler = &root;
                while (crawler != NULL && charbuf != '\n')
                {               
                    parentNode = crawler;
                    crawler = crawler->children[index];
                    if (crawler == NULL)
                    {
                        break;
                    }
                    end = fread(&charbuf,sizeof(char), 1, dict);
                    if (end != 1)
                    {
                        break;
                    }
                    index = convertChar(charbuf);
                }
                if (crawler == NULL && charbuf == '\n')
                {
                    parentNode->children[index] = createNode(index, parentNode);
                    parentNode = parentNode->children[index];
                    parentNode->a = true;
                    ++dicwords;
                }            
                else if (crawler == NULL && charbuf != '\n')
                {
                    while (charbuf != '\n')
                    {
                        parentNode->children[index] = createNode(index, parentNode);
                        parentNode = parentNode->children[index];
                        end = fread(&charbuf, sizeof(char), 1, dict);
                        if (end != 1)
                        {
                            break;
                        }
                        index = convertChar(charbuf);
                    }
                    if (charbuf == '\n')
                    {
                        parentNode->a = true;
                        dicwords++;       
                    }
                } 
                else if (crawler != NULL && charbuf == '\n')
                {
                    if (crawler->a != true)
                    {
                        crawler->a = true;
                        dicwords++;
                    }
                }
            }           
        }
        fclose(dict);
        return true;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return dicwords;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // build ("unload node") function, crawl to end of trie, unload node, go up a level, pan over to each child, unload node beneath, once children are clear, go up uload again, until you hit the root
    // down to null, up, over, repeat until
    
            
    struct dicnode* crawlerer;
    bool rootChildCleared = false;
    bool cleared = true;
    
    for (int r = 0; r < 27; r++)
    {
        while (root.children[r] != NULL)
        {
            crawlerer = root.children[r];
            rootChildCleared = freeChildren (crawlerer, &root, r, 1);
        }
    }
    for (int y = 0; y < 27; y++)
    {
        if (root.children[y] != NULL)
        {
            cleared = false; 
        }
    }
    if (cleared == false)
    {
        return false;
    }
    else
    {
       return true;
    }
}


