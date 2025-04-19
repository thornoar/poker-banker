#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_with_padding (char str[], int size)
{
    printf("%s", str);
    int len = size - strlen(str);
    for (int i = 0; i < len; i++) {
        printf(" ");
    }
}

int main ()
{
    int number_of_players;
    printf("| Enter the number of players:\n> \e[34m"); //]
    if (scanf("%d", &number_of_players) == EOF)
        exit(0);
    printf("\e[0m"); //]

    char players[number_of_players][16];
    printf("| Enter the names of the players:\n");
    for (int i = 0; i < number_of_players; i++) {
        printf("> "); scanf("%s", players[i]);
    }

    int base_budget;
    printf("| Enter the initial budget of all players:\n> \e[34m"); //]
    if (scanf("%d", &base_budget) == EOF)
        exit(0);
    printf("\e[0m"); //]
    int budgets[number_of_players];

    int base_bet;
    printf("| Enter the initial bet of all players:\n> \e[34m"); //]
    if (scanf("%d", &base_bet) == EOF)
        exit(0);
    printf("\e[0m"); //]
    int bets[number_of_players];

    int bank = number_of_players * base_bet;
    int maxbet = base_bet;

    for (int i = 0; i < number_of_players; i++) {
        budgets[i] = base_budget - base_bet;
        bets[i] = base_bet;
    }

    int folded[number_of_players];
    for (int i = 0; i < number_of_players; i++) {
        folded[i] = 0;
    }

    int cp = 0;
    int made_bet = 0;
    system("clear");

    while (1) {
        // Printing current table
        system("clear");
        printf("| Current table configuration:\n");
        printf("+");
        for (int i = 0; i < 48; i++) { printf("-"); }
        printf("+");
        printf("\n");
        char base_color[] = "\e[34m"; // ]
        char current_color[] = "\e[35m"; // ]
        for (int i = 0; i < number_of_players; i++) {
            printf(
                (i == cp) ? "| > " : "|   "
            );
            printf(
                "no.%1$s%2$d\e[0m | %1$s",
                (i == cp) ? current_color : base_color,
                i
            ); // ]
            print_with_padding(players[i], 8);
            printf("\e[0m | "); // ]
            printf(
                "budget: %s%-5d\e[0m | ",
                (i == cp) ? current_color : base_color,
                budgets[i]
            ); // ]
            printf(
                "bet: %s%-5d\e[0m |",
                (i == cp) ? current_color : base_color,
                bets[i]
            ); // ]
            if (folded[i] == 1) {
                printf(" (\e[31mfolded\e[0m)"); // ]]
            }
            printf("\n");
        }
        printf("+");
        for (int i = 0; i < 48; i++) { printf("-"); }
        printf("+");
        printf("\n");

        printf("| Current bet: \e[34m%d\e[0m. Bank value: \e[34m%d\e[0m.\n", maxbet, bank);

        for (int i = 0; i < number_of_players; i++) {
            if (folded[i] == 0) goto round;
        }
        goto end;
round:
        if (folded[cp]) { cp = (cp+1)%number_of_players; continue; }

        printf("| Enter the action of the current player:\n");
        printf("> \e[34m"); // ]

        char action[16];
        if (scanf("%s", action) == EOF)
            exit(0);
        printf("\e[0m"); // ]
        if (strcmp(action, "fold") == 0) { goto fold; }
        if (strcmp(action, "bet") == 0) { goto bet; }
        if (strcmp(action, "raise") == 0) { goto raise; }
        if (strcmp(action, "call") == 0) { goto call; }
        if (strcmp(action, "check") == 0) { goto check; }
        if (strcmp(action, "end") == 0) { goto end; }

fold:
        folded[cp] = 1;
        cp = (cp+1)%number_of_players;
        continue;
bet:
        if (made_bet) {
            printf("| \e[31mError: a bet has already been made.\e[0m\n");
            goto round;
        }
        int bet;
        printf("| Enter the amount of the bet:\n");
        printf("> \e[33m"); // ]
        scanf("%d", &bet); printf("\e[0m"); // ]
        if (bet <= budgets[cp]) {
            budgets[cp] -= bet;
            bets[cp] += bet;
            bank += bet;
            maxbet += bet;
            made_bet = 1;
        } else {
            printf("| \e[31mError: bet value exceeds the budget of the player.\e[0m\n");
            goto bet;
        }
        cp = (cp+1)%number_of_players;
        continue;
raise:
        if (!made_bet) {
            printf("| \e[31mError: a bet has not been made.\e[0m\n");
            goto round;
        }
        int raise_amt;
        printf("| Enter the amount of the raise:\n");
        printf("> \e[33m"); // ]
        scanf("%d", &raise_amt); printf("\e[0m"); // ]
        if (raise_amt + bets[cp] <= maxbet) {
            printf("| \e[31mError: raise value must exceed the current bet.\e[0m\n");
            goto raise;
        }
        if (raise_amt > budgets[cp]) {
            printf("| \e[31mError: raise value exceeds the budget of the player.\e[0m\n");
            goto raise;
        }
        budgets[cp] -= raise_amt;
        bets[cp] += raise_amt;
        maxbet += raise_amt;
        bank += raise_amt;
        cp = (cp+1)%number_of_players;
        system("clear");
        continue;
call:
        if (!made_bet) {
            printf("| \e[31mError: a bet has not been made.\e[0m\n");
            goto round;
        }
        int addition = maxbet - bets[cp];
        if (addition <= budgets[cp]) {
            budgets[cp] -= addition;
            bets[cp] += addition;
            // maxbet += addition;
            bank += addition;
        } else {
            printf("| \e[31mError: bet value exceeds the budget of the player.\e[0m\n");
            goto round;
        }
        cp = (cp+1)%number_of_players;
        continue;
check:
        cp = (cp+1)%number_of_players;
        continue;
end:
        printf("| Enter the index of the winner:\n");
        int winner_index = 0;
        printf("> \e[33m"); // ]
        scanf("%d", &winner_index); printf("\e[0m"); // ]
        if (winner_index < 0 || winner_index >= number_of_players) {
            winner_index = 0;
        }

        budgets[winner_index] += bank;
        bank = base_bet * number_of_players;
        for (int i = 0; i < number_of_players; i++) {
            bets[i] = base_bet;
            budgets[i] -= base_bet;
            folded[i] = 0;
        }
        made_bet = 0;
        cp = 0;

        continue;
    }
}
