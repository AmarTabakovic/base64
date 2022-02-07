#include <stdio.h>
#include <stdlib.h>

#define PROGRAM_NAME "base64"
#define BASE64_TABLE "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

char *base64_table = BASE64_TABLE;

/**
 * @brief Converts and prints three given bytes to four groups of six bits.
 *
 * TODO: Optimize end conditions in order to set the new_chars only once instead of twice (0 and '=').
 *
 * @param byte1 first byte
 * @param byte2 second byte
 * @param byte3 third byte
 */
void convert_and_print(int byte1, int byte2, int byte3)
{
	char new_byte1, new_byte2, new_byte3, new_byte4;
	char new_char1, new_char2, new_char3, new_char4;

	if (byte1 == EOF)
	{
		return;
	}

	new_byte1 = byte1 >> 2;

	if (byte2 == EOF)
	{
		new_byte2 = (byte1 << 4) & 63;
		new_char3 = '=';
		new_byte3 = 0;
		new_byte4 = 0;
	}
	else
	{
		new_byte2 = ((byte1 << 4) | (byte2 >> 4)) & 63;
		new_byte3 = ((byte3 >> 6) | (byte2 << 2)) & 63;
		new_byte4 = byte3 & 63;
	}

	if (byte3 == EOF)
	{
		new_byte3 = (byte2 << 2) & 63;
		new_byte4 = 0;
	}
	else
	{
		new_byte4 = byte3 & 63;
	}

	new_char1 = *(base64_table + new_byte1);
	new_char2 = *(base64_table + new_byte2);
	new_char3 = *(base64_table + new_byte3);
	new_char4 = *(base64_table + new_byte4);

	if (byte3 == EOF)
	{
		new_char4 = '=';
	}
	if (byte2 == EOF)
	{
		new_char3 = '=';
	}

	printf("%c%c%c%c", new_char1, new_char2, new_char3, new_char4);
}

/**
 * @brief Reads three bytes from the given filename and forwards them to the base 64 conversion.
 *
 * @param file_name filename of the file to read
 * @return int 0 if the file was found, 1 if the file was not found
 */
int read_file(char *file_name)
{
	FILE *file_pointer;
	int byte1, byte2, byte3;

	if ((file_pointer = fopen(file_name, "r")) == NULL)
	{
		return 1;
	}

	while (1)
	{
		byte1 = fgetc(file_pointer);
		byte2 = fgetc(file_pointer);
		byte3 = fgetc(file_pointer);

		convert_and_print(byte1, byte2, byte3);

		if (byte1 == EOF || byte2 == EOF || byte3 == EOF)
		{
			break;
		}
	}
	printf("\n");

	fclose(file_pointer);
	return 0;
}

/**
 * @brief Checks the given command line arguments and initializes the base 64 conversion.
 *
 * @param argc number of command line arguments
 * @param argv list of command line arguments
 * @return int return code
 */
int main(int argc, char *argv[])
{
	int code;

	if (argc < 2)
	{
		printf("%s: Please specify a filename.\n", PROGRAM_NAME);
		return EXIT_FAILURE;
	}
	else if (argc > 2)
	{
		printf("%s: Please specify only one filename.\n", PROGRAM_NAME);
		return EXIT_FAILURE;
	}
	else
	{
		code = read_file(argv[1]);
		if (code != 0)
		{
			printf("%s: File not found.\n", PROGRAM_NAME);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
