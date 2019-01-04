#ifndef __UTILS_INC
#define __UTILS_INC

/* null pointer */
#define NULL  (0 as @void)

/* EOF marker */
#define EOF   (-1)

/* true and false */
#define TRUE  1
#define FALSE 0

/* basic types used for utilities */
typ FileDescT: @void;
typ ByteT    : byte;
typ Int08T   : byte;
typ Int16T   : twin;
typ Int32T   : four;
typ Int64T   : eigh;
typ BooleanT : byte;
typ StringT  : [byte];

/* memory */
dec UtilsMemAlloc        : func(Size:       Int32T) returns @void;

/* string routines */
dec UtilsStringAlloc     : func(Size:       Int32T                       ) returns @StringT;
dec UtilsStringLength    : func(String:     @StringT                     ) returns Int32T;
dec UtilsStringToInt     : func(String:     @StringT                     ) returns Int32T;
dec UtilsStringFromInt   : func(String:     @StringT, Integer  : Int32T  ) returns @StringT;
dec UtilsStringCopy      : func(StringDest: @StringT, StringSrc: @StringT) returns @StringT;
dec UtilsStringConcatStr : func(StringDest: @StringT, StringSrc: @StringT) returns @StringT;
dec UtilsStringConcatDec : func(String:     @StringT, Integer  : Int32T  ) returns @StringT;
dec UtilsStringIsEqual   : func(String1:    @StringT, String2  : @StringT) returns BooleanT;

/* file functions */

dec UtilsFileOpen        : func(FileName:   @StringT , Mode: @StringT) returns FileDescT;
dec UtilsFileClose       : func(FileDesc:   FileDescT                ) returns Int32T;

dec UtilsFileRead        : func(Line    : @StringT , FileDesc: FileDescT) returns Int32T;
dec UtilsFileWrite       : func(FileDesc: FileDescT, Line:     @StringT ) returns Int32T;

/* console */

dec UtilsConsoleWriteStr : func(Line   : @StringT) returns byte;
dec UtilsConsoleWriteInt : func(Integer: Int32T  ) returns byte;

#endif /* __UTILS_INC */
