#ifndef __SYMTAB_INC
#define __SYMTAB_INC

/* utility library */
#include <Utils.inc>

/* AST definitions */
#include <AST.inc>

/* symbol table */
typ SymbolT: record
(
    Next  : @SymbolT,
    Name  : @StringT,
    Type  : @ASTTypeNodeT,
    Loc   : @StringT,
    Scope : Int32T,
    IsDec : BooleanT,
    IsTyp : BooleanT
);

/* public functions */
dec SymtabIncScope: func();
dec SymtabDecScope: func();
dec SymtabGetScope: func() returns Int32T;
dec SymtabGetSym  : func(Name: @StringT) returns @SymbolT;
dec SymtabAddSym  : func(Name: @StringT, Type: @ASTTypeNodeT, Loc: @StringT);

/* Init functions */
dec SymtabInit   : func();
dec SymtabCleanup: func();

#endif /* __SYMTAB_INC */
