from PIL import Image
import math
import re

IMAGE_PATH = "./RA6963-N1/bitmaps/_resources/alarm_indicator.jpg"
ARRAY_NAME = "digit_8"

THRESHOLD = 128
BLACK_IS_ONE = True  # True = black -> 1, white/transparent -> 0


def safe_cpp_name(name):
    name = re.sub(r"\W+", "_", name)
    if name and name[0].isdigit():
        name = "_" + name
    return name


def pixel_is_black(pixel, threshold):
    r, g, b, a = pixel

    # Transparent counts as white/empty
    if a == 0:
        return False

    gray = (r + g + b) // 3
    return gray < threshold


def image_to_binary_header(image_path, array_name):
    img = Image.open(image_path).convert("RGBA")

    width, height = img.size

    if width % 8 != 0:
        print(f"ERROR: Image width must be divisible by 8. Got width = {width}")
        sys.exit(1)

    bytes_per_row = width // 8
    total_bytes = (width * height) // 8

    array_name = safe_cpp_name(array_name)

    print("#pragma once")
    print("#include <stdint.h>")
    print("#include <avr/pgmspace.h>")
    print()
    print(f"const uint8_t {array_name}[{total_bytes}] PROGMEM = {{")

    pixels = img.load()

    for y in range(height):
        row = []

        for byte_x in range(bytes_per_row):
            bits = ""

            for bit in range(8):
                x = byte_x * 8 + bit
                black = pixel_is_black(pixels[x, y], THRESHOLD)

                value = 1 if black else 0
                if not BLACK_IS_ONE:
                    value = 1 - value

                bits += str(value)

            row.append(f"0b{bits}")

        print("  " + ", ".join(row) + ",")

    print("};")


if __name__ == "__main__":
    image_to_binary_header(IMAGE_PATH, ARRAY_NAME)