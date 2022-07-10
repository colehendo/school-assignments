from typing import Optional


class Utils:
    @staticmethod
    def change_input_to_int(
        string_input: str, error_message_beginning: str
    ) -> Optional[int]:
        try:
            int_output = int(string_input)
        except ValueError:
            print(f"{error_message_beginning} must be a number")
            return

        return int_output
