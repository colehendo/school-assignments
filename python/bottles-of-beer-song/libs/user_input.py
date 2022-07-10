from typing import Optional
from .utils import Utils


class UserInput:
    def __init__(self):
        print("We will be singing the classic bottles on the wall song.\n\n")
        self.utils = Utils()

    def __get_user_input_for_initial_bottle_count(self) -> Optional[int]:
        initial_bottle_count_input = input(
            "\nEnter the total number of bottles on the wall: "
        )
        initial_bottle_count = self.utils.change_input_to_int(
            initial_bottle_count_input,
            error_message_beginning="The total number of bottles on the wall",
        )

        if type(initial_bottle_count) == int and initial_bottle_count <= 0:
            print(
                "The total number of bottles on the wall must be a number greater than 0"
            )
            return

        return initial_bottle_count

    def __get_user_input_for_bottles_to_remove(
        self,
        number_of_bottles_on_the_wall: int,
    ) -> Optional[int]:
        number_of_bottles_to_remove_input = input(
            "\nEnter how many bottles will be removed at a time: "
        )
        number_of_bottles_to_remove = self.utils.change_input_to_int(
            number_of_bottles_to_remove_input,
            error_message_beginning="The number of bottles to remove",
        )

        if type(number_of_bottles_to_remove) == int:
            if number_of_bottles_to_remove <= 0:
                print("The number of bottles to remove must be a number greater than 0")
                return
            if number_of_bottles_to_remove > number_of_bottles_on_the_wall:
                print(
                    f"The number of bottles to remove must be between 1 and {number_of_bottles_on_the_wall}"
                )
                return

        return number_of_bottles_to_remove

    @staticmethod
    def get_beverage_name() -> str:
        beverage_name = input("Enter your beverage of choice: ")
        while not beverage_name:
            beverage_name = input("\n\nInput invalid. Enter your beverage of choice: ")

        return beverage_name

    def get_initial_bottle_count(self) -> int:
        initial_bottle_count = self.__get_user_input_for_initial_bottle_count()

        while initial_bottle_count is None:
            initial_bottle_count = self.__get_user_input_for_initial_bottle_count()

        return initial_bottle_count

    def get_number_of_bottles_to_remove(
        self, number_of_bottles_on_the_wall: int
    ) -> int:
        number_of_bottles_to_remove = self.__get_user_input_for_bottles_to_remove(
            number_of_bottles_on_the_wall
        )

        while number_of_bottles_to_remove is None:
            number_of_bottles_to_remove = self.__get_user_input_for_bottles_to_remove(
                number_of_bottles_on_the_wall
            )

        return number_of_bottles_to_remove
