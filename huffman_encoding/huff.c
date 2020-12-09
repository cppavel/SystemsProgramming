// code for a huffman coder
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "huff.h"

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
    struct huffcoder* ans = malloc(sizeof(struct huffcoder));
    ans->tree = malloc(sizeof(struct huffchar));
    return ans;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
    FILE* file;
    file = fopen(filename,"r");
    char c = fgetc(file);
    while(!feof(file))
    {
        int index = (int)c;
        if(index<0)
        {
            index += NUM_CHARS;
        }
        this->freqs[index]++;
        c =fgetc(file);
    }
    int i;
    for(i = 0; i < NUM_CHARS; i++)
    {
        if(this->freqs[i]== 0)
        {
            this->freqs[i]++;
        }
    }
    fclose(file);
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes

int find_lowest(struct huffchar** chars, int size)
{
    int min = chars[0]->freq;
    int min_index = 0;
    int seqno = chars[0]->seqno;
    for(int i = 1; i < size ;i++)
    {
        if(min>chars[i]->freq)
        {
            min_index = i;
            min = chars[i]->freq;
            seqno = chars[i]->seqno;
        }
        else if(min == chars[i]->freq)
        {
            if(seqno>chars[i]->seqno)
            {
                min = chars[i]->freq;
                min_index = i;
                seqno = chars[i]->seqno;
            }
        }
    }

    return min_index;
}
void huffcoder_build_tree(struct huffcoder * this)
{
    struct huffchar** chars = malloc(sizeof(struct huffchar*)*NUM_CHARS);
    int i =0;
    for(i = 0; i < NUM_CHARS; i++)
    {
        chars[i] = malloc(sizeof(struct huffchar));
        chars[i]->seqno = i;
        chars[i]->freq = this->freqs[i];
        chars[i]->is_compound = 0;
        chars[i]->u.c = (char)i;
    }

    int size = NUM_CHARS;
    int sequenceNumber = 256;

    while(size>1)
    {
        int min1 = find_lowest(chars,size);
        struct huffchar* first =  chars[min1];
        chars[min1] = chars[size-1];
        size--;
        int min2 = find_lowest(chars,size);
        struct huffchar* second = chars[min2];
        chars[min2] = chars[size-1];
        size--;
        struct huffchar* compound = malloc(sizeof(struct huffchar));
        compound->freq = first->freq + second->freq;
        compound->is_compound = 1;
        compound->seqno = sequenceNumber;
        sequenceNumber++;
        compound->u.compound.left = first;
        compound->u.compound.right = second;
        chars[size] = compound;
        size++;
    }
    this->tree = chars[0];
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object

void huffcoder_tree2table_helper(struct huffcoder* this, struct huffchar* current, char* path, int length)
{
    if(!current->is_compound )
    {
        path[length] = '\0';
        int index = (int)current->u.c;
        if(index<0)
        {
            index += NUM_CHARS;
        }
        this->code_lengths[index] = length;
        this->codes[index] = malloc(sizeof(char)*NUM_CHARS);
        strncpy(this->codes[index],path,length);
    }
    else
    {
        path[length] = '0';
        huffcoder_tree2table_helper(this,current->u.compound.left,path, length + 1);
        path[length] = '1';
        huffcoder_tree2table_helper(this,current->u.compound.right,path, length + 1);
    }

}

void huffcoder_tree2table(struct huffcoder * this)
{
    char* path = malloc(sizeof(char)*NUM_CHARS);
    huffcoder_tree2table_helper(this,this->tree,path,0);
}

// print the Huffman codes for each character in order;
// you should not modify this function
void huffcoder_print_codes(struct huffcoder * this)
{
    for ( int i = 0; i < NUM_CHARS; i++ ) {
        // print the code
        printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], this->codes[i]);;
    }
}

// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
                      char * output_filename)
{
    FILE* file;
    file = fopen(input_filename,"r");
    FILE* output;
    output = fopen(output_filename,"w");
    char c;
    char* code;
    c = fgetc(file);
    while(!feof(file))
    {
        int index = (int)c;
        if(index<0)
        {
            index += NUM_CHARS;
        }
        code = this->codes[index];
        fputs(code,output);
        c=fgetc(file);
    }

    c = (int)4; //EOT
    fputs(this->codes[4],output);

    fclose(file);
    fclose(output);
}

int find_code(struct huffcoder * this, char* code)
{
    for(int i =0 ; i < NUM_CHARS; i++)
    {
        if(strcmp(this->codes[i],code) == 0)
        {
            return i;
        }
    }
    return  -1;

}

// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
                      char * output_filename)
{
    char* code = malloc(sizeof(char)*NUM_CHARS);
    int length = 0;
    FILE* file;
    file = fopen(input_filename,"r");
    FILE* output;
    output = fopen(output_filename,"w");
    char c;
    c =fgetc(file);
    while(!feof(file))
    {
        code[length] = c;
        length++;
        code[length] = '\0';
        int found = find_code(this,code);
        if(found != -1 && found !=4)
        {
            fputc((char)found,output);
            length = 0;
            code[length] ='\0';
        }
        c =fgetc(file);
    }
    fclose(file);
    fclose(output);

}
