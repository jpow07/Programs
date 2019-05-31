#include <stdio.h>


#define NUMBER_OF_CARDS 14 * 4
#define HEARTS   0
#define DIAMONDS 1
#define SPADES   2
#define CLUBS    3
#define CARDS "Ace", "One", "Two", "Three", "Four", "Five", "Six",      \
       		"Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"

int main(int argc, char *argv[]) {

	char *deckOfCards[NUMBER_OF_CARDS] = { CARDS, CARDS, CARDS, CARDS };
	
	int numCards = 14;
	int numSuits = 4;
	for (int j = 0; j < numSuits; j++) {
		for (int i = 0; i < numCards; i++) {
			if (j == HEARTS)
			       	printf("%2s of Hearts.\n", deckOfCards[(j * numCards) + i]);
			else if (j == SPADES)
			       	printf("%2s of Spades.\n", deckOfCards[(j * numCards) + i]);
			else if (j == DIAMONDS)
			       	printf("%2s of Diamonds.\n", deckOfCards[(j * numCards) + i]);
			else if (j == CLUBS)
			       	printf("%2s of Clubs.\n", deckOfCards[(j * numCards) + i]);
		}
		printf("\n");
	}	
		

	


	return 0;
}
