import random
from typing import List, Tuple

VOWELS = "aeiou"


class WheelOfFortune:
    @staticmethod
    def __secret_phrase_is_valid(secret_phrase: str) -> bool:
        if not secret_phrase or secret_phrase.isspace():
            print("The phrase must contain at least one letter\n")
            return False
        if all(
            character.isalpha() or character.isspace() for character in secret_phrase
        ):
            return True
        print("The phrase can only contain letters and spaces\n")
        return False

    def input_secret_phrase(self) -> str:
        secret_phrase = input("Enter the secret phrase: ")

        while not self.__secret_phrase_is_valid(secret_phrase):
            secret_phrase = input("Enter the secret phrase: ")

        return secret_phrase

    @staticmethod
    def __build_puzzle_string(secret_phrase: str, letters_guessed: List[str]):
        puzzle_string = ""

        for character in secret_phrase:
            puzzle_string += (
                character
                if character.lower() in letters_guessed or character.isspace()
                else "_"
            )

        return puzzle_string

    @staticmethod
    def __user_guess_is_valid(letter_guessed: str, letters_guessed: List[str]) -> bool:
        if not letter_guessed.isalpha():
            print("Your guess must be a letter")
        elif len(letter_guessed) != 1:
            print("Your guess must be one letter")
        elif letter_guessed.lower() in letters_guessed:
            print(
                f"'{letter_guessed}' has already been guessed.\nPlease enter a new letter"
            )
        else:
            return True
        return False

    def __get_user_guess(self, letters_guessed: List[str]) -> str:
        letter_guessed = input("\nEnter the letter you would like to guess: ")
        while not self.__user_guess_is_valid(letter_guessed, letters_guessed):
            letter_guessed = input("\nEnter the letter you would like to guess: ")

        return letter_guessed.lower()

    @staticmethod
    def __print_occurrence_info(letter_guessed: str, occurrence_of_guess: int):
        if occurrence_of_guess == 1:
            verb = "is"
            plural = ""
        else:
            verb = "are"
            plural = "'s"

        print(
            f"\nThere {verb} {occurrence_of_guess} {letter_guessed}{plural} in the secret phrase."
        )

    @staticmethod
    def __print_current_game_info(puzzle_string: str, winnings: int):
        print(f"\nCurrent puzzle: '{puzzle_string}'")
        print(f"Current winnings: ${winnings}")

    def __handle_correct_guess(
        self,
        current_winnings: int,
        spin: int,
        secret_phrase: str,
        letter_guessed: str,
        letters_guessed: List[str],
        occurrence_of_guess: int,
    ) -> Tuple[str, int]:
        puzzle_string = self.__build_puzzle_string(secret_phrase, letters_guessed)

        if letter_guessed in VOWELS:
            vowel_cost = occurrence_of_guess * 10
            print(
                f"You bought {occurrence_of_guess} {letter_guessed}'s for ${vowel_cost}."
            )
            new_winnings = current_winnings - vowel_cost
        else:
            consonant_winnings = spin * occurrence_of_guess
            print(
                f"You won ${consonant_winnings} for {occurrence_of_guess} {letter_guessed}'s."
            )
            new_winnings = current_winnings + consonant_winnings

        if "_" in puzzle_string:
            self.__print_current_game_info(puzzle_string, new_winnings)

        return puzzle_string, new_winnings

    def play_wheel_of_fortune(self, secret_phrase: str) -> int:
        winnings = 0
        letters_guessed = []
        puzzle_string = self.__build_puzzle_string(secret_phrase, letters_guessed)

        self.__print_current_game_info(puzzle_string, winnings)

        while "_" in puzzle_string:
            spin = random.randint(1, 100)
            print(f"\nConsonants are now worth {spin}")
            letter_guessed = self.__get_user_guess(letters_guessed)
            letters_guessed.append(letter_guessed)
            occurrence_of_guess = secret_phrase.lower().count(letter_guessed)

            self.__print_occurrence_info(letter_guessed, occurrence_of_guess)

            if occurrence_of_guess > 0:
                puzzle_string, winnings = self.__handle_correct_guess(
                    winnings,
                    spin,
                    secret_phrase,
                    letter_guessed,
                    letters_guessed,
                    occurrence_of_guess,
                )

        return winnings


def main():
    wheel_of_fortune = WheelOfFortune()

    print("Welcome to Wheel of Fortune!\n\n")

    secret_phrase = wheel_of_fortune.input_secret_phrase()
    winnings = wheel_of_fortune.play_wheel_of_fortune(secret_phrase)

    print(f"\nYou win! Your total earnings are ${winnings}")
    print(f"The secret phrase is: '{secret_phrase}'")


if __name__ == "__main__":
    main()
