/**
 * Implementation of SymbolTable.h
 */

#include "SymbolTable.h"

/* =========== PRIVATE ============= */

bool SymbolTable::isInClassTable(const std::string& name) const {
    if (classTable.find(name) != classTable.end()) return true;
    else return false;
}

bool SymbolTable::isInSubroutineTable(const std::string& name) const {
    if (subroutineTable.find(name) != subroutineTable.end()) return true;
    else return false;
}

/* =========== PUBLIC ============= */

SymbolTable::SymbolTable() {

}

SymbolTable::~SymbolTable() {

}

void SymbolTable::resetTable() {
    static_index_ = 0;
    field_index_ = 0;
    argument_index_ = 0;
    var_index_ = 0;

    classTable.clear();
    subroutineTable.clear();
}

void SymbolTable::startSubroutine() {
    argument_index_ = 0;
    var_index_ = 0;
    subroutineTable.clear();
}

void SymbolTable::define(const std::string& name, const std::string& type, const VarKind& kind) {
    switch (kind) {
    case VarKind::STATIC:
        classTable.insert({name, {type, kind, static_index_++}});
        break;
    case VarKind::FIELD:
        classTable.insert({name, {type, kind, field_index_++}});
        break;
    case VarKind::ARGUMENT:
        subroutineTable.insert({name, {type, kind, argument_index_++}});
        break;
    case VarKind::VAR:
        subroutineTable.insert({name, {type, kind, var_index_++}});
        break;
    default:
        throw compile_exception("Invalid identifier kind.");
    }
}

int SymbolTable::varCount(const VarKind& kind) const {
    switch (kind) {
    case VarKind::STATIC:
        return static_index_;
    case VarKind::FIELD:
        return field_index_;
    case VarKind::ARGUMENT:
        return argument_index_;
    case VarKind::VAR:
        return var_index_;
    default:
        throw compile_exception("Invalid identifier kind.");
    }
}

std::string SymbolTable::typeOf(const std::string& name) const {
    if (isInSubroutineTable(name)) return std::get<0>(subroutineTable.at(name)); 
    if (isInClassTable(name)) return std::get<0>(classTable.at(name));
    throw compile_exception(name + " doesn't exist.");
}

VarKind SymbolTable::kindOf(const std::string& name) const {
    if (isInSubroutineTable(name)) return std::get<1>(subroutineTable.at(name));
    if (isInClassTable(name)) return std::get<1>(classTable.at(name));
    return VarKind::NONE;
}

int SymbolTable::indexOf(const std::string& name) const {
    if (isInSubroutineTable(name)) return std::get<2>(subroutineTable.at(name)); 
    if (isInClassTable(name)) return std::get<2>(classTable.at(name));
    throw compile_exception(name + " doesn't exist.");
}