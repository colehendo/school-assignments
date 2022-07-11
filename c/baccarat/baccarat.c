#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

// mutex and condition variables
pthread_mutex_t first_draw, second_draw, scores;
pthread_cond_t draw, cards_in, scores_checked, p_to_b;

// SHARED VARIABLES
// where to store card values
int p_card_1 = 0;
int p_card_2 = 0;
int b_card_1 = 0;
int b_card_2 = 0;

int p_card_3 = 0;
int b_card_3 = 0;

// round scores
int p_score = 0;
int b_score = 0;

// CONDITION VARIABLES
// 0 means both have gone, 1 is one has gone, 2 is ready to go
int first_round_ready = 0;
int second_round_ready = 0;

// 2 means unread updates, 0 means read
int checking_scores = 0;

int game_over = 0;

// if the player or bank has drawn their cards in a certain round
int p_rnd_1 = 0;
int p_rnd_2 = 0;
int b_rnd_1 = 0;
int b_rnd_2 = 0;

// 1 means no winner, 2 means winner
int winner = 0;

void *player(void *arg) {
  int temp_p_card_1 = 0;
  int temp_p_card_2 = 0;
  int temp_p_card_3 = 0;

  do {
    // wait to play
    if (pthread_mutex_lock(&first_draw) != 0)
      perror("player lock first draw");
    while (first_round_ready < 1) {
      if (pthread_cond_wait(&draw, &first_draw) != 0)
        perror("player wait first draw");
    }

    // return if game over
    if (game_over == 1) {
      if (pthread_cond_signal(&cards_in) != 0) //
        perror("player signal game over");
      if (pthread_mutex_unlock(&first_draw) != 0)
        perror("player unlock game over");
    
      return NULL;
    }

    // draw and store first two cards
    temp_p_card_1 = p_card_1 = rand() % 12 + 2;
    temp_p_card_2 = p_card_2 = rand() % 12 + 2;
  
    first_round_ready -= 1;
    if (first_round_ready == 1) {
      if (pthread_cond_signal(&draw) != 0)
        perror("player signal first draw");
    }
    else {
      if (pthread_cond_signal(&cards_in) != 0)
        perror("player signal first draw");
    }
    if (pthread_mutex_unlock(&first_draw) != 0)
      perror("player unlock first draw");

    if (pthread_mutex_lock(&scores) != 0)
      perror("player lock first score");
    while (checking_scores == 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("player wait first score");
    }

    if (temp_p_card_1 < 10)
      p_score += temp_p_card_1;
    if (temp_p_card_2 < 10)
      p_score += temp_p_card_2;

    p_score = p_score % 10;

    checking_scores += 1;
    if (pthread_cond_signal(&scores_checked) != 0)
      perror("player signal first score");
    if (pthread_mutex_unlock(&scores) != 0)
      perror("player unlock first score");

    // wait to draw third card
    if (pthread_mutex_lock(&second_draw) != 0)
      perror("player lock second draw");
    while (second_round_ready != 2) {
      if (pthread_cond_wait(&draw, &second_draw) != 0)
        perror("player wait second draw");
    }

    // no winner decided, stand or draw
    if (winner == 1){
      if (p_score > 5)
        temp_p_card_3 = p_card_3 = 1; 
      else {
        temp_p_card_3 = p_card_3 = rand() % 12 + 2;
      }
    }

    second_round_ready = 1;
    if (pthread_cond_signal(&p_to_b) != 0)
      perror("player signal second draw");
    if (pthread_mutex_unlock(&second_draw) != 0)
      perror("player unlock second draw");

    if (pthread_mutex_lock(&scores) != 0)
      perror("player lock second score");
    while (checking_scores == 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("player wait second score");
    }

    if (temp_p_card_3 >=2 && temp_p_card_3< 10) {
      p_score += temp_p_card_3;
      p_score = p_score % 10;
    }

    checking_scores += 1;
    if (pthread_cond_signal(&scores_checked) != 0)
      perror("player signal second score");
    if (pthread_mutex_unlock(&scores) != 0)
      perror("player unlock second score");
  } while (1);
}

void *bank(void *arg) {
  int temp_b_card_1 = 0;
  int temp_b_card_2 = 0;
  int temp_b_card_3 = 0;
  
  do {
    // wait to play
    if (pthread_mutex_lock(&first_draw) != 0)
      perror("bank lock first draw");
    while (first_round_ready < 1) {
      if (pthread_cond_wait(&draw, &first_draw) != 0)
        perror("bank wait first draw");
    }

    // return if game over
    if (game_over == 1) {
      if (pthread_cond_signal(&cards_in) != 0)
        perror("bank signal game over");
      if (pthread_mutex_unlock(&first_draw)!= 0)
        perror("bank unlock game over");

      return NULL;
    }

    // draw and store first two cards
    temp_b_card_1 = b_card_1 = rand() % 12 + 2;
    temp_b_card_2 = b_card_2 = rand() % 12 + 2;

    first_round_ready -= 1;
    if (first_round_ready == 1) {
      if (pthread_cond_signal(&draw) != 0)
        perror("bank signal first draw");
    }
    else {
      if (pthread_cond_signal(&cards_in) != 0)
        perror("bank signal first draw");
    }
    if (pthread_mutex_unlock(&first_draw) != 0)
      perror("bank unlock first draw");
    
    if (pthread_mutex_lock(&scores) != 0)
      perror("bank lock first score");
    while (checking_scores == 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("bank wait first draw");
    }

    if (b_card_1 < 10)
      b_score += b_card_1;
    if (b_card_2 < 10)
      b_score += b_card_2;

    b_score = b_score % 10;

    checking_scores += 1;
    if (pthread_cond_signal(&scores_checked) != 0)
      perror("bank signal first score");
    if (pthread_mutex_unlock(&scores) != 0)
      perror("bank unlock first score");

    // wait for player to draw third card
    if (pthread_mutex_lock(&second_draw) != 0)
      perror("bank lock second draw");
    while (second_round_ready != 1) {
      if (pthread_cond_wait(&p_to_b, &second_draw) != 0)
        perror("bank wait second draw");
    }

    // no winner decided, draw
    if (winner == 1){
      if (b_score == 7)
        temp_b_card_3 = b_card_3 = 1;
      else if ((b_score <= 2) || (b_score == 3 && p_card_3 != 8) || (b_score == 4 && (p_card_3 >= 2 && p_card_3 <= 7)) || (b_score == 5 && (p_card_3 >= 4 && p_card_3 <= 7)) || (b_score == 6 && (p_card_3 == 6 || p_card_3 == 7))) {
        temp_b_card_3 = b_card_3 = rand() % 12 + 2;
      }
    }

    second_round_ready = 0;
    if (pthread_cond_signal(&cards_in) != 0)
      perror("bank signal second draw");
    if (pthread_mutex_unlock(&second_draw) != 0)
      perror("bank unlock second draw");
  
    if (pthread_mutex_lock(&scores) != 0)
      perror("bank lock second score");
    while (checking_scores == 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("bank wait second score");
    }

    if (temp_b_card_3 >=2 && temp_b_card_3< 10) {
      b_score += temp_b_card_3;
      b_score = b_score % 10;
    }

    checking_scores += 1;
    if (pthread_cond_signal(&scores_checked) != 0)
      perror("bank signal second score");
    if (pthread_mutex_unlock(&scores) != 0)
      perror("bank unlock second score");
  } while (1);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Provide the number of rounds to play as a command line argument.\n");
    return 0;
  }

  if (argc > 2) {
    printf("Provide only one command line argument: the number of rounds to play.\n");
    return 0;
  }
  
  int p_wins = 0;
  int b_wins = 0;
  int ties = 0;
  int rounds = atoi(argv[1]);
  pthread_t p, b;

  const char * cards[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

  // make threads
  if (pthread_create(&p, NULL, player, NULL) != 0)
    perror("player creation");
  if (pthread_create(&b, NULL, bank, NULL) != 0)
    perror("bank creation");

  printf("Beginning %d Rounds...\n", rounds);
  
  // loop that plays the game
  for (int i = 0; i < rounds; i++) {
    srand((i + 1) * 100 * time(NULL));
    // print round
    printf("------------------------\n");
    printf("Round %d:\n", i + 1);

    // start the game 
    if (pthread_mutex_lock(&first_draw) != 0)
      perror("main lock first draw");
    
    first_round_ready = 2;
    if (pthread_cond_broadcast(&draw) != 0)
      perror("main signal first draw");
    if (pthread_mutex_unlock(&first_draw) != 0)
      perror("main unlock first draw");

    // wait for the scores to come in
    if (pthread_mutex_lock(&first_draw) != 0)
      perror("main lock first print");
    while (first_round_ready > 0 ) {
      if (pthread_cond_wait(&cards_in, &first_draw) != 0)
        perror("main wait first print");
    }

    printf("Player\tdraws %s, %s\n", cards[p_card_1 - 2], cards[p_card_2 - 2]);
    printf("Bank\tdraws %s, %s\n", cards[b_card_1 - 2], cards[b_card_2 - 2]);
   
    second_round_ready = 2;
    if (pthread_cond_signal(&draw) != 0)
      perror("main signal second draw");
    if (pthread_mutex_unlock(&first_draw) != 0)
      perror("main unlock second draw");

    if (pthread_mutex_lock(&scores) != 0)
      perror("main lock winner check");
    while (checking_scores < 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("main wait winner check");
    }

    // player and bank wait on winner to be > 0, make decisions based on if its 1 or 2
    if (p_score >= 8 || b_score >= 8)
      winner = 2;
    else
      winner = 1;
    
    checking_scores = 0;
    if (pthread_cond_signal(&scores_checked) != 0)
      perror("main signal winner check");
    if (pthread_mutex_unlock(&scores) != 0)
      perror("main unlock winner check");

    // wait for third cards to be picked or not
    if (pthread_mutex_lock(&second_draw) != 0)
      perror("main lock second print");
    while (second_round_ready != 0) {
      if (pthread_cond_wait(&cards_in, &second_draw) != 0)
        perror("main wait second print");
    }

    if (p_card_3 == 1)
      printf("Player\tstands\n");
    else if (p_card_3 > 1)
      printf("Player\tdraws %s\n", cards[p_card_3 - 2]);

    if (b_card_3 == 1)
      printf("Bank\tstands\n");
    else if (b_card_3 > 1)
      printf("Bank\tdraws %s\n", cards[b_card_3 - 2]);

    // reset cards
    p_card_3 = b_card_3 = 0;
    if (pthread_mutex_unlock(&second_draw) != 0)
      perror("main unlock second print");

    if (pthread_mutex_lock(&scores) != 0)
      perror("main lock result");
    while (checking_scores < 2) {
      if (pthread_cond_wait(&scores_checked, &scores) != 0)
        perror("main wait result");
    }

    // determine score of game
    if (p_score == b_score) {
      ties += 1;
      printf("It's a tie!\n");
    }

    else if (p_score > b_score) {
      p_wins += 1;
      printf("Player wins!\n");
    }

    else {
      b_wins += 1;
      printf("Bank wins!\n");
    }
    
    // reset round values
    winner = p_score = b_score = 0;
    checking_scores = 0;
    if (pthread_mutex_unlock(&scores) != 0)
      perror("main unlock result");
  }

  // signal game over
  if (pthread_mutex_lock(&first_draw) != 0)
    perror("main lock game over");

  game_over = 1;
  first_round_ready = 2;
  
  if (pthread_cond_broadcast(&draw) != 0)
    perror("main signal game over");
  if (pthread_mutex_unlock(&first_draw) != 0)
    perror("main unlock game over");
 
  // join child threads
  if (pthread_join(p, NULL) != 0)
    perror("player join");
  if (pthread_join(b, NULL) != 0)
    perror("bank join");

  // print final results
  printf("------------------------\n");
  printf("Results:\nPlayer\t: %d\nBank\t: %d\nTies\t: %d\n", p_wins, b_wins, ties);
  
  if (p_wins > b_wins)
    printf("Player wins!\n");
  else if (b_wins > p_wins)
    printf("Bank wins!\n");
  else
    printf("It's a tie!\n");

  return 0;
}

