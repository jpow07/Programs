#include <stdio.h>

#define NumberOfCards 14 * 4
#define HEARTS   0
#define CLUBS    1
#define DIAMONDS 2
#define SPADES   3
#define CARDS_ABBREV "A", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"

#define CARDS "Ace", "One", "Two", "Three", "Four", "Five", "Six",      \
       		"Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"

void drawCard(int card, int suit) {
	char *cards[NumberOfCards + 4] = { CARDS_ABBREV, CARDS_ABBREV, CARDS_ABBREV, CARDS_ABBREV};
	char *symbol;
	char *number;

	switch(suit) {
		case HEARTS :
			printf("\033[38;2;255;0;0m");
		      	number = cards[card];
			symbol = "\u2665"; //Heart Symbol
			break;

		case SPADES :
			printf("\033[38;2;100;100;100m");
		      	number = cards[card];
			symbol = "\u2660"; //Spade Symbol
			break;
	
		case DIAMONDS :
			printf("\033[38;2;255;0;0m");
		      	number = cards[card];
			symbol = "\u2666"; //Diamond Symbol
			break;
	
		case CLUBS :
			printf("\033[38;2;100;100;100m");
		      	number = cards[card];
			symbol = "\u2663"; //Club Symbol
			break;
	      	default :
			printf("\033[38;2;255;105;0m");
		      	number = "JKR";//cards[NumberOfCards + 3];
			symbol = "";
			break;
      }
	
	
	printf( "\u23A7\u23BA\u23BA\u23BA\u23BA\u23BA\u23AB\n"
		"\u23A5 %2s%s \u23A5\n"
		"\u23A9\u23AF\u23AF\u23AF\u23AF\u23AF\u23AD\n", number, symbol);

}

int main(int argc, char *argv[]) {

	char *deckOfCards[NumberOfCards] = { CARDS, CARDS, CARDS, CARDS };
	
	int numCards = 14;
	int numSuits = 4;

	for (int j = 0; j < numSuits; j++) {
		for (int i = 0; i < numCards; i++) {
			drawCard( (j * numCards) + i, j);
			printf("\n");
		}
	}	
		
	drawCard(0, 5);

	


	return 0;
}
