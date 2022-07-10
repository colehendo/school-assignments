'''
 * Program or Lab #: 7
 *
 * Programmer: Cole Henderson
 *
 * Due Date: 4/12/17
 *
 * COMP141, Spring 2017
 *
 * Pledge: I have neither given nor received unauthorized aid
 *         on this program. 
 *
 * Description: This program plays Wheel of Fortune for one person. 
 *
 * Input: A secret phrase and letters guessed.
 *        
 * Output: The program will print the current state of the phrase, the winnings,
 *        and the worth of a consonant.
'''

import random

# This function is used to check the secret phrase entered by the host
def any_digits(string):
    for ix in string:
        if ix.isdigit():
            return True
    return False

# This function will be called once at the beginning of the program.
def input_secret_phrase():
    phrase = input("Enter the secret phrase: ")
    while True:
        if not phrase:
            phrase = input("Enter a non-empty phrase: ")
        elif any_digits(phrase) is True:
            phrase = input("Enter a phrase without numbers: ")
        else:
            return phrase.lower()
    
# This function will be called once at the beginning of the program.
def build_known_chars(secret_phrase):
    string = []
    for ix in secret_phrase:
        if ix.islower():
            continue
        else:
            string.append(ix)
        
    new_string = ''.join(string)
    return new_string


# This function will be called after every guess.
def build_puzzle_string(secret_phrase, known_chars):
    string = []
    for ix in secret_phrase:
        if ix in known_chars:
            string.append(ix)
        else:
            string.append('_')

    new_string = ''.join(string)
    return new_string

# This function is called in every turn to get the player's guess.
def input_guess(known_chars):
    guess = input("Enter the letter you would like to guess: ")
    while True:
        if len(guess) > 1 or len(guess) < 1:
            guess = input("Please enter a single letter: ")
        elif guess.isalpha() is False:
            guess = input("Please enter a letter: ")
        elif guess in known_chars:
            guess = input("Please enter a new letter: ")
        else:
            new_guess = guess.lower()
            return new_guess

# This function counts the occurrences of char in string
def count_char(char, string):
    count = 0
    for ix in string:
        if ix == char:
            count+=1
    return count

def main():
    print("Welcome to Wheel of Fortune!")
    winnings = 0
    vowels = ('aeiou')
    secret_phrase = input_secret_phrase()
    known_chars = build_known_chars(secret_phrase)
    string = build_puzzle_string(secret_phrase, known_chars)
    while '_' in string:
        print("\nPuzzle is: ", string)
        print("Winnings so far: $%d." % winnings)
        spin = random.randint(1, 100)
        print("Consonants are worth $%d." % spin)
        guess = input_guess(known_chars)
        known_chars += guess
        count = count_char(guess, secret_phrase)
        print("\nThere are %d %s's in the secret phrase." % (count, guess))
        if count > 0:
            string = build_puzzle_string(secret_phrase, known_chars)
            if guess in vowels:
                cost = count * 10
                print("You bought %d %s's for $%d." % (count, guess, cost))
                winnings -= cost
            else:
                won = spin * count
                print("You won $%d for %d %s's." % (won, count, guess))
                winnings += won 
        print()
    print("\nYou win! Your earnings are $%d" % winnings)
    print("The secret phrase is:", secret_phrase)

if __name__ == '__main__':
    main()
