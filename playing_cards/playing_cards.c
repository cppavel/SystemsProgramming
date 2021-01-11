//   playing_cards.c
//   David Gregg
//   December 2020

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "playing_cards.h"



/* pack the playing card structures into bytes of memory */
unsigned char * pack_playing_cards(struct playing_card * cards,
				   int number_of_cards) {

    /*in this block of code the size of output array is computed*/
    int number_of_bits_required = number_of_cards * 6;
    int number_of_bytes_required = (number_of_bits_required)/8;

    if(number_of_bits_required % 8 !=0)
    {
        number_of_bytes_required = number_of_bits_required + 1;
    }

    /*creating and initializing the array with 0s, in case something else was stored at this memory location previously*/
    unsigned  char * packed_cards = malloc(sizeof(unsigned  char) * number_of_bytes_required);

    /* a simple index variable. note it is reused in the main for loop of the function*/
    int index;

    for(index = 0; index < number_of_bytes_required; index++)
    {
        packed_cards[index] = 0;
    }

    /*
     *  next_vacant_position describes the next available position in the array of bytes, numbered as if we just worked
     *  with an array of bits, so it could have a value of say 18.
     *
     *  suit - just a variable which has the suit value taken from the structure
     *
     *  value - just a variable which has the card value taken from the structure
     *
     *  combined - a variable which is the bit-wise concatenation of suit and value.
     *
     *  mask - a simple mask used to get particular bits from combined variable
     *
     *  current_bit - value of the current bit taken from combined and shifted appropriately to be stored in the packed_cards
     *  array
     *
     *  array_vacant_position - a position of array element which has the next vacant bit
     *
     *  bit_vacant_position - a position of the vacant bit in a particular element of the packed_cards array. Numbered
     *  from left to right so bit_vacant_position = 0, means a bit which corresponds to a binary value of 2^7 and
     *  bit_vacant_position = 7 means a bit which corresponds to a binary value of 2^0
     *
     */

    int next_vacant_position = 0;

    int suit, value, combined, mask, current_bit, array_vacant_position, bit_vacant_position;

    for(index = 0; index < number_of_cards; index++)
    {
        suit = cards[index].suit;
        value = cards[index].value;

        combined = (suit<<4) | value;
        mask = 1 << 5;

        while(mask > 0)
        {
            array_vacant_position = next_vacant_position/8;
            bit_vacant_position = next_vacant_position % 8;

            current_bit = ((combined & mask )!= 0) << ((7 - bit_vacant_position));

            packed_cards[array_vacant_position] = packed_cards[array_vacant_position] | current_bit;

            next_vacant_position = next_vacant_position + 1;
            mask = mask / 2;
        }
    }
    return packed_cards;
}




/* unpack bytes of memory containing card data into playing card structures */
struct playing_card * unpack_playing_cards(unsigned char * packed_cards,
					   int number_of_cards) {

    /*
     *  current_position - a variable which describes the position of a bit we are currently dealing with as if we worked
     *  just with an array of bits
     *
     *  maximum_position - the bit position we need to examine, as if we worked with just an array of bits. Depends on the
     *  number of cards supplied.
     *
     *  array_position - index of the element in packed_cards array where the current bit is located
     *
     *  bit_position - the position of the current bit in the element of the packed_cards array it is contained in.
     *  Numbered from left to right so bit_position = 0, means a bit which corresponds to a binary value of 2^7 and
     *  bit_position = 7 means a bit which corresponds to a binary value of 2^0
     *
     *  current_unpacked_value - the current value which is the result of unpacking, could be suit or value of the card
     *  depending on what is currently being unpacked
     *
     *  current_bit - the value of the bit which was unpacked. it is not shifted anyhow, because shifting is handled by
     *  shifting the current_unpacked_value right by 1 each time we unpack a bit.
     *
     *  counter - a simple variable which takes account of what is being unpacked now (suit or value) and when it should
     *  be stored in the output array of playing_card structures
     *
     *  current_card - an index in the array of playing_card structs. It is used as current_card/2 because we have two
     *  values in each structure to store. Whenever we store suit or value we increment current_card. So when we store suit
     *  and then value, current_card becomes greater by 2 and we proceed to the next element in the playing_card struct
     *  array
     */

    int current_position = 0;
    int maximum_position = 6 * number_of_cards - 1;
    int array_position, bit_position;
    int current_unpacked_value = 0;
    int current_bit = 0;
    int counter = 0;
    int current_card = 0;

    /* allocating memory for the unpacked array based on the number of cards supplied*/
    struct playing_card * unpacked = malloc(sizeof(struct playing_card)*number_of_cards);


    while(current_position <= maximum_position)
    {

        array_position = current_position / 8;
        bit_position = current_position % 8;

        current_bit = ((packed_cards[array_position] & (1 << (7-bit_position))) != 0);
        current_unpacked_value = current_unpacked_value | current_bit;

        if(counter == 1)                //finished unpacking suit
        {
            unpacked[current_card/2].suit = current_unpacked_value;
            current_unpacked_value = 0;
            current_card++;
        }
        else if(counter == 5)           //finished unpacking value
        {
            unpacked[current_card / 2].value = current_unpacked_value;
            current_unpacked_value = 0;
            current_card++;
        }
        else                            //shift so that other bits can be correctly stored
        {
            current_unpacked_value = current_unpacked_value << 1;
        }

        current_position = current_position + 1;
        counter = (counter + 1) % 6;
    }

    return unpacked;
}


