#ifndef DRIVER_HPP
#define DRIVER_HPP
/************************* IR related modules ******************************/
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <llvm/IR/GlobalVariable.h>
#include "llvm/Support/raw_ostream.h"
/**************** C++ modules and generic data types ***********************/
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <stack>

#include "parser.hpp"

using namespace llvm;

// Dichiarazione del prototipo yylex per Flex
// Flex va proprio a cercare YY_DECL perché
// deve espanderla (usando M4) nel punto appropriato
#define YY_DECL \
  yy::parser::symbol_type yylex(driver &drv)
// Per il parser è sufficiente una forward declaration
YY_DECL;

// Classe 'driver': Gestisce il processo di compilazione
class driver
{
public:
  // Costruttore della classe.
  driver();

  // Struct 'ScopeContext': Rappresenta un contesto (o scope) nel processo di compilazione.
  struct ScopeContext
  {
    // Mappa per tenere traccia delle variabili definite in questo scope.
    std::unordered_map<std::string, llvm::AllocaInst *> variables;

    // Enum 'ContextType': Identifica il tipo di contesto (funzione, blocco, ecc.).
    enum class ContextType
    {
      Function,
      Block,
      ForLoop,
      IfStatement,
      Other
    } type;

    // Puntatore allo scope genitore: consente di creare una gerarchia di scope.
    ScopeContext *parent;

    // Flag per indicare se lo scope è attivo o meno.
    bool isActive;

    // Costruttore di 'ScopeContext': inizializza lo scope con un tipo, genitore, e stato.
    ScopeContext(ContextType type, ScopeContext *parent = nullptr, bool isActive = true)
        : type(type), parent(parent), isActive(isActive) {}
  };

  // Mappa per le variabili globali, accessibili in tutto il processo di compilazione.
  std::map<std::string, llvm::GlobalVariable *> GlobalValues;

  // Vettore degli scope attualmente esistenti.
  std::vector<ScopeContext> scopes;

  // Puntatore alla radice dell'AST
  RootAST *root;

  // Stack dei nodi AST: utilizzato per tenere traccia del nodo corrente durante il parsing. Parte della gestione degli scope.
  std::vector<RootAST *> astNodeStack;


  int parse(const std::string &f);    // Funzione per avviare il parsing
  std::string file;                   // Nome del file sorgente
  bool trace_parsing;                 // Flag per attivare il tracing del parsing
  void scan_begin();                  // Funzione per inizializzare Flex
  void scan_end();                    // Funzione per terminare Flex
  bool trace_scanning;                // Flag per attivare il tracing della scansione
  yy::location location;              // Oggetto per tenere traccia della posizione corrente nel file sorgente
  void codegen();                     // Funzione per avviare la generazione del codice dell'AST

  void pushScope(ScopeContext::ContextType type);                             // Funzione per creare uno scope
  void popScope();                                                            // Funzione per eliminare lo scope corrente           
  bool shouldCreateNewScope(ScopeContext::ContextType newType) const;         // Funzione per verificare se è necessario creare uno scope
  void printScope();                                                          // Funzione per stampare lo scope corrente
  static AllocaInst *findLocalVar(const std::string &name, driver &drv);      // Funzione per cercare una variabile locale
  static GlobalVariable *findGlobalVar(const std::string &name, driver &drv); // Funzione per cercare una variabile globale
  static void printAllScopes(driver &drv);                                    // Funzione per stampare tutti gli scope
  static void getScopeNumber(driver &drv);                                    // Funzione per stampare il numero dello scope corrente           
};

typedef std::variant<std::string, double> lexval;                             // Tipo per il valore di un token
const lexval NONE = 0.0;                                                      // Costante per indicare un valore non valido

// RootAST - Classe base per tutti i nodi dell'AST
class RootAST
{
public:
  virtual ~RootAST(){};
  virtual lexval getLexVal() const { return NONE; };
  virtual Value *codegen(driver &drv) { return nullptr; };
};

// SeqAST - Classe che rappresenta la sequenza di statement
class SeqAST : public RootAST
{
private:
  RootAST *first;
  RootAST *continuation;

public:
  SeqAST(RootAST *first, RootAST *continuation);
  Value *codegen(driver &drv) override;
};

// ExprAST - Classe base per tutti i nodi espressione
class ExprAST : public RootAST
{
};

// NumberExprAST - Classe per la rappresentazione di costanti numeriche
class NumberExprAST : public ExprAST
{
private:
  double Val;

public:
  NumberExprAST(double Val);
  lexval getLexVal() const override;
  Value *codegen(driver &drv) override;
};

// VariableExprAST - Classe per la rappresentazione di riferimenti a variabili
class VariableExprAST : public ExprAST
{
private:
  std::string Name;

public:
  VariableExprAST(const std::string &Name);
  lexval getLexVal() const override;
  Value *codegen(driver &drv) override;
};

/// ArrayAccessAST - Classe per la rappresentazione di accessi ad array
class ArrayAccessAST : public ExprAST
{
private:
  std::string ArrayName;
  ExprAST *Index;

public:
  ArrayAccessAST(const std::string &ArrayName, ExprAST *Index);
  Value *codegen(driver &drv) override;
};

/// GlobalVarAST - Classe per la rappresentazione di variabili globali
class GlobalVarAST : public RootAST
{
private:
  std::string Name;

public:
  GlobalVarAST(const std::string &Name);
  lexval getLexVal() const override;
  Value *codegen(driver &drv) override;
  std::string getName() const;
};

/// GlobalArrayVarAST - Classe per la rappresentazione di array globali
class GlobalArrayVarAST : public GlobalVarAST
{
private:
  NumberExprAST *Size; // Dimensione dell'array
  std::vector<ExprAST *> InitValues;

public:
  GlobalArrayVarAST(const std::string &Name, NumberExprAST *Size, std::vector<ExprAST *> InitValues = {});
  Value *codegen(driver &drv) override;
};

/// AssignmentType - Enum per i tipi di assegnamento
enum class AssignmentType
{
  Normal,
  PreIncrement,
  PreDecrement,
  PostIncrement,
  PostDecrement
};
/// Classe AssignmentAST - Rappresenta un assegnamento
class AssignmentAST : public RootAST
{
private:
  std::string Name;
  ExprAST *RHS;
  AssignmentType Type;

public:
  AssignmentAST(const std::string &Name, ExprAST *RHS, AssignmentType Type);
  Value *codegen(driver &drv) override;
  std::string getName() const;
  ExprAST *getRHS() const;
  AssignmentType getType() const;
};

/// ArrayAssignmentAST - Classe per la rappresentazione di assegnamenti ad array
class ArrayAssignmentAST : public AssignmentAST
{
private:
  ExprAST *Index;

public:
  ArrayAssignmentAST(const std::string &Name, ExprAST *Index, ExprAST *RHS);
  Value *codegen(driver &drv) override;
};

/// BinaryExprAST - Classe per la rappresentazione di operatori binari
class BinaryExprAST : public ExprAST
{
private:
  std::string Op;
  ExprAST *LHS;
  ExprAST *RHS;

public:
  BinaryExprAST(std::string Op, ExprAST *LHS, ExprAST *RHS);
  Value *codegen(driver &drv) override;
};


/// UnaryExprAST - Classe per la rappresentazione di operatori unari
class UnaryExprAST : public ExprAST
{
  char Op;
  ExprAST *RHS;

public:
  UnaryExprAST(char Op, ExprAST *RHS);
  Value *codegen(driver &drv) override;
};

/// CallExprAST - Classe per la rappresentazione di chiamate di funzione
class CallExprAST : public ExprAST
{
private:
  std::string Callee;
  std::vector<ExprAST *> Args;

public:
  CallExprAST(std::string Callee, std::vector<ExprAST *> Args);
  lexval getLexVal() const override;
  Value *codegen(driver &drv) override;
};

/// IfExprAST - Classe per la rappresentazione dell'operatore ternario
class IfExprAST : public ExprAST
{
private:
  ExprAST *Cond;
  ExprAST *TrueExp;
  ExprAST *FalseExp;

public:
  IfExprAST(ExprAST *Cond, ExprAST *TrueExp, ExprAST *FalseExp);
  Value *codegen(driver &drv) override;
};

/// IfStmtAST - Classe per la rappresentazione di if-then-(else)
class IfStmtAST : public RootAST
{
private:
  ExprAST *Cond;
  RootAST *ThenBody;
  RootAST *ElseBody;

public:
  IfStmtAST(ExprAST *Cond, RootAST *ThenBody, RootAST *ElseBody);
  Value *codegen(driver &drv) override;
};

/// ForStmtAST - Classe per la rappresentazione di cicli for
class ForStmtAST : public RootAST
{
private:
  RootAST *Init;
  ExprAST *Cond;
  RootAST *Step;
  RootAST *Body;

public:
  ForStmtAST(RootAST *Init, ExprAST *Cond, RootAST *Step, RootAST *Body);
  Value *codegen(driver &drv) override;
};

/// BlockStmtAST - Classe per la rappresentazione di blocchi di statement
class BlockStmtAST : public RootAST
{
private:
  std::vector<VarBindingAST *> VarBindings;
  std::vector<RootAST *> Statements;

public:
  BlockStmtAST(std::vector<VarBindingAST *> VarBindings,
               std::vector<RootAST *> Statements);
  ~BlockStmtAST();

  virtual Value *codegen(driver &drv) override;
};

/// VarBindingAST - Classe per la rappresentazione di binding di variabili
class VarBindingAST : public RootAST
{
private:
  const std::string Name;
  ExprAST *Val;

public:
  VarBindingAST(const std::string Name, ExprAST *Val);
  AllocaInst *codegen(driver &drv) override;
  const std::string &getName() const;
};

/// ArrayBindingAST - Classe per la rappresentazione di binding di array
class ArrayBindingAST : public VarBindingAST
{
private:
  NumberExprAST *Size;               // Dimensione dell'array
  std::vector<ExprAST *> InitValues; // Valori di inizializzazione opzionali

public:
  ArrayBindingAST(const std::string &Name, NumberExprAST *Size, std::vector<ExprAST *> InitValues = {});
  AllocaInst *codegen(driver &drv) override;
};

/// PrototypeAST - Classe per la rappresentazione dei prototipi di funzione
/// (nome, numero e nome dei parametri; in questo caso il tipo è implicito
/// perché unico)
class PrototypeAST : public RootAST
{
private:
  std::string Name;
  std::vector<std::string> Args;
  bool emitcode;

public:
  PrototypeAST(std::string Name, std::vector<std::string> Args);
  const std::vector<std::string> &getArgs() const;
  lexval getLexVal() const override;
  Function *codegen(driver &drv) override;
  void noemit();
};

/// FunctionAST - Classe che rappresenta la definizione di una funzione
class FunctionAST : public RootAST
{
private:
  PrototypeAST *Proto;
  BlockStmtAST *Body;
  bool external;

public:
  FunctionAST(PrototypeAST *Proto, BlockStmtAST *Body);
  Function *codegen(driver &drv) override;
};

#endif // ! DRIVER_HH
