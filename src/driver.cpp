#include "driver.hpp"
#include "parser.hpp"

// Generazione di un'istanza per ciascuna della classi LLVMContext,
// Module e IRBuilder. Nel caso di singolo modulo è sufficiente
LLVMContext *context = new LLVMContext;                             // Contesto LLVM, contiene le informazioni globali
Module *module = new Module("Kaleidoscope", *context);              // Modulo LLVM contiene il codice sorgente
IRBuilder<> *builder = new IRBuilder(*context);                     // IRBuilder per la generazione del codice

// Funzione di utilità per la stampa degli errori
Value *LogErrorV(const std::string Str)
{
  std::cerr << Str << std::endl;
  return nullptr;
}

// Funzione di utilità per la creazione di un alloca nel blocco di ingresso
static AllocaInst *CreateEntryBlockAlloca(Function *fun, StringRef VarName, Type *T = nullptr)
{
  LLVMContext &Context = fun->getContext();
  IRBuilder<> TmpB(&fun->getEntryBlock(), fun->getEntryBlock().begin());

  // Se T non è fornito, usa il tipo double di default
  if (!T)
  {
    T = Type::getDoubleTy(Context);
  }

  return TmpB.CreateAlloca(T, nullptr, VarName);
}

/***************** Implementazione della classe driver *******************/

// Implementazione del costruttore della classe driver
driver::driver() : trace_parsing(false), trace_scanning(false){};

// Implementazione del metodo parse
int driver::parse(const std::string &f)
{
  file = f;                              // File con il programma
  location.initialize(&file);            // Inizializzazione dell'oggetto location
  scan_begin();                          // Inizio scanning (ovvero apertura del file programma)
  yy::parser parser(*this);              // Istanziazione del parser
  parser.set_debug_level(trace_parsing); // Livello di debug del parsed
  int res = parser.parse();              // Chiamata dell'entry point del parser
  scan_end();                            // Fine scanning (ovvero chiusura del file programma)
  return res;                            // Restituzione del risultato del parsing
}

// Implementazione del metodo codegen, che è una "semplice" chiamata del
// metodo omonimo presente nel nodo root (il puntatore root è stato scritto dal parser)
void driver::codegen()
{
  root->codegen(*this);
};

void driver::pushScope(ScopeContext::ContextType type)
{
  ScopeContext *parentScope = scopes.empty() ? nullptr : &scopes.back();
  if (scopes.empty() || shouldCreateNewScope(type))
  {
    scopes.emplace_back(type, parentScope);
    scopes.back().isActive = true;
  }
  else
  {
    scopes.emplace_back(type, parentScope);
    scopes.back().isActive = false;
  }
};

void driver::popScope()
{
  if (!scopes.empty() && scopes.back().isActive)
  {
    scopes.pop_back();
  }
  else if (!scopes.empty())
  {
    scopes.pop_back();
  }
};

bool driver::shouldCreateNewScope(ScopeContext::ContextType newType) const
{
  // Controlla se la lista degli scope è vuota.
  // Se è vuota, significa che non ci sono ambiti attivi e quindi è necessario crearne uno nuovo.
  if (scopes.empty())
  {
    return true;
  }

  // Ottiene un riferimento allo scope corrente, che è l'ultimo elemento dell'elenco degli scope.
  const auto &currentScope = scopes.back();

  // Itera attraverso il stack degli AST nodi dal più recente al più vecchio.
  // Questo viene fatto per controllare se ci sono istruzioni specifiche che richiedono la creazione di un nuovo scope.
  for (auto it = astNodeStack.rbegin(); it != astNodeStack.rend(); ++it)
  {
    // Usa 'dynamic_cast' per verificare se il nodo corrente è un'istruzione 'if' (IfStmtAST).
    // 'dynamic_cast' è usato qui per determinare il tipo effettivo del nodo AST.
    // Se il nodo corrente è un'istruzione 'if' e il nuovo tipo di scope è un blocco,
    // allora è necessario creare un nuovo scope. Questo potrebbe essere dovuto alla necessità
    // di isolare le variabili e le istruzioni all'interno dell'istruzione 'if'.
    if (dynamic_cast<IfStmtAST *>(*it) != nullptr && newType == ScopeContext::ContextType::Block)
    {
      return true;
    }
  }

  // Controlla se il nuovo tipo di scope è un blocco e lo scope corrente è una funzione o un ciclo 'for'.
  // In questo caso, non è necessario creare un nuovo ambito poiché il blocco all'interno di una funzione
  // o di un ciclo 'for' appartiene già a un ambito definito.
  if (newType == ScopeContext::ContextType::Block &&
      (currentScope.type == ScopeContext::ContextType::Function || currentScope.type == ScopeContext::ContextType::ForLoop))
  {
    return false;
  }
  return true;
};

// Metodo di utilità per la ricerca di una variabile locale
AllocaInst *driver::findLocalVar(const std::string &name, driver &drv)
{
  int scopeLevel = drv.scopes.size();

  // Itera sugli scope partendo dall'ultimo (scope corrente)
  for (auto it = drv.scopes.rbegin(); it != drv.scopes.rend(); ++it, --scopeLevel)
  {
    // Accedi alle variabili locali attraverso la struct ScopeContext
    for (const auto &pair : it->variables)
    {
      if (pair.first == name)
      {
        return pair.second;
      }
    }
  }
  return nullptr;
};

// Metodo di utilità per la ricerca di una variabile globale
GlobalVariable *driver::findGlobalVar(const std::string &name, driver &drv)
{
  auto GlobalVarIter = drv.GlobalValues.find(name);
  if (GlobalVarIter != drv.GlobalValues.end())
  {
    return GlobalVarIter->second;
  }
  else
  {
    return nullptr;
  }
};

// Metodo di utilità per la stampa di un messaggio di errore
void driver::printScope()
{
  if (!scopes.empty())
  {
    std::cerr << "Current scope: " << std::endl;
    const auto &currentScope = scopes.back();
    for (const auto &var : currentScope.variables)
    {
      std::cerr << "  " << var.first << " (AllocaInst* address: " << var.second << ")" << std::endl;
    }
  }
  else
  {
    std::cerr << "No current scope." << std::endl;
  }
};

// Metodo di utilità per la stampa di tutti gli scope
void driver::printAllScopes(driver &drv)
{
  std::cerr << "Printing all scopes:\n";

  int scopeLevel = drv.scopes.size();
  for (auto it = drv.scopes.rbegin(); it != drv.scopes.rend(); ++it, --scopeLevel)
  {
    std::cerr << "Scope level " << scopeLevel << " contains:\n";
    for (const auto &pair : it->variables)
    {
      std::cerr << "  Local variable: " << pair.first << " (AllocaInst* address: " << pair.second << ")" << std::endl;
    }
  }

  std::cerr << "Global scope contains:\n";
  for (const auto &pair : drv.GlobalValues)
  {
    std::cerr << "  Global variable: " << pair.first << " (GlobalVariable* address: " << pair.second << ")" << std::endl;
  }
};

void driver::getScopeNumber(driver &drv)
{
  std::cerr << "Current scope number: " << drv.scopes.size() << std::endl;
};




/************************* Sequence Tree **************************/
SeqAST::SeqAST(RootAST *first, RootAST *continuation) : first(first), continuation(continuation){};

Value *SeqAST::codegen(driver &drv)
{
  if (first != nullptr)
  {
    Value *f = first->codegen(drv);
  }
  else
  {
    if (continuation == nullptr)
      return nullptr;
  }
  Value *c = continuation->codegen(drv);
  return nullptr;
};


/********************* Number Expression Tree *********************/
NumberExprAST::NumberExprAST(double Val) : Val(Val){};

lexval NumberExprAST::getLexVal() const
{
  // Non utilizzata, Inserita per continuità con versione precedente
  lexval lval = Val;
  return lval;
};

// Non viene generata un'istruzione; soltanto una costante LLVM IR
// corrispondente al valore float memorizzato nel nodo
// La costante verrà utilizzata in altra parte del processo di generazione
// Si noti che l'uso del contesto garantisce l'unicità della costanti
Value *NumberExprAST::codegen(driver &drv)
{
  return ConstantFP::get(*context, APFloat(Val));
};

/******************** Variable Expression Tree ********************/
VariableExprAST::VariableExprAST(const std::string &Name) : Name(Name){};

lexval VariableExprAST::getLexVal() const
{
  lexval lval = Name;
  return lval;
};

Value *VariableExprAST::codegen(driver &drv)
{
  // Cerca la variabile nell'ambito locale
  AllocaInst *LocalVar = drv.findLocalVar(Name, drv);

  if (LocalVar)
  {
    // Se la variabile è locale, restituisci il suo valore, caricandolo dalla memoria
    return builder->CreateLoad(LocalVar->getAllocatedType(), LocalVar, Name.c_str());
  }
  else
  {
    // Cerca la variabile nell'ambito globale
    GlobalVariable *GlobalVar = drv.findGlobalVar(Name, drv);

    if (GlobalVar)
    { 
      // Se la variabile è globale, restituisci il suo valore, caricandolo dalla memoria
      return builder->CreateLoad(GlobalVar->getValueType(), GlobalVar, Name.c_str());
    }
    else
    {
      return LogErrorV("Variable " + Name + " not defined in expression");
    }
  }
};

/********************* Array Access Tree **********************/
ArrayAccessAST::ArrayAccessAST(const std::string &ArrayName, ExprAST *Index)
    : ArrayName(ArrayName), Index(Index) {}

Value *ArrayAccessAST::codegen(driver &drv)
{
  // Genera il codice per l'indice e verifica se è valido.
  Value *IndexVal = Index->codegen(drv);
  if (!IndexVal)
    return LogErrorV("Failed to generate index value");

  // Converte l'indice a float e poi a intero.
  IndexVal = builder->CreateFPTrunc(IndexVal, Type::getFloatTy(*context), "indexfloat");
  IndexVal = builder->CreateFPToUI(IndexVal, Type::getInt32Ty(*context), "indexint");

  // Cerca la variabile array nell'ambito globale o locale
  GlobalVariable *GlobalArray = drv.findGlobalVar(ArrayName, drv);
  AllocaInst *LocalArray = drv.findLocalVar(ArrayName, drv);

  if (!GlobalArray && !LocalArray) {
    return LogErrorV("Unknown array variable " + ArrayName);
  }

  // Prepara gli indici per l'accesso all'elemento dell'array.
  std::vector<Value *> Indices = {builder->getInt32(0), IndexVal};

  // Determina il tipo dell'array e crea un puntatore all'elemento.
  Value *ElementPtr = nullptr;
  Type *ArrayType = (GlobalArray) ? GlobalArray->getValueType() : LocalArray->getAllocatedType();

  ElementPtr = builder->CreateInBoundsGEP(ArrayType, 
                                          (GlobalArray) ? (Value*)GlobalArray : (Value*)LocalArray, 
                                          Indices, 
                                          "elementptr");

  if (!ElementPtr) {
    return LogErrorV("Error in creating array element pointer");
  }

  // Carica l'elemento dell'array.
  return builder->CreateLoad(ArrayType->getArrayElementType(), ElementPtr, "arrayelem");
};

/******************** Global Variable **********************/
GlobalVarAST::GlobalVarAST(const std::string &Name) : Name(Name){};

lexval GlobalVarAST::getLexVal() const
{
  lexval lval = Name;
  return lval;
};

std::string GlobalVarAST::getName() const
{
  return Name;
};

Value *GlobalVarAST::codegen(driver &drv)
{
  // Allocazione della variabile globale
  GlobalVariable *gvar = new GlobalVariable(*module, Type::getDoubleTy(*context), false,
                                            GlobalValue::CommonLinkage, nullptr, Name);
  // Inizializzazione della variabile globale
  gvar->setInitializer(ConstantFP::get(*context, APFloat(0.0)));

  // Inserimento della variabile globale nella symbol table
  drv.GlobalValues[Name] = gvar;

  // emitting code
  gvar->print(errs());
  fprintf(stderr, "\n");

  return gvar;
}

/**************************** Global Array ***********************************/
GlobalArrayVarAST::GlobalArrayVarAST(const std::string &Name, NumberExprAST *Size, std::vector<ExprAST *> InitValues)
    : GlobalVarAST(Name), Size(Size), InitValues(std::move(InitValues)){};

Value *GlobalArrayVarAST::codegen(driver &drv)
{
  // Calcolo della dimensione dell'array
  Value *ArraySizeVal = Size->codegen(drv);

  ArraySizeVal = builder->CreateFPTrunc(ArraySizeVal, Type::getFloatTy(*context), "indexfloat");
  ArraySizeVal = builder->CreateFPToUI(ArraySizeVal, Type::getInt32Ty(*context), "indexint");

  if (!ArraySizeVal)
  {
    LogErrorV("Error in array size conversion");
    return nullptr;
  }

  unsigned arraySize = 0;
  if (ConstantInt *CI = dyn_cast<ConstantInt>(ArraySizeVal))
  {
    arraySize = CI->getZExtValue();
  }
  else
  {
    LogErrorV("Error in array size");
    return nullptr;
  }

  ArrayType *ArrayType = ArrayType::get(Type::getDoubleTy(*context), arraySize);

  // Allocazione della variabile globale
  GlobalVariable *gvar = new GlobalVariable(*module, ArrayType, false,
                                            GlobalValue::CommonLinkage, nullptr, getName());

  // Preparazione dei valori iniziali
  std::vector<Constant *> constInitValues;
  for (auto &InitValue : InitValues)
  {
    constInitValues.push_back(static_cast<Constant *>(InitValue->codegen(drv)));
  }

  // Controllo del numero di valori iniziali
  // Se il numero di valori iniziali è minore della dimensione dell'array,
  // si aggiungono valori 0.0 alla fine dell'array
  // Se il numero di valori iniziali è maggiore della dimensione dell'array,
  // si genera un errore
  if (constInitValues.size() > arraySize)
  {
    LogErrorV("Too many initializers for array " + getName() +
              " (expected " + std::to_string(arraySize) + ", got " + std::to_string(constInitValues.size()) + ")");
    return nullptr;
  }
  else if (constInitValues.size() < arraySize)
  {
    constInitValues.resize(arraySize, ConstantFP::get(*context, APFloat(0.0)));
  }

  // Inizializzazione della variabile globale
  gvar->setInitializer(ConstantArray::get(ArrayType, constInitValues));

  // Inserimento della variabile globale nella symbol table
  drv.GlobalValues[getName()] = gvar;

  // Emissione del codice
  gvar->print(errs());
  fprintf(stderr, "\n");

  return gvar;
}

/********************* Assignment Tree **********************/
AssignmentAST::AssignmentAST(const std::string &Name, ExprAST *RHS, AssignmentType Type)
    : Name(Name), RHS(RHS), Type(Type) {}

std::string AssignmentAST::getName() const
{
  return Name;
};

ExprAST *AssignmentAST::getRHS() const
{
  return RHS;
};

AssignmentType AssignmentAST::getType() const
{
  return Type;
};

Value *AssignmentAST::codegen(driver &drv)
{
  AllocaInst *LocalVariable = nullptr;
  GlobalVariable *GlobalVariable = nullptr;

  LocalVariable = drv.findLocalVar(Name, drv);
  GlobalVariable = drv.findGlobalVar(Name, drv);

  Value *Val = RHS->codegen(drv);
  if (!Val)
    return nullptr;

  // Se l'assegnamento è di tipo post-increment o post-decrement, è necessario
  // caricare il valore della variabile prima di modificarlo
  // Se l'assegnamento è di tipo pre-increment o pre-decrement, è necessario
  // caricare il valore della variabile dopo averlo modificato
  // In tutti gli altri casi, non è necessario caricare il valore della variabile
  // prima di modificarlo
  if (LocalVariable)
  {
    Value *OriginalVal = builder->CreateLoad(LocalVariable->getAllocatedType(), LocalVariable, Name);
    if (Type == AssignmentType::PostIncrement || Type == AssignmentType::PostDecrement)
    {
      builder->CreateStore(Val, LocalVariable);
      return OriginalVal;
    }
    else
    {
      builder->CreateStore(Val, LocalVariable);
      return Val;
    }
  }
  else if (GlobalVariable)
  {
    Value *OriginalVal = builder->CreateLoad(GlobalVariable->getValueType(), GlobalVariable, Name);
    if (Type == AssignmentType::PostIncrement || Type == AssignmentType::PostDecrement)
    {
      builder->CreateStore(Val, GlobalVariable);
      return OriginalVal;
    }
    else
    {
      builder->CreateStore(Val, GlobalVariable);
      return Val;
    }
  }

  return nullptr;
};

/********************* Array Assignment Tree **********************/
ArrayAssignmentAST::ArrayAssignmentAST(const std::string &Name, ExprAST *Index, ExprAST *RHS)
    : AssignmentAST(Name, RHS, AssignmentType::Normal), Index(Index) {}

Value *ArrayAssignmentAST::codegen(driver &drv)
{
  // Genera il codice per l'indice dell'array.
  Value *IndexVal = Index->codegen(drv);
  if (!IndexVal)
    return LogErrorV("Failed to generate index value");

  // Genera il codice per il valore da assegnare.
  Value *RHSVal = getRHS()->codegen(drv);
  if (!RHSVal)
    return LogErrorV("Failed to generate RHS value");

  // Converti l'indice in float e poi in un intero a 32 bit.
  // Questo è necessario per assicurarsi che l'indice sia trattato come un numero intero.
  IndexVal = builder->CreateFPTrunc(IndexVal, Type::getFloatTy(*context), "indexfloat");
  IndexVal = builder->CreateFPToUI(IndexVal, Type::getInt32Ty(*context), "indexint");

  // Cerca la variabile array nell'ambito globale o locale.
  GlobalVariable *GlobalArray = drv.findGlobalVar(getName(), drv);
  AllocaInst *LocalArray = drv.findLocalVar(getName(), drv);

  // Verifica se la variabile array è stata trovata.
  if (!GlobalArray && !LocalArray) {
    return LogErrorV("Unknown array variable: " + getName());
  }

  // Preparazione degli indici per l'accesso all'elemento dell'array.
  std::vector<Value *> Indices = {builder->getInt32(0), IndexVal};

  // Crea un puntatore all'elemento dell'array.
  llvm::Type *ArrayType = (GlobalArray) ? GlobalArray->getValueType() : LocalArray->getAllocatedType();
  Value *ElementPtr = builder->CreateInBoundsGEP(ArrayType, 
                                                 (GlobalArray) ? (Value*)GlobalArray : (Value*)LocalArray, 
                                                 Indices, 
                                                 "elementptr");

  if (!ElementPtr) {
    return LogErrorV("Failed to create element pointer for array");
  }

  // Assegna il valore all'elemento dell'array e restituisci il valore assegnato.
  builder->CreateStore(RHSVal, ElementPtr);
  return RHSVal;
};


/******************** Binary Expression Tree **********************/
BinaryExprAST::BinaryExprAST(std::string Op, ExprAST *LHS, ExprAST *RHS) : Op(Op), LHS(LHS), RHS(RHS){};

Value *BinaryExprAST::codegen(driver &drv)
{

  Value *L = LHS->codegen(drv);
  Value *R = RHS->codegen(drv);
  if (!L || !R)
    return nullptr;

  if (Op == "+")
  {
    return builder->CreateFAdd(L, R, "addres");
  }
  else if (Op == "-")
  {
    return builder->CreateFSub(L, R, "subres");
  }
  else if (Op == "*")
  {
    return builder->CreateFMul(L, R, "mulres");
  }
  else if (Op == "/")
  {
    return builder->CreateFDiv(L, R, "divres");
  }
  else if (Op == "<")
  {
    return builder->CreateFCmpULT(L, R, "lttest");
  }
  else if (Op == ">")
  {
    return builder->CreateFCmpUGT(L, R, "gttest");
  }
  else if (Op == "==")
  {
    return builder->CreateFCmpUEQ(L, R, "eqtest");
  }
  else if (Op == "&&")
  {
    return builder->CreateLogicalAnd(L, R, "andres");
  }
  else if (Op == "||")
  {
    return builder->CreateLogicalOr(L, R, "orres");
  }
  else
  {
    std::cerr << Op << std::endl;
    return LogErrorV("Operatore binario non supportato");
  }
};

/********************* Unary Expression Tree **********************/
UnaryExprAST::UnaryExprAST(char Op, ExprAST *RHS) : Op(Op), RHS(RHS){};

Value *UnaryExprAST::codegen(driver &drv)
{
  Value *R = RHS->codegen(drv);
  if (!R)
    return nullptr;
  switch (Op)
  {
  case '-':
    return builder->CreateFNeg(R, "negres");
  case '!':
    return builder->CreateICmpEQ(R, llvm::Constant::getNullValue(R->getType()), "nottest");
  default:
    std::cerr << Op << std::endl;
    return LogErrorV("Operatore unario non supportato");
  }
};

/********************* Call Expression Tree ***********************/
CallExprAST::CallExprAST(std::string Callee, std::vector<ExprAST *> Args) : Callee(Callee), Args(std::move(Args)){};

lexval CallExprAST::getLexVal() const
{
  lexval lval = Callee;
  return lval;
};

Value *CallExprAST::codegen(driver &drv)
{ 
  Function *CalleeF = module->getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Funzione " + Callee + " non definita");

  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Numero di argomenti non corretto");

  std::vector<Value *> ArgsV;
  for (auto arg : Args)
  {
    ArgsV.push_back(arg->codegen(drv));
    if (!ArgsV.back())
      return nullptr;
  }
  return builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

/************************* If Expression Tree *************************/
IfExprAST::IfExprAST(ExprAST *Cond, ExprAST *TrueExp, ExprAST *FalseExp) : Cond(Cond), TrueExp(TrueExp), FalseExp(FalseExp){};

Value *IfExprAST::codegen(driver &drv)
{

  // Genera il codice per la condizione
  Value *CondV = Cond->codegen(drv);
  if (!CondV)
    return nullptr;

  // Ottieni la funzione corrente e crea tre blocchi base: vero, falso e fusione
  Function *function = builder->GetInsertBlock()->getParent();
  BasicBlock *TrueBB = BasicBlock::Create(*context, "trueexp", function);  // Blocco per il percorso vero 
  BasicBlock *FalseBB = BasicBlock::Create(*context, "falseexp");          // Blocco per il percorso falso
  BasicBlock *MergeBB = BasicBlock::Create(*context, "endcond");           // Blocco per unire i percorsi vero e falso

  // // Crea un branch basato sulla condizione
  builder->CreateCondBr(CondV, TrueBB, FalseBB);

  // Imposta il punto di inserimento nel blocco vero e genera il codice per l'espressione vera
  builder->SetInsertPoint(TrueBB);
  Value *TrueV = TrueExp->codegen(drv);
  if (!TrueV)
    return nullptr;

  builder->CreateBr(MergeBB);     // Salta al blocco di fusione dopo l'espressione vera

  // Aggiorna il blocco vero corrente e inserisce il blocco falso nella funzione
  TrueBB = builder->GetInsertBlock();
  function->insert(function->end(), FalseBB);

  builder->SetInsertPoint(FalseBB);

  // Imposta il punto di inserimento nel blocco falso e genera il codice per l'espressione falsa
  Value *FalseV = FalseExp->codegen(drv);
  if (!FalseV)
    return nullptr;

  builder->CreateBr(MergeBB);     // Salta al blocco di fusione dopo l'espressione falsa

  // Aggiorna il blocco falso corrente e inserisce il blocco di fusione nella funzione
  FalseBB = builder->GetInsertBlock();
  function->insert(function->end(), MergeBB);   

  builder->SetInsertPoint(MergeBB); // Imposta il punto di inserimento nel blocco di fusione

  // Crea un PHINode per il valore di ritorno e aggiungi i valori di ritorno per i blocchi vero e falso
  // Il PHINode restituirà il valore appropriato in base al percorso di esecuzione
  PHINode *PN = builder->CreatePHI(Type::getDoubleTy(*context), 2, "condval");
  PN->addIncoming(TrueV, TrueBB);
  PN->addIncoming(FalseV, FalseBB);
  return PN;
};

/************************* If Statement Tree *************************/
IfStmtAST::IfStmtAST(ExprAST *Cond, RootAST *ThenBody, RootAST *ElseBody) : Cond(Cond), ThenBody(ThenBody), ElseBody(ElseBody){};

Value *IfStmtAST::codegen(driver &drv)
{
  drv.astNodeStack.push_back(this);

  Value *CondV = Cond->codegen(drv);
  if (!CondV)
  {
    drv.astNodeStack.pop_back();
    return nullptr;
  }

  Function *TheFunction = builder->GetInsertBlock()->getParent();

  // Crea i blocchi per il percorso then, else e di fusione
  // Il blocco else è opzionale
  BasicBlock *ThenBB = BasicBlock::Create(*context, "then", TheFunction);
  BasicBlock *ElseBB = ElseBody ? BasicBlock::Create(*context, "else") : nullptr;
  BasicBlock *MergeBB = BasicBlock::Create(*context, "ifcont");

  builder->CreateCondBr(CondV, ThenBB, ElseBB ? ElseBB : MergeBB);

  builder->SetInsertPoint(ThenBB);

  Value *ThenV = ThenBody->codegen(drv);
  if (!ThenV)
  {
    drv.astNodeStack.pop_back();
    return nullptr;
  }

  builder->CreateBr(MergeBB);
  ThenBB = builder->GetInsertBlock();

  Value *ElseV = nullptr;
  if (ElseBody)
  {
    TheFunction->insert(TheFunction->end(), ElseBB);
    builder->SetInsertPoint(ElseBB);

    ElseV = ElseBody->codegen(drv);
    if (!ElseV)
    {
      drv.astNodeStack.pop_back();
      return nullptr;
    }
    builder->CreateBr(MergeBB);
    ElseBB = builder->GetInsertBlock();
  }

  TheFunction->insert(TheFunction->end(), MergeBB);
  builder->SetInsertPoint(MergeBB);
  if (ElseBody)
  {
    PHINode *PN = builder->CreatePHI(Type::getDoubleTy(*context), 2, "iftmp");

    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);

    drv.astNodeStack.pop_back();
    return PN;
  }
  else
  {
    drv.astNodeStack.pop_back();
    return ThenV;
  }
}

ForStmtAST::ForStmtAST(RootAST *Init, ExprAST *Cond, RootAST *Step, RootAST *Body) : Init(Init), Cond(Cond), Step(Step), Body(Body){};

Value *ForStmtAST::codegen(driver &drv)
{
  drv.astNodeStack.push_back(this);
  drv.pushScope(driver::ScopeContext::ContextType::ForLoop);

  // Valuta l'espressione di inizializzazione del ciclo, se presente, altrimenti restituisce nullptr
  Value *initVal = Init ? Init->codegen(drv) : nullptr;
  if (!initVal)
  {
    drv.popScope();
    drv.astNodeStack.pop_back();
    return LogErrorV("Error in loop initialization");
  }

  // Se il valore iniziale è un puntatore, lo dereferenzia e lo carica in un valore double
  if (initVal->getType()->isPointerTy())
  {
    initVal = builder->CreateLoad(Type::getDoubleTy(*context), initVal, "loadInitVal");
  }

  // Ottiene la funzione corrente in cui inserire il codice del ciclo
  Function *theFunction = builder->GetInsertBlock()->getParent();
  // Ottiene il blocco di codice precedente all'inizio del ciclo
  BasicBlock *preHeaderBB = builder->GetInsertBlock(); 
  // Crea un nuovo blocco di codice per il corpo del ciclo e lo aggiunge alla funzione
  BasicBlock *loopBB = BasicBlock::Create(*context, "loop", theFunction);

  builder->CreateBr(loopBB);        // Crea un salto incondizionato dal blocco precedente al blocco del ciclo
  builder->SetInsertPoint(loopBB);  // Imposta il punto di inserimento nel blocco del ciclo

  // Crea un nodo PHI per gestire il valore della variabile di controllo del ciclo
  PHINode *phiNode = builder->CreatePHI(Type::getDoubleTy(*context), 2, "loopVar");

  // Aggiunge il valore iniziale come ingresso del nodo PHI proveniente dal blocco precedente
  phiNode->addIncoming(initVal, preHeaderBB);

  if (!Body->codegen(drv))
  {
    drv.popScope();
    drv.astNodeStack.pop_back();
    return LogErrorV("Error in loop body");
  }

  // Valuta l'espressione di incremento del ciclo, se presente, altrimenti restituisce 1.0
  Value *stepVal = Step ? Step->codegen(drv) : llvm::ConstantFP::get(*context, llvm::APFloat(1.0));
  if (!stepVal)
  {
    drv.popScope();
    drv.astNodeStack.pop_back();
    return LogErrorV("Error in loop step");
  }

  // Valuta l'espressione di condizione del ciclo, se fallisce restituisce un errore
  Value *endCond = Cond->codegen(drv);
  if (!endCond)
  {
    drv.popScope();
    drv.astNodeStack.pop_back();
    return LogErrorV("Error in loop condition");
  }

  // Crea un confronto tra la condizione e il valore nullo, restituisce vero se sono diversi
  endCond = builder->CreateICmpNE(endCond, llvm::Constant::getNullValue(endCond->getType()), "loopCond");

  BasicBlock *loopEndBB = builder->GetInsertBlock();  // Ottiene il blocco di codice finale del ciclo
  BasicBlock *afterBB = BasicBlock::Create(*context, "afterLoop", theFunction); // Crea un nuovo blocco di codice dopo il ciclo

  // Crea un salto condizionato dal blocco finale al blocco del ciclo o al blocco successivo, in base alla condizione
  builder->CreateCondBr(endCond, loopBB, afterBB);

  // Imposta il punto di inserimento nel blocco successivo
  builder->SetInsertPoint(afterBB);
  phiNode->addIncoming(stepVal, loopEndBB); // Aggiunge il valore di incremento come ingresso del nodo PHI proveniente dal blocco finale

  // Restituisce un valore nullo come risultato dell'istruzione for
  drv.popScope();
  return llvm::Constant::getNullValue(Type::getDoubleTy(*context));
};

/******************** BlockExprTree  *******************/
BlockStmtAST::BlockStmtAST(std::vector<VarBindingAST *> VarBindings,
                           std::vector<RootAST *> Statements)
    : VarBindings(std::move(VarBindings)), Statements(std::move(Statements)) {}

// Distruttore per la pulizia della memoria
BlockStmtAST::~BlockStmtAST()
{
  for (auto &binding : VarBindings)
  {
    delete binding;
  }
  for (auto &stmt : Statements)
  {
    delete stmt;
  }
}

Value *BlockStmtAST::codegen(driver &drv)
{
  drv.astNodeStack.push_back(this);
  drv.pushScope(driver::ScopeContext::ContextType::Block);

  // Ciclo che itera su tutte le dichiarazioni di variabili nel blocco.
  for (auto &varBinding : VarBindings)
  {
    // Chiama il metodo codegen per ogni dichiarazione di variabile.
    // Questo genera il codice per l'allocazione di memoria per la variabile.
    AllocaInst *allocaInst = varBinding->codegen(drv);
    if (!allocaInst)
    {
      drv.popScope();
      LogErrorV("Error in variable binding");
      drv.astNodeStack.pop_back();
      return nullptr;
    }

    // Ottiene il nome della variabile.
    const std::string &varName = varBinding->getName();

    // Verifica se la variabile locale fa shadowing a una variabile globale.
    if (drv.GlobalValues.find(varName) != drv.GlobalValues.end())
    {
      // Se esiste una variabile globale con lo stesso nome, restituisce un errore.
      drv.popScope();
      drv.astNodeStack.pop_back();
      return LogErrorV("Local variable " + varName + " shadows a global variable");
    }
  }

  // Inizializza lastValue a nullptr. Questa variabile terrà l'ultimo valore calcolato nel blocco.
  Value *lastValue = nullptr;

  for (auto &stmt : Statements)
  {
    // Chiama il metodo codegen per ogni istruzione nel blocco.
    lastValue = stmt->codegen(drv);
    if (!lastValue)
    {
      drv.popScope();
      drv.astNodeStack.pop_back();
      return nullptr;
    }
  }

  drv.popScope();
  drv.astNodeStack.pop_back();

  return lastValue;
};

/************************* Var binding Tree *************************/
VarBindingAST::VarBindingAST(const std::string Name, ExprAST *Val) : Name(Name), Val(Val){};

const std::string &VarBindingAST::getName() const
{
  return Name;
};

AllocaInst *VarBindingAST::codegen(driver &drv)
{
  Function *fun = builder->GetInsertBlock()->getParent();

  // Val potrebbe essere nullptr se la variabile è dichiarata ma non inizializzata
  Value *BoundVal = nullptr;
  if (Val)
  {
    BoundVal = Val->codegen(drv);
    if (!BoundVal) // Qualcosa è andato storto nella generazione del codice per il valore, restituisce nullptr
      return nullptr;
  }

  // Si genera l'istruzione che alloca memoria per la variabile
  AllocaInst *Alloca = CreateEntryBlockAlloca(fun, Name, Type::getDoubleTy(*context));

  // Se Val non è nullptr, si memorizza il suo valore nella memoria allocata
  if (BoundVal)
  {
    builder->CreateStore(BoundVal, Alloca);
  }

  // Aggiunge la variabile alla symbol table
  drv.scopes.back().variables[Name] = Alloca;

  return Alloca;
};

/*************************  Array binding Tree *************************/
ArrayBindingAST::ArrayBindingAST(const std::string &Name, NumberExprAST *Size, std::vector<ExprAST *> InitValues)
    : VarBindingAST(Name, nullptr), Size(Size), InitValues(std::move(InitValues)) {}

AllocaInst *ArrayBindingAST::codegen(driver &drv)
{
  // Ottiene la funzione corrente
  Function *fun = builder->GetInsertBlock()->getParent();

  Value *ArraySizeVal = Size->codegen(drv);     // Genera il codice per la dimensione dell'array
  if(!ArraySizeVal)
    return nullptr;

  // Inizializza ArraySizeVal a nullptr. Questa variabile terrà la dimensione dell'array.
  Value *ArrSize = nullptr;

  // Conversione della dimensione dell'array a float e poi a intero  
  ArraySizeVal = builder->CreateFPTrunc(ArraySizeVal, Type::getFloatTy(*context), "indexfloat");
  ArraySizeVal = builder->CreateFPToUI(ArraySizeVal, Type::getInt32Ty(*context), "indexint");

  if (!ArraySizeVal)
  {
    LogErrorV("Error in array size conversion");
    return nullptr;
  }

  // Dichiara arraySize come unsigned int
  unsigned arraySize;

  // Si prende la dimensione dell'array come valore costante
  if (ConstantInt *CI = dyn_cast<ConstantInt>(ArrSize))
  {   
    // Si converte la dimensione dell'array a unsigned int
    arraySize = CI->getZExtValue();
  }
  else
  {
    // Se la dimensione dell'array non è un valore costante, si restituisce un errore
    LogErrorV("Error in array size");
    return nullptr;
  }

  // Crea il tipo dell'array
  ArrayType *ArrayType = ArrayType::get(Type::getDoubleTy(*context), arraySize);

  // Alloca l'array nel blocco di ingresso
  AllocaInst *ArrayAlloca = CreateEntryBlockAlloca(fun, getName(), ArrayType);

  // Opzionalmente, inizializza l'array con i valori forniti
  if (!InitValues.empty())
  {
    for (size_t i = 0; i < InitValues.size(); ++i)
    {
      // Genera il codice per il valore di inizializzazione
      Value *InitVal = InitValues[i]->codegen(drv);
      if (!InitVal)
      {
        LogErrorV("Error in array initialization");
        return nullptr; // Fallimento nella generazione del codice per un valore di inizializzazione
      }

      // Calcola l'indirizzo dell'i-esimo elemento dell'array
      Value *Idx[] = {builder->getInt32(0), builder->getInt32(i)};
      Value *ElemPtr = builder->CreateInBoundsGEP(ArrayType, ArrayAlloca, Idx, "elementptr");

      if (!ElemPtr)
      {
        LogErrorV("Error in array element address" + std::to_string(i));
        return nullptr; // Fallimento nella generazione del codice per l'indirizzo dell'elemento
      }

      // Assegna il valore di inizializzazione all'elemento
      builder->CreateStore(InitVal, ElemPtr);
    }
  }

  // Aggiungi l'allocazione dell'array alla symbol table
  drv.scopes.back().variables[getName()] = ArrayAlloca;

  return ArrayAlloca;
}

/************************* Prototype Tree *************************/
PrototypeAST::PrototypeAST(std::string Name, std::vector<std::string> Args) : Name(Name), Args(std::move(Args)), emitcode(true){}; // Di regola il codice viene emesso

lexval PrototypeAST::getLexVal() const
{
  lexval lval = Name;
  return lval;
};

const std::vector<std::string> &PrototypeAST::getArgs() const
{
  return Args;
};

// Previene la doppia emissione del codice.
void PrototypeAST::noemit()
{
  emitcode = false;
};

Function *PrototypeAST::codegen(driver &drv)
{
  // Genera il tipo della funzione, nel caso del nostro programma
  // tutte le funzioni hanno tipo double, che non viene specificato nella dichiarazione
  // ma viene solo aggiunto nella generazione del codice
  std::vector<Type *> Doubles(Args.size(), Type::getDoubleTy(*context));
  FunctionType *FT = FunctionType::get(Type::getDoubleTy(*context), Doubles, false);

  Function *F = Function::Create(FT, Function::ExternalLinkage, Name, *module); // Crea la funzione nel modulo

  // Imposta i nomi degli argomenti
  unsigned Idx = 0;
  for (auto &Arg : F->args())
  {
    Arg.setName(Args[Idx]);
    Idx++;
  }

  if (emitcode)
  {
    F->print(errs());
    fprintf(stderr, "\n");
  }

  return F;
};

/************************* Function Tree **************************/
FunctionAST::FunctionAST(PrototypeAST *Proto, BlockStmtAST *Body) : Proto(Proto), Body(Body){};

Function *FunctionAST::codegen(driver &drv)
{
  drv.astNodeStack.push_back(this);

  // Genera il codice per il prototipo della funzione

  // Si ottiene il nome della funzione
  std::string functionName = std::get<std::string>(Proto->getLexVal());

  // Si cerca la funzione nel modulo
  Function *function = module->getFunction(functionName);

  if (!function)
  { 
    // Se la funzione non è stata trovata, si genera il codice per il prototipo
    function = Proto->codegen(drv);
    if (!function)
    {
      // Se la generazione del codice per il prototipo fallisce, si restituisce nullptr
      drv.astNodeStack.pop_back();
      return nullptr;
    }
  }
  else if (!function->empty())
  {   
    // Se la funzione è già stata definita, si restituisce un errore
    drv.astNodeStack.pop_back();
    return (Function *)LogErrorV("Function " + functionName + " cannot be redefined");
  }

  // Si crea un nuovo blocco base per la funzione
  BasicBlock *BB = BasicBlock::Create(*context, "entry", function);

  builder->SetInsertPoint(BB); // Si imposta il punto di inserimento nel blocco appena creato

  // Si crea un nuovo scope per la funzione
  drv.pushScope(driver::ScopeContext::ContextType::Function);

  // Si aggiungono gli argomenti della funzione allo scope
  for (auto &Arg : function->args())
  {
    AllocaInst *Alloca = CreateEntryBlockAlloca(function, Arg.getName(), Arg.getType());
    builder->CreateStore(&Arg, Alloca);

    drv.scopes.back().variables[std::string(Arg.getName())] = Alloca;
  }
  
  if (Value *RetVal = Body->codegen(drv))
  {
    // Se il corpo della funzione genera un valore, si aggiunge un return statement
    builder->CreateRet(RetVal);

    verifyFunction(*function); // Verifica la correttezza della funzione

    function->print(errs()); // Emissione del codice
    fprintf(stderr, "\n");

    // Successo, si esce dallo scope della funzione
    drv.popScope();
    drv.astNodeStack.pop_back();

    return function; // Si restituisce la funzione
  }

  // Se la generazione del codice per il corpo della funzione fallisce, si cancella la funzione
  function->eraseFromParent();

  drv.popScope();
  drv.astNodeStack.pop_back();

  return nullptr;
};