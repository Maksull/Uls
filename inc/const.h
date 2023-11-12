#ifndef ULS_CONST_H
#define ULS_CONST_H

// Constants related to formatting and file attributes
static const int TAB_SIZE = 8;            // Size of a tab character in spaces
static const int CAT_WIDTH = 80;          // Maximum width for formatted output
static const int SIX_MON_LEN = 15552000;  // Length of six months in seconds
#define XATTR_LEN 10000                    // Maximum length for extended attribute data

// Constants related to command-line flags and usage messages
static const int FLAG_COUNT = 19;         // Total number of supported flags
static const char const_uls_flags[] = { 'a', 'A', 'G', 'l', 'r', 't', 'c', 'u', 'S', 'R',
                                         'T', 'h', '@', 'e', 'C', '1', 'F', 'f', 'm' };
                                         
static const char* USAGE_STR    = "usage: uls [-ACFGRSTacefhlmrtu1] [file ...]\n";  // Usage message
static const char* ULS_STR      = "uls: ";  // Prefix for uls-related messages
static const char* ILOP_STR     = "illegal option -- ";  // Error message for illegal option
static const char* EXE_STR      = "uls";  // Executable name

// Constants for color codes
static const char* COLOR_DEFAULT        = "\033[0m";

static const char* COLOR_RED_DFLT       = "\033[0;31m";
static const char* COLOR_GREEN_DFLT     = "\033[0;32m";
static const char* COLOR_YELLOW_DFLT    = "\033[0;33m";
static const char* COLOR_BLUE_DFLT      = "\033[0;34m";
static const char* COLOR_MAGENTA_DFLT   = "\033[0;35m";

static const char* COLOR_BLUE_CYAN      = "\033[46;34m";
static const char* COLOR_BLUE_YELLOW    = "\033[43;34m";
static const char* COLOR_BLACK_RED      = "\033[41;30m";
static const char* COLOR_BLACK_CYAN     = "\033[46;30m";
static const char* COLOR_BLACK_GREEN    = "\033[42;30m";
static const char* COLOR_BLACK_YELLOW   = "\033[43;30m";

#endif
