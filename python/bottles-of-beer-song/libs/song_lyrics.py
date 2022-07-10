class SongLyrics:
    @staticmethod
    def __print_song_lyrics(
        beverage_name: str,
        number_of_bottles_on_the_wall: int,
        number_of_bottles_to_remove: int,
        bottles_remaining_on_the_wall: int,
    ):
        if number_of_bottles_on_the_wall == 1:
            bottle_reference = "bottle"
            beverage_reference = "it"
        else:
            bottle_reference = "bottles"
            beverage_reference = "them"

        print(
            f"""
{number_of_bottles_on_the_wall} {bottle_reference} of {beverage_name} on the wall,
{number_of_bottles_on_the_wall} {bottle_reference} of {beverage_name}.
Take {number_of_bottles_to_remove} down, pass {beverage_reference} around,
{bottles_remaining_on_the_wall} {bottle_reference} of {beverage_name} on the wall."""
        )

    def generate_and_print_song_lyrics(
        self,
        beverage_name: str,
        number_of_bottles_on_the_wall: int,
        number_of_bottles_to_remove: int,
    ):
        while number_of_bottles_on_the_wall > 0:
            if number_of_bottles_to_remove > number_of_bottles_on_the_wall:
                number_of_bottles_to_remove = number_of_bottles_on_the_wall

            bottles_remaining_on_the_wall = (
                number_of_bottles_on_the_wall - number_of_bottles_to_remove
            )

            self.__print_song_lyrics(
                beverage_name,
                number_of_bottles_on_the_wall,
                number_of_bottles_to_remove,
                bottles_remaining_on_the_wall,
            )

            number_of_bottles_on_the_wall = bottles_remaining_on_the_wall

        print(
            f"\nThere are 0 bottles of {beverage_name} left on the wall.\nYou ran out!"
        )
