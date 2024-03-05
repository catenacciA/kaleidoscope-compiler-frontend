%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines

%define api.token.constructor
%define api.location.file none
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  # include <exception>
  class driver;
  class RootAST;
  class ExprAST;
  class NumberExprAST;
  class VariableExprAST;
  class CallExprAST;
  class FunctionAST;
  class SeqAST;
  class PrototypeAST;
  class VarBindingAST;
  class IfExprAST;

  class BlockStmtAST;
  class AssignmentAST;
  class GlobalVarAST;
  class IfStmtAST;
  class ForStmtAST;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hpp"
}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  SEMICOLON  ";"
  COMMA      ","
  MINUS      "-"
  PLUS       "+"
  INCREMENT  "++"
  DECREMENT  "--"
  STAR       "*"
  SLASH      "/"
  LPAREN     "("
  RPAREN     ")"
  QMARK	     "?"
  COLON      ":"
  LT         "<"
  GT         ">"
  EQ         "=="
  AND       "and"
  OR        "or"
  NOT       "not"
  ASSIGN     "="
  LBRACE     "{"
  RBRACE     "}"
  LBRACKET   "["
  RBRACKET   "]"
  EXTERN     "extern"
  DEF        "def"
  VAR        "var"
  GLOBAL     "global"
  IF         "if"
  THEN       "then"
  ELSE       "else"
  FOR        "for"
  UMINUS
;

%token <std::string> IDENTIFIER "id"
%token <double> NUMBER "number"

%left "?" ":";
%left ">" "<" "==";
%left "+" "-";
%left "*" "/";
%right UMINUS;
%left "or";
%left "and";
%right "not";
%nonassoc LOWER_THAN_ELSE;
%nonassoc ELSE;

%type <ExprAST*> exp
%type <ExprAST*> idexp
%type <ExprAST*> expif
%type <ExprAST*> relexp
%type <ExprAST*> condexp
%type <ExprAST*> initexp
%type <std::vector<ExprAST*>> optexp
%type <std::vector<ExprAST*>> explist
%type <RootAST*> program
%type <RootAST*> top
%type <std::vector<RootAST*>> stmts
%type <RootAST*> stmt
%type <FunctionAST*> definition
%type <PrototypeAST*> external
%type <PrototypeAST*> proto
%type <std::vector<std::string>> idseq
%type <std::vector<VarBindingAST*>> vardefs
%type <VarBindingAST*> binding
%type <BlockStmtAST*> block
%type <GlobalVarAST*> globalvar
%type <AssignmentAST*> assignment

%type <IfStmtAST*> ifstmt
%type <ForStmtAST*> forstmt
%type <RootAST*> init


%%
%start startsymb;

startsymb:
  program               { drv.root = $1; }

program:
  %empty                { $$ = new SeqAST(nullptr,nullptr); }
|  top ";" program      { $$ = new SeqAST($1,$3); };

top:
%empty                  { $$ = nullptr; }
| definition            { $$ = $1; }
| external              { $$ = $1; }
| globalvar             { $$ = $1; };

definition:
  "def" proto block     { $$ = new FunctionAST($2,$3); $2->noemit();}

external:
  "extern" proto        { $$ = $2; };

proto:
  "id" "(" idseq ")"    { $$ = new PrototypeAST($1,$3);  };

globalvar: 
  "global" "id"                                         { $$ = new GlobalVarAST($2); };
| "global" "id" "[" "number" "]"                        { $$ = new GlobalArrayVarAST($2, new NumberExprAST($4)); } 
| "global" "id" "[" "number" "]" "=" "{" explist "}"    { $$ = new GlobalArrayVarAST($2, new NumberExprAST($4), $8); };

idseq:
  %empty                { std::vector<std::string> args;
                          $$ = args; }
| "id" idseq            { $2.insert($2.begin(),$1); $$ = $2; };

stmts:
  stmt                   { std::vector<RootAST*> args;
                           args.push_back($1);
			                     $$ = args; }
| stmt ";" stmts         { $3.insert($3.begin(), $1); $$ = $3; };

init: 
   binding                 { $$ = $1; }
|  assignment             { $$ = $1; };

stmt:
  assignment            { $$ = $1; }
| block                  { $$ = $1; }
| ifstmt                 { $$ = $1; } 
| forstmt                { $$ = $1; }
| exp                    { $$ = $1; };

assignment:
  "id" "=" exp              { $$ = new AssignmentAST($1, $3, AssignmentType::Normal); }  
| "++" "id"                 { $$ = 
                            new AssignmentAST($2, new BinaryExprAST("+", new VariableExprAST($2), new NumberExprAST(1)), 
                            AssignmentType::PreIncrement); 
                            }
| "--" "id"                 { $$ = 
                              new AssignmentAST($2, new BinaryExprAST("-", new VariableExprAST($2), new NumberExprAST(1)),
                              AssignmentType::PreDecrement); 
                            }
| "id" "++"                 { $$ = 
                              new AssignmentAST($1, new BinaryExprAST("+", new VariableExprAST($1), new NumberExprAST(1)),
                              AssignmentType::PostIncrement); 
                            }
| "id" "--"                 { $$ =
                             new AssignmentAST($1, new BinaryExprAST("-", new VariableExprAST($1), new NumberExprAST(1)),
                             AssignmentType::PostDecrement); 
                            };
| "id" "[" exp "]" "=" exp  { $$ = new ArrayAssignmentAST($1, $3, $6); };

ifstmt:
  "if" "(" condexp ")" %prec LOWER_THAN_ELSE stmt           { $$ = new IfStmtAST($3,$5,nullptr); } 
| "if" "(" condexp ")" stmt "else" stmt                     { $$ = new IfStmtAST($3,$5,$7); };

forstmt:
  "for" "(" init ";" condexp ";" assignment ")" stmt       { $$ = new ForStmtAST($3,$5,$7,$9); }

block:
  "{" stmts "}"                                             { $$ = new BlockStmtAST(std::vector<VarBindingAST*>(), $2); }
| "{" vardefs ";" stmts "}"                                 { $$ = new BlockStmtAST($2, $4); };

  
vardefs:
  binding                 { std::vector<VarBindingAST*> definitions;
                            definitions.push_back($1);
                            $$ = definitions; }
| vardefs ";" binding     { $1.push_back($3);
                            $$ = $1; };
                            
binding:
  "var" "id" initexp                                      { $$ = new VarBindingAST($2,$3); }
| "var" "id" "[" "number" "]"                             { $$ = new ArrayBindingAST($2, new NumberExprAST($4)); }
| "var" "id" "[" "number" "]" "=" "{" explist "}"         { $$ = new ArrayBindingAST($2, new NumberExprAST($4), $8); };

exp:
  exp "+" exp           { $$ = new BinaryExprAST("+",$1,$3); }
| exp "-" exp           { $$ = new BinaryExprAST("-",$1,$3); }
| exp "*" exp           { $$ = new BinaryExprAST("*",$1,$3); }
| exp "/" exp           { $$ = new BinaryExprAST("/",$1,$3); }
| idexp                 { $$ = $1; }
| "(" exp ")"           { $$ = $2; }
| "number"              { $$ = new NumberExprAST($1); }
| "-" exp %prec UMINUS  { $$ = new UnaryExprAST('-', $2); }
| expif                 { $$ = $1; }

initexp:
  %empty                { $$ = nullptr; }
| "=" exp               { $$ = $2; }    
                      
expif:
  condexp "?" exp ":" exp { $$ = new IfExprAST($1,$3,$5); }

relexp:
  exp "<" exp               { $$ = new BinaryExprAST("<",$1,$3); }
| exp ">" exp               { $$ = new BinaryExprAST(">",$1,$3); }
| exp "==" exp              { $$ = new BinaryExprAST("==",$1,$3); }

condexp:
  relexp                    { $$ = $1; }
| relexp "and" condexp      { $$ = new BinaryExprAST("&&",$1,$3); }
| relexp "or" condexp       { $$ = new BinaryExprAST("||",$1,$3); }
| "not" condexp             { $$ = new UnaryExprAST('!',$2); }
| "(" condexp ")"           { $$ = $2; }

idexp:
  "id"                  { $$ = new VariableExprAST($1); }
| "id" "(" optexp ")"   { $$ = new CallExprAST($1,$3); };
| "id" "[" exp "]"      { $$ = new ArrayAccessAST($1, $3); };

optexp:
  %empty                { std::vector<ExprAST*> args;
			                    $$ = args; }
| explist               { $$ = $1; };

explist:
  exp                   { std::vector<ExprAST*> args;
                          args.push_back($1);
			                    $$ = args;
                        }
| exp "," explist       { $3.insert($3.begin(), $1); $$ = $3; };
 
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}