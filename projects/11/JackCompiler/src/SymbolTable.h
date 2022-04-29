/**
 * Symbol Table Module(Class)
 * It provides the ability to generate and utilize symbol tables.
 * It is used to classify identifiers.
 *
 * Interface
 * - resetTable: Reset all symbol tables.
 * - startSubroutine: Reset the symbol table for subroutine.
 * - define: Define a new identifier.
 * - varCount: Returns the number of variables according to a given type.
 * - kindOf: Returns the kind of identifier as the variable name.
 * - typeOf:Returns the type of identifier as the variable name.
 * - indexOf: Returns the index assigned to the identifier by the variable name.
 *
 */

#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "Global.h"

class SymbolTable {
private:

public:
    SymbolTable();
    ~SymbolTable();

    void resetTable();
    void startSubroutine();
    void define(const std::string& name, const std::string& type, const VarKind& kind);
    int varCount(const VarKind& kind) const;
    VarKind kindOf(const std::string& name) const;
    std::string typeOf(const std::string& name) const;
    int indexOf(const std::string* name) const;
};

#endif