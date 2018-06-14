/**
 * @file
 * Functions for retrieving and setting data in the MCS.
 */

#ifndef _SDK_OS_MCS_H
#define _SDK_OS_MCS_H

/**
 * Variable type: OBCD (@c struct @ref OBCD).
 */
#define VARTYPE_OBCD 0x01

/**
 * Variable type: CBCD (@c struct @ref CBCD).
 */
#define VARTYPE_CBCD 0x02

/**
 * Variable type: null-terminated string.
 */
#define VARTYPE_STR 0x05

/**
 * Variable type: list.
 */
#define VARTYPE_LIST 0x0A

/**
 * Variable type: matrix.
 */
#define VARTYPE_MAT 0x0C

/**
 * Variable type: program.
 */
#define VARTYPE_PRGM 0x47

/**
 * Variable type: function.
 */
#define VARTYPE_FUNC 0x48

/**
 * Variable type: geometry data.
 */
#define VARTYPE_GEO 0x54

/**
 * Error code signaling that the variable does not exist.
 */
#define MCS_NO_VARIABLE 0x30

/**
 * Error code signaling that the folder does not exist.
 */
#define MCS_NO_FOLDER 0x40

/**
 * Error code signaling that the folder already exists.
 */
#define MCS_FOLDER_EXISTS 0x42

/**
 * Error code signaling that the specified size was not a power of 2.
 */
#define MCS_SIZE_NOT_PO2 0x61

/**
 * Error code signaling that the variable was not a list.
 */
#define MCS_NOT_LIST 0x62

/**
 * Error code signaling that the index was out of bounds.
 */
#define MCS_INDEX_OOB 0x63

/**
 * Retrieves the BCD digit at position @c i (0 representing the least
 * significant place) from a BCD encoded number @c n.
 *
 * @param n The number to retrieve a digit from.
 * @param i The index of the digit to retrieve.
 */
#define GET_BCD_DIGIT(n, i) (((n) >> ((i) * 4)) & 0xF)

/**
 * Stores a decimal number. Both the mantissa and exponent are stored in BCD.
 *
 * See ClassPad 300 SDK documentation for more details.
 */
struct OBCD {
	uint8_t mantissa[10];
	uint16_t exponent;
};

/**
 * Stores a complex number, with real part @c re and imaginary part @c im.
 *
 * See ClassPad 300 SDK documentation for more details.
 */
struct CBCD {
	struct OBCD re;
	struct OBCD im;
};

/**
 * Creates a folder in the MCS.
 *
 * If the folder name is greater than the 8 character max, it is truncated to a
 * valid length.
 *
 * The @p folderIndex parameter is still populated if the folder already exists.
 * However, its purpose is currently unknown.
 *
 * @param folder The name of the folder to create.
 * @param folderIndex A pointer to store an unknown index for the folder in.
 * @return 0 if the folder was created successfully, or @c MCS_FOLDER_EXISTS if
 * the folder already exists.
 */
int MCS_CreateFolder(char *folder, uint8_t *folderIndex);

/**
 * Retrieves information about a variable stored in the MCS.
 *
 * @param folder The folder the requested MCS variable is located in.
 * @param name The name of the requested MCS variable.
 * @param variableType A pointer to store the variable's type in.
 * @param name2 A pointer to store the variable's name in.
 * @param data A pointer to store a pointer to the variable's data in.
 * @param size A pointer to store the length of the variable's data in.
 * @return 0 if the variable exists, @c MCS_NO_VARIABLE if the variable does not
 * exist, or @c MCS_NO_FOLDER if the folder does not exist.
 */
int MCS_GetVariable(
	char *folder, char *name,
	uint8_t *variableType, char **name2, void **data, uint32_t *size
);

/**
 * Creates a list variable in the MCS.
 *
 * The list is initially populated with the default value of the variable type
 * specified. If the variable already exists, it is overwritten (even if it's
 * not a list).
 * 
 * @param folder The folder to create the list in.
 * @param name The desired name of the variable.
 * @param size The size of the variable type specified.
 * @praram length The number of entries in the list.
 * @param variableType The variable type to initialize the list with (macros
 * starting with @c VARTYPE).
 * @return 0 on success, @c MCS_NO_FOLDER if the requested folder does not
 * exist.
 */
int MCS_List_Create(
	char *folder, char *name,
	uint32_t size, uint16_t length, uint8_t variableType
);

/**
 * Sets an element of a list in the MCS.
 *
 * @param folder The folder containing the list variable.
 * @param name The name of the list variable.
 * @praam size The size of the data to store in the element.
 * @param index The index into the list of the element to modify.
 * @param variableType The type of the data to be stored in the list element
 * (macros starting with @c VARTYPE).
 * @param data A pointer to the data to copy into the list element.
 * @return 0 on success, @c MCS_NO_FOLDER if the requested folder does not
 * exist, @c MCS_NO_VARIABLE if the variable does not exist, @c MCS_NOT_LIST
 * if the variable was not a list, or @c MCS_INDEX_OOB if the index was out of
 * bounds.
 */
int MCS_List_Set(
	char *folder, char *name,
	uint32_t size, int index, uint8_t variableType, void *data
);

/**
 * Sets the value of a variable stored in the MCS. Can be used to create a new
 * variable, or replace the value of an existing variable.
 *
 * This function does not create the folder if it does not exist. Use
 * @ref MCS_CreateFolder to create a folder if necessary.
 *
 * @param folder The folder the variable to set it stored in.
 * @param name The name of the desired MCS variable.
 * @param variableType The type of the variable (macros starting with
 * @c VARTYPE).
 * @param size The size of the variable's data.
 * @param data A pointer to the data to copy into the variable.
 * @return 0 if the variable was set successfully, or @c MCS_NO_FOLDER if the
 * specified folder does not exist.
 */
int MCS_SetVariable(
	char *folder, char *name,
	uint8_t variableType, uint32_t size, void *data
);

#endif
