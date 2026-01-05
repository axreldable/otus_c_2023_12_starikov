#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// KOI8-R to UTF-8 mapping for characters 0x80-0xFF
static const char* koi8r_to_utf8[128] = {
    "─", "│", "┌", "┐", "└", "┘", "├", "┤", "┬", "┴", "┼", "▀", "▄", "█", "▌", "▐",
    "░", "▒", "▓", "⌠", "■", "∙", "√", "≈", "≤", "≥", " ", "⌡", "°", "²", "·", "÷",
    "═", "║", "╒", "ё", "╓", "╔", "╕", "╖", "╗", "╘", "╙", "╚", "╛", "╜", "╝", "╞",
    "╟", "╠", "╡", "Ё", "╢", "╣", "╤", "╥", "╦", "╧", "╨", "╩", "╪", "╫", "╬", "©",
    "ю", "а", "б", "ц", "д", "е", "ф", "г", "х", "и", "й", "к", "л", "м", "н", "о",
    "п", "я", "р", "с", "т", "у", "ж", "в", "ь", "ы", "з", "ш", "э", "щ", "ч", "ъ",
    "Ю", "А", "Б", "Ц", "Д", "Е", "Ф", "Г", "Х", "И", "Й", "К", "Л", "М", "Н", "О",
    "П", "Я", "Р", "С", "Т", "У", "Ж", "В", "Ь", "Ы", "З", "Ш", "Э", "Щ", "Ч", "Ъ"
};

// CP1251 to UTF-8 mapping for characters 0x80-0xFF
static const char* cp1251_to_utf8[128] = {
    "Ђ", "Ѓ", "‚", "ѓ", "„", "…", "†", "‡", "€", "‰", "Љ", "‹", "Њ", "Ќ", "Ћ", "Џ",
    "ђ", "'", "'", "“", "”", "•", "–", "—", "˜", "™", "љ", "›", "њ", "ќ", "ћ", "џ",
    " ", "Ў", "ў", "Ј", "¤", "Ґ", "¦", "§", "Ё", "©", "Є", "«", "¬", "­", "®", "Ї",
    "°", "±", "І", "і", "ґ", "µ", "¶", "·", "ё", "№", "є", "»", "ј", "Ѕ", "µ", "ї",
    "А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П",
    "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
    "а", "б", "в", "г", "д", "е", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п",
    "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я"
};

// ISO-8859-5 to UTF-8 mapping for characters 0x80-0xFF
static const char* iso8859_5_to_utf8[128] = {
    "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86", "\x87", "\x88", "\x89", "\x8A", "\x8B", "\x8C", "\x8D", "\x8E", "\x8F",
    "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", "\x98", "\x99", "\x9A", "\x9B", "\x9C", "\x9D", "\x9E", "\x9F",
    " ", "Ё", "Ђ", "Ѓ", "Є", "Ѕ", "І", "Ї", "Ј", "Љ", "Њ", "Ћ", "Ќ", "­", "Ў", "Џ",
    "А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П",
    "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я",
    "а", "б", "в", "г", "д", "е", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п",
    "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я",
    "№", "ё", "ђ", "ѓ", "є", "ѕ", "і", "ї", "ј", "љ", "њ", "ћ", "ќ", "§", "ў", "џ"
};

/**
 * Converts a single byte encoding to UTF-8.
 *
 * Notes:
 *  KOI8-R, CP1251, and ISO-8859-5 all use identical mappings for bytes 0-127 (0x00-0x7F)
 *  They only differ in how they map bytes 128-255 (0x80-0xFF)
 *  --
 *  ASCII characters occupy the range 0-127 (0x00-0x7F)
 *  ASCII is already valid UTF-8 for values < 128
 *
 * @param byte - input byte
 * @param from - encoding name, e.g. ('cp1251', 'iso-8859-5', 'koi8-r')
 * @param out - output buffer (must have at least 4 bytes)
 * @return number of bytes written to output
 */
int convert(unsigned char byte, const char *from, unsigned char *out) {
    const char *utf8_char;

    // ASCII characters are the same in all encodings (first 127 characters)
    if (byte < 0x80) {
        out[0] = byte;
        return 1;
    }

    // Select encoding table and get UTF-8 string
    int mapping_index = byte - 0x80;
    if (strcmp(from, "koi8-r") == 0) {
        utf8_char = koi8r_to_utf8[mapping_index];
    } else if (strcmp(from, "cp1251") == 0) {
        utf8_char = cp1251_to_utf8[mapping_index];
    } else if (strcmp(from, "iso-8859-5") == 0) {
        utf8_char = iso8859_5_to_utf8[mapping_index];
    } else {
        // Unknown encoding, pass through
        out[0] = byte;
        return 1;
    }

    // Copy UTF-8 bytes to output buffer
    int len = 0;
    while (utf8_char[len] != '\0') {
        out[len] = (unsigned char)utf8_char[len];
        len++;
    }

    return len;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_file> <encoding> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *in_file = argv[1];
    char *encoding = argv[2];
    char *out_file = argv[3];

    FILE *inFile = fopen(in_file, "rb");
    FILE *outFile = fopen(out_file, "wb");
    if (!inFile || !outFile) return EXIT_FAILURE;

    int byte;
    unsigned char utf8_buf[4];
    while ((byte = fgetc(inFile)) != EOF) {
        int len = convert((unsigned char)byte, encoding, utf8_buf);
        fwrite(utf8_buf, 1, len, outFile);
    }

    fclose(inFile);
    fclose(outFile);
    return EXIT_SUCCESS;
}
