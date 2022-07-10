from libs.song_lyrics import SongLyrics
from libs.user_input import UserInput


def sing_bottles_of_beer_song():
    song_lyrics = SongLyrics()
    user_input = UserInput()

    beverage_name = user_input.get_beverage_name()
    number_of_bottles_on_the_wall = user_input.get_initial_bottle_count()
    number_of_bottles_to_remove = user_input.get_number_of_bottles_to_remove(
        number_of_bottles_on_the_wall
    )

    song_lyrics.generate_and_print_song_lyrics(
        beverage_name, number_of_bottles_on_the_wall, number_of_bottles_to_remove
    )


if __name__ == "__main__":
    sing_bottles_of_beer_song()
