// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hpp"


// Unqualified %code blocks.
#line 40 "grammar/parser.yy"

# include "driver.hpp"

#line 50 "src/parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 142 "src/parser.cpp"

  /// Build a parser object.
  parser::parser (driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_assignment: // assignment
        value.YY_MOVE_OR_COPY< AssignmentAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.YY_MOVE_OR_COPY< BlockStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_initexp: // initexp
      case symbol_kind::S_expif: // expif
      case symbol_kind::S_relexp: // relexp
      case symbol_kind::S_condexp: // condexp
      case symbol_kind::S_idexp: // idexp
        value.YY_MOVE_OR_COPY< ExprAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_forstmt: // forstmt
        value.YY_MOVE_OR_COPY< ForStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
        value.YY_MOVE_OR_COPY< FunctionAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_globalvar: // globalvar
        value.YY_MOVE_OR_COPY< GlobalVarAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ifstmt: // ifstmt
        value.YY_MOVE_OR_COPY< IfStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.YY_MOVE_OR_COPY< PrototypeAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
      case symbol_kind::S_init: // init
      case symbol_kind::S_stmt: // stmt
        value.YY_MOVE_OR_COPY< RootAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_binding: // binding
        value.YY_MOVE_OR_COPY< VarBindingAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.YY_MOVE_OR_COPY< std::vector<ExprAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmts: // stmts
        value.YY_MOVE_OR_COPY< std::vector<RootAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_vardefs: // vardefs
        value.YY_MOVE_OR_COPY< std::vector<VarBindingAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_idseq: // idseq
        value.YY_MOVE_OR_COPY< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_assignment: // assignment
        value.move< AssignmentAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_block: // block
        value.move< BlockStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_initexp: // initexp
      case symbol_kind::S_expif: // expif
      case symbol_kind::S_relexp: // relexp
      case symbol_kind::S_condexp: // condexp
      case symbol_kind::S_idexp: // idexp
        value.move< ExprAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_forstmt: // forstmt
        value.move< ForStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
        value.move< FunctionAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_globalvar: // globalvar
        value.move< GlobalVarAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ifstmt: // ifstmt
        value.move< IfStmtAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.move< PrototypeAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
      case symbol_kind::S_init: // init
      case symbol_kind::S_stmt: // stmt
        value.move< RootAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_binding: // binding
        value.move< VarBindingAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.move< std::vector<ExprAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_stmts: // stmts
        value.move< std::vector<RootAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_vardefs: // vardefs
        value.move< std::vector<VarBindingAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_idseq: // idseq
        value.move< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_assignment: // assignment
        value.copy< AssignmentAST* > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.copy< BlockStmtAST* > (that.value);
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_initexp: // initexp
      case symbol_kind::S_expif: // expif
      case symbol_kind::S_relexp: // relexp
      case symbol_kind::S_condexp: // condexp
      case symbol_kind::S_idexp: // idexp
        value.copy< ExprAST* > (that.value);
        break;

      case symbol_kind::S_forstmt: // forstmt
        value.copy< ForStmtAST* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
        value.copy< FunctionAST* > (that.value);
        break;

      case symbol_kind::S_globalvar: // globalvar
        value.copy< GlobalVarAST* > (that.value);
        break;

      case symbol_kind::S_ifstmt: // ifstmt
        value.copy< IfStmtAST* > (that.value);
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.copy< PrototypeAST* > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
      case symbol_kind::S_init: // init
      case symbol_kind::S_stmt: // stmt
        value.copy< RootAST* > (that.value);
        break;

      case symbol_kind::S_binding: // binding
        value.copy< VarBindingAST* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.copy< std::vector<ExprAST*> > (that.value);
        break;

      case symbol_kind::S_stmts: // stmts
        value.copy< std::vector<RootAST*> > (that.value);
        break;

      case symbol_kind::S_vardefs: // vardefs
        value.copy< std::vector<VarBindingAST*> > (that.value);
        break;

      case symbol_kind::S_idseq: // idseq
        value.copy< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_assignment: // assignment
        value.move< AssignmentAST* > (that.value);
        break;

      case symbol_kind::S_block: // block
        value.move< BlockStmtAST* > (that.value);
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_initexp: // initexp
      case symbol_kind::S_expif: // expif
      case symbol_kind::S_relexp: // relexp
      case symbol_kind::S_condexp: // condexp
      case symbol_kind::S_idexp: // idexp
        value.move< ExprAST* > (that.value);
        break;

      case symbol_kind::S_forstmt: // forstmt
        value.move< ForStmtAST* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
        value.move< FunctionAST* > (that.value);
        break;

      case symbol_kind::S_globalvar: // globalvar
        value.move< GlobalVarAST* > (that.value);
        break;

      case symbol_kind::S_ifstmt: // ifstmt
        value.move< IfStmtAST* > (that.value);
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.move< PrototypeAST* > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
      case symbol_kind::S_init: // init
      case symbol_kind::S_stmt: // stmt
        value.move< RootAST* > (that.value);
        break;

      case symbol_kind::S_binding: // binding
        value.move< VarBindingAST* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< double > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.move< std::vector<ExprAST*> > (that.value);
        break;

      case symbol_kind::S_stmts: // stmts
        value.move< std::vector<RootAST*> > (that.value);
        break;

      case symbol_kind::S_vardefs: // vardefs
        value.move< std::vector<VarBindingAST*> > (that.value);
        break;

      case symbol_kind::S_idseq: // idseq
        value.move< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_assignment: // assignment
        yylhs.value.emplace< AssignmentAST* > ();
        break;

      case symbol_kind::S_block: // block
        yylhs.value.emplace< BlockStmtAST* > ();
        break;

      case symbol_kind::S_exp: // exp
      case symbol_kind::S_initexp: // initexp
      case symbol_kind::S_expif: // expif
      case symbol_kind::S_relexp: // relexp
      case symbol_kind::S_condexp: // condexp
      case symbol_kind::S_idexp: // idexp
        yylhs.value.emplace< ExprAST* > ();
        break;

      case symbol_kind::S_forstmt: // forstmt
        yylhs.value.emplace< ForStmtAST* > ();
        break;

      case symbol_kind::S_definition: // definition
        yylhs.value.emplace< FunctionAST* > ();
        break;

      case symbol_kind::S_globalvar: // globalvar
        yylhs.value.emplace< GlobalVarAST* > ();
        break;

      case symbol_kind::S_ifstmt: // ifstmt
        yylhs.value.emplace< IfStmtAST* > ();
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        yylhs.value.emplace< PrototypeAST* > ();
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
      case symbol_kind::S_init: // init
      case symbol_kind::S_stmt: // stmt
        yylhs.value.emplace< RootAST* > ();
        break;

      case symbol_kind::S_binding: // binding
        yylhs.value.emplace< VarBindingAST* > ();
        break;

      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        yylhs.value.emplace< std::vector<ExprAST*> > ();
        break;

      case symbol_kind::S_stmts: // stmts
        yylhs.value.emplace< std::vector<RootAST*> > ();
        break;

      case symbol_kind::S_vardefs: // vardefs
        yylhs.value.emplace< std::vector<VarBindingAST*> > ();
        break;

      case symbol_kind::S_idseq: // idseq
        yylhs.value.emplace< std::vector<std::string> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // startsymb: program
#line 126 "grammar/parser.yy"
                        { drv.root = yystack_[0].value.as < RootAST* > (); }
#line 906 "src/parser.cpp"
    break;

  case 3: // program: %empty
#line 129 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = new SeqAST(nullptr,nullptr); }
#line 912 "src/parser.cpp"
    break;

  case 4: // program: top ";" program
#line 130 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = new SeqAST(yystack_[2].value.as < RootAST* > (),yystack_[0].value.as < RootAST* > ()); }
#line 918 "src/parser.cpp"
    break;

  case 5: // top: %empty
#line 133 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = nullptr; }
#line 924 "src/parser.cpp"
    break;

  case 6: // top: definition
#line 134 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = yystack_[0].value.as < FunctionAST* > (); }
#line 930 "src/parser.cpp"
    break;

  case 7: // top: external
#line 135 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = yystack_[0].value.as < PrototypeAST* > (); }
#line 936 "src/parser.cpp"
    break;

  case 8: // top: globalvar
#line 136 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = yystack_[0].value.as < GlobalVarAST* > (); }
#line 942 "src/parser.cpp"
    break;

  case 9: // definition: "def" proto block
#line 139 "grammar/parser.yy"
                        { yylhs.value.as < FunctionAST* > () = new FunctionAST(yystack_[1].value.as < PrototypeAST* > (),yystack_[0].value.as < BlockStmtAST* > ()); yystack_[1].value.as < PrototypeAST* > ()->noemit();}
#line 948 "src/parser.cpp"
    break;

  case 10: // external: "extern" proto
#line 142 "grammar/parser.yy"
                        { yylhs.value.as < PrototypeAST* > () = yystack_[0].value.as < PrototypeAST* > (); }
#line 954 "src/parser.cpp"
    break;

  case 11: // proto: "id" "(" idseq ")"
#line 145 "grammar/parser.yy"
                        { yylhs.value.as < PrototypeAST* > () = new PrototypeAST(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<std::string> > ());  }
#line 960 "src/parser.cpp"
    break;

  case 12: // globalvar: "global" "id"
#line 148 "grammar/parser.yy"
                                                        { yylhs.value.as < GlobalVarAST* > () = new GlobalVarAST(yystack_[0].value.as < std::string > ()); }
#line 966 "src/parser.cpp"
    break;

  case 13: // globalvar: "global" "id" "[" "number" "]"
#line 149 "grammar/parser.yy"
                                                        { yylhs.value.as < GlobalVarAST* > () = new GlobalArrayVarAST(yystack_[3].value.as < std::string > (), new NumberExprAST(yystack_[1].value.as < double > ())); }
#line 972 "src/parser.cpp"
    break;

  case 14: // globalvar: "global" "id" "[" "number" "]" "=" "{" explist "}"
#line 150 "grammar/parser.yy"
                                                        { yylhs.value.as < GlobalVarAST* > () = new GlobalArrayVarAST(yystack_[7].value.as < std::string > (), new NumberExprAST(yystack_[5].value.as < double > ()), yystack_[1].value.as < std::vector<ExprAST*> > ()); }
#line 978 "src/parser.cpp"
    break;

  case 15: // idseq: %empty
#line 153 "grammar/parser.yy"
                        { std::vector<std::string> args;
                          yylhs.value.as < std::vector<std::string> > () = args; }
#line 985 "src/parser.cpp"
    break;

  case 16: // idseq: "id" idseq
#line 155 "grammar/parser.yy"
                        { yystack_[0].value.as < std::vector<std::string> > ().insert(yystack_[0].value.as < std::vector<std::string> > ().begin(),yystack_[1].value.as < std::string > ()); yylhs.value.as < std::vector<std::string> > () = yystack_[0].value.as < std::vector<std::string> > (); }
#line 991 "src/parser.cpp"
    break;

  case 17: // stmts: stmt
#line 158 "grammar/parser.yy"
                         { std::vector<RootAST*> args;
                           args.push_back(yystack_[0].value.as < RootAST* > ());
			                     yylhs.value.as < std::vector<RootAST*> > () = args; }
#line 999 "src/parser.cpp"
    break;

  case 18: // stmts: stmt ";" stmts
#line 161 "grammar/parser.yy"
                         { yystack_[0].value.as < std::vector<RootAST*> > ().insert(yystack_[0].value.as < std::vector<RootAST*> > ().begin(), yystack_[2].value.as < RootAST* > ()); yylhs.value.as < std::vector<RootAST*> > () = yystack_[0].value.as < std::vector<RootAST*> > (); }
#line 1005 "src/parser.cpp"
    break;

  case 19: // init: binding
#line 164 "grammar/parser.yy"
                           { yylhs.value.as < RootAST* > () = yystack_[0].value.as < VarBindingAST* > (); }
#line 1011 "src/parser.cpp"
    break;

  case 20: // init: assignment
#line 165 "grammar/parser.yy"
                          { yylhs.value.as < RootAST* > () = yystack_[0].value.as < AssignmentAST* > (); }
#line 1017 "src/parser.cpp"
    break;

  case 21: // stmt: assignment
#line 168 "grammar/parser.yy"
                        { yylhs.value.as < RootAST* > () = yystack_[0].value.as < AssignmentAST* > (); }
#line 1023 "src/parser.cpp"
    break;

  case 22: // stmt: block
#line 169 "grammar/parser.yy"
                         { yylhs.value.as < RootAST* > () = yystack_[0].value.as < BlockStmtAST* > (); }
#line 1029 "src/parser.cpp"
    break;

  case 23: // stmt: ifstmt
#line 170 "grammar/parser.yy"
                         { yylhs.value.as < RootAST* > () = yystack_[0].value.as < IfStmtAST* > (); }
#line 1035 "src/parser.cpp"
    break;

  case 24: // stmt: forstmt
#line 171 "grammar/parser.yy"
                         { yylhs.value.as < RootAST* > () = yystack_[0].value.as < ForStmtAST* > (); }
#line 1041 "src/parser.cpp"
    break;

  case 25: // stmt: exp
#line 172 "grammar/parser.yy"
                         { yylhs.value.as < RootAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1047 "src/parser.cpp"
    break;

  case 26: // assignment: "id" "=" exp
#line 175 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () = new AssignmentAST(yystack_[2].value.as < std::string > (), yystack_[0].value.as < ExprAST* > (), AssignmentType::Normal); }
#line 1053 "src/parser.cpp"
    break;

  case 27: // assignment: "++" "id"
#line 176 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () = 
                            new AssignmentAST(yystack_[0].value.as < std::string > (), new BinaryExprAST("+", new VariableExprAST(yystack_[0].value.as < std::string > ()), new NumberExprAST(1)), 
                            AssignmentType::PreIncrement); 
                            }
#line 1062 "src/parser.cpp"
    break;

  case 28: // assignment: "--" "id"
#line 180 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () = 
                              new AssignmentAST(yystack_[0].value.as < std::string > (), new BinaryExprAST("-", new VariableExprAST(yystack_[0].value.as < std::string > ()), new NumberExprAST(1)),
                              AssignmentType::PreDecrement); 
                            }
#line 1071 "src/parser.cpp"
    break;

  case 29: // assignment: "id" "++"
#line 184 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () = 
                              new AssignmentAST(yystack_[1].value.as < std::string > (), new BinaryExprAST("+", new VariableExprAST(yystack_[1].value.as < std::string > ()), new NumberExprAST(1)),
                              AssignmentType::PostIncrement); 
                            }
#line 1080 "src/parser.cpp"
    break;

  case 30: // assignment: "id" "--"
#line 188 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () =
                             new AssignmentAST(yystack_[1].value.as < std::string > (), new BinaryExprAST("-", new VariableExprAST(yystack_[1].value.as < std::string > ()), new NumberExprAST(1)),
                             AssignmentType::PostDecrement); 
                            }
#line 1089 "src/parser.cpp"
    break;

  case 31: // assignment: "id" "[" exp "]" "=" exp
#line 192 "grammar/parser.yy"
                            { yylhs.value.as < AssignmentAST* > () = new ArrayAssignmentAST(yystack_[5].value.as < std::string > (), yystack_[3].value.as < ExprAST* > (), yystack_[0].value.as < ExprAST* > ()); }
#line 1095 "src/parser.cpp"
    break;

  case 32: // ifstmt: "if" "(" condexp ")" stmt
#line 195 "grammar/parser.yy"
                                                            { yylhs.value.as < IfStmtAST* > () = new IfStmtAST(yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < RootAST* > (),nullptr); }
#line 1101 "src/parser.cpp"
    break;

  case 33: // ifstmt: "if" "(" condexp ")" stmt "else" stmt
#line 196 "grammar/parser.yy"
                                                            { yylhs.value.as < IfStmtAST* > () = new IfStmtAST(yystack_[4].value.as < ExprAST* > (),yystack_[2].value.as < RootAST* > (),yystack_[0].value.as < RootAST* > ()); }
#line 1107 "src/parser.cpp"
    break;

  case 34: // forstmt: "for" "(" init ";" condexp ";" assignment ")" stmt
#line 199 "grammar/parser.yy"
                                                           { yylhs.value.as < ForStmtAST* > () = new ForStmtAST(yystack_[6].value.as < RootAST* > (),yystack_[4].value.as < ExprAST* > (),yystack_[2].value.as < AssignmentAST* > (),yystack_[0].value.as < RootAST* > ()); }
#line 1113 "src/parser.cpp"
    break;

  case 35: // block: "{" stmts "}"
#line 202 "grammar/parser.yy"
                                                            { yylhs.value.as < BlockStmtAST* > () = new BlockStmtAST(std::vector<VarBindingAST*>(), yystack_[1].value.as < std::vector<RootAST*> > ()); }
#line 1119 "src/parser.cpp"
    break;

  case 36: // block: "{" vardefs ";" stmts "}"
#line 203 "grammar/parser.yy"
                                                            { yylhs.value.as < BlockStmtAST* > () = new BlockStmtAST(yystack_[3].value.as < std::vector<VarBindingAST*> > (), yystack_[1].value.as < std::vector<RootAST*> > ()); }
#line 1125 "src/parser.cpp"
    break;

  case 37: // vardefs: binding
#line 207 "grammar/parser.yy"
                          { std::vector<VarBindingAST*> definitions;
                            definitions.push_back(yystack_[0].value.as < VarBindingAST* > ());
                            yylhs.value.as < std::vector<VarBindingAST*> > () = definitions; }
#line 1133 "src/parser.cpp"
    break;

  case 38: // vardefs: vardefs ";" binding
#line 210 "grammar/parser.yy"
                          { yystack_[2].value.as < std::vector<VarBindingAST*> > ().push_back(yystack_[0].value.as < VarBindingAST* > ());
                            yylhs.value.as < std::vector<VarBindingAST*> > () = yystack_[2].value.as < std::vector<VarBindingAST*> > (); }
#line 1140 "src/parser.cpp"
    break;

  case 39: // binding: "var" "id" initexp
#line 214 "grammar/parser.yy"
                                                          { yylhs.value.as < VarBindingAST* > () = new VarBindingAST(yystack_[1].value.as < std::string > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1146 "src/parser.cpp"
    break;

  case 40: // binding: "var" "id" "[" "number" "]"
#line 215 "grammar/parser.yy"
                                                          { yylhs.value.as < VarBindingAST* > () = new ArrayBindingAST(yystack_[3].value.as < std::string > (), new NumberExprAST(yystack_[1].value.as < double > ())); }
#line 1152 "src/parser.cpp"
    break;

  case 41: // binding: "var" "id" "[" "number" "]" "=" "{" explist "}"
#line 216 "grammar/parser.yy"
                                                          { yylhs.value.as < VarBindingAST* > () = new ArrayBindingAST(yystack_[7].value.as < std::string > (), new NumberExprAST(yystack_[5].value.as < double > ()), yystack_[1].value.as < std::vector<ExprAST*> > ()); }
#line 1158 "src/parser.cpp"
    break;

  case 42: // exp: exp "+" exp
#line 219 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new BinaryExprAST("+",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1164 "src/parser.cpp"
    break;

  case 43: // exp: exp "-" exp
#line 220 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new BinaryExprAST("-",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1170 "src/parser.cpp"
    break;

  case 44: // exp: exp "*" exp
#line 221 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new BinaryExprAST("*",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1176 "src/parser.cpp"
    break;

  case 45: // exp: exp "/" exp
#line 222 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new BinaryExprAST("/",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1182 "src/parser.cpp"
    break;

  case 46: // exp: idexp
#line 223 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1188 "src/parser.cpp"
    break;

  case 47: // exp: "(" exp ")"
#line 224 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = yystack_[1].value.as < ExprAST* > (); }
#line 1194 "src/parser.cpp"
    break;

  case 48: // exp: "number"
#line 225 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new NumberExprAST(yystack_[0].value.as < double > ()); }
#line 1200 "src/parser.cpp"
    break;

  case 49: // exp: "-" exp
#line 226 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new UnaryExprAST('-', yystack_[0].value.as < ExprAST* > ()); }
#line 1206 "src/parser.cpp"
    break;

  case 50: // exp: expif
#line 227 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1212 "src/parser.cpp"
    break;

  case 51: // initexp: %empty
#line 230 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = nullptr; }
#line 1218 "src/parser.cpp"
    break;

  case 52: // initexp: "=" exp
#line 231 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1224 "src/parser.cpp"
    break;

  case 53: // expif: condexp "?" exp ":" exp
#line 234 "grammar/parser.yy"
                          { yylhs.value.as < ExprAST* > () = new IfExprAST(yystack_[4].value.as < ExprAST* > (),yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1230 "src/parser.cpp"
    break;

  case 54: // relexp: exp "<" exp
#line 237 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new BinaryExprAST("<",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1236 "src/parser.cpp"
    break;

  case 55: // relexp: exp ">" exp
#line 238 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new BinaryExprAST(">",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1242 "src/parser.cpp"
    break;

  case 56: // relexp: exp "==" exp
#line 239 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new BinaryExprAST("==",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1248 "src/parser.cpp"
    break;

  case 57: // condexp: relexp
#line 242 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1254 "src/parser.cpp"
    break;

  case 58: // condexp: relexp "and" condexp
#line 243 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new BinaryExprAST("&&",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1260 "src/parser.cpp"
    break;

  case 59: // condexp: relexp "or" condexp
#line 244 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new BinaryExprAST("||",yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 1266 "src/parser.cpp"
    break;

  case 60: // condexp: "not" condexp
#line 245 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = new UnaryExprAST('!',yystack_[0].value.as < ExprAST* > ()); }
#line 1272 "src/parser.cpp"
    break;

  case 61: // condexp: "(" condexp ")"
#line 246 "grammar/parser.yy"
                            { yylhs.value.as < ExprAST* > () = yystack_[1].value.as < ExprAST* > (); }
#line 1278 "src/parser.cpp"
    break;

  case 62: // idexp: "id"
#line 249 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new VariableExprAST(yystack_[0].value.as < std::string > ()); }
#line 1284 "src/parser.cpp"
    break;

  case 63: // idexp: "id" "(" optexp ")"
#line 250 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new CallExprAST(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<ExprAST*> > ()); }
#line 1290 "src/parser.cpp"
    break;

  case 64: // idexp: "id" "[" exp "]"
#line 251 "grammar/parser.yy"
                        { yylhs.value.as < ExprAST* > () = new ArrayAccessAST(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1296 "src/parser.cpp"
    break;

  case 65: // optexp: %empty
#line 254 "grammar/parser.yy"
                        { std::vector<ExprAST*> args;
			                    yylhs.value.as < std::vector<ExprAST*> > () = args; }
#line 1303 "src/parser.cpp"
    break;

  case 66: // optexp: explist
#line 256 "grammar/parser.yy"
                        { yylhs.value.as < std::vector<ExprAST*> > () = yystack_[0].value.as < std::vector<ExprAST*> > (); }
#line 1309 "src/parser.cpp"
    break;

  case 67: // explist: exp
#line 259 "grammar/parser.yy"
                        { std::vector<ExprAST*> args;
                          args.push_back(yystack_[0].value.as < ExprAST* > ());
			                    yylhs.value.as < std::vector<ExprAST*> > () = args;
                        }
#line 1318 "src/parser.cpp"
    break;

  case 68: // explist: exp "," explist
#line 263 "grammar/parser.yy"
                        { yystack_[0].value.as < std::vector<ExprAST*> > ().insert(yystack_[0].value.as < std::vector<ExprAST*> > ().begin(), yystack_[2].value.as < ExprAST* > ()); yylhs.value.as < std::vector<ExprAST*> > () = yystack_[0].value.as < std::vector<ExprAST*> > (); }
#line 1324 "src/parser.cpp"
    break;


#line 1328 "src/parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -93;

  const signed char parser::yytable_ninf_ = -6;

  const short
  parser::yypact_[] =
  {
      39,   -30,   -30,    -4,    38,   -93,    64,   -93,   -93,   -93,
      73,   -93,    72,    62,   -93,    39,    75,    65,   -93,    66,
     -93,    75,   102,    -3,    77,    80,    -3,    -3,    81,   106,
     108,   100,   -93,    99,   124,   -93,   -93,   -93,   -93,   129,
     -93,   186,   -93,     3,   110,   -93,   115,   -93,   -93,     5,
     -93,   -93,   -93,   160,    67,   186,   -93,    57,    -3,    -1,
     -93,   -93,    -3,    -3,    -3,   -93,    98,    65,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,   123,    -3,
     -93,   -93,    -3,   105,   -93,    70,    50,   143,   -93,   -93,
       9,   135,   -93,   186,   120,   -93,   130,   -93,    53,    53,
     -93,   -93,   186,   186,   186,   -93,   -93,   173,   132,   133,
     186,   134,    98,    -3,    -3,    -3,   -93,   136,   -93,    -3,
      -3,   -93,   139,   141,   146,    17,   -93,    -3,   186,   144,
     142,    98,   136,     2,   186,   -93,    -3,   -93,   156,   151,
      98,   -93,   -93
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     0,     0,     2,     0,     6,     7,     8,
       0,    10,     0,    12,     1,     3,    15,     0,     9,     0,
       4,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,    48,     0,    17,    21,    23,    24,    22,     0,
      37,    25,    50,    57,     0,    46,     0,    16,    11,    62,
      49,    27,    28,     0,     0,     0,    60,    51,     0,     0,
      29,    30,    65,     0,     0,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
      47,    61,     0,     0,    39,     0,     0,     0,    20,    19,
      67,     0,    66,    26,     0,    18,     0,    38,    43,    42,
      44,    45,    54,    55,    56,    58,    59,     0,     0,     0,
      52,     0,     0,     0,     0,     0,    63,    64,    36,     0,
       0,    64,    40,    32,     0,     0,    68,     0,    53,     0,
       0,     0,     0,     0,    31,    14,     0,    33,     0,     0,
       0,    41,    34
  };

  const short
  parser::yypgoto_[] =
  {
     -93,   -93,   165,   -93,   -93,   -93,   179,   -93,   163,    22,
     -93,   -76,   -58,   -93,   -93,   174,   -93,    10,   -23,   -93,
     -93,   -93,   -15,   -93,   -93,   -92
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     4,     5,     6,     7,     8,    11,     9,    22,    33,
      87,    34,    35,    36,    37,    38,    39,    40,    41,    84,
      42,    43,    44,    45,    91,    92
  };

  const short
  parser::yytable_[] =
  {
      50,    88,    23,    53,    55,    10,    24,    25,    26,    24,
      25,    54,    56,   115,    68,    69,    62,    27,    70,    71,
     133,    75,    76,   126,    72,    73,    74,    28,   129,    79,
      77,    13,    49,    32,    86,    55,   123,    86,    14,    90,
      93,    94,    -5,    85,   139,    98,    99,   100,   101,   102,
     103,   104,    55,    55,   107,   137,   109,    60,    61,   110,
     105,   106,    70,    71,   142,     1,     2,    15,     3,    89,
      23,    63,    24,    25,   113,   138,    26,    97,    82,    81,
      77,    83,   112,    77,    16,    27,    19,    17,    95,    96,
     124,    55,    90,    28,    17,    29,   128,    90,    30,   125,
      31,    32,    46,    23,   134,    24,    25,    60,    61,    26,
      21,    62,    51,    90,    48,    52,    57,    58,    27,    59,
      17,    63,    65,    77,    64,    68,    69,    66,    29,    70,
      71,    30,    67,    31,    32,    72,    73,    74,    68,    69,
      78,   111,    70,    71,   108,   117,   114,   116,    72,    73,
      74,    68,    69,   118,   120,    70,    71,   127,   121,   122,
     130,    72,    73,    74,   136,    68,    69,   135,   140,    70,
      71,   132,    80,   131,   141,    72,    73,    74,    68,    69,
      20,    12,    70,    71,    47,     0,    18,   119,    72,    73,
      74,    68,    69,     0,     0,    70,    71,     0,     0,     0,
       0,    72,    73,    74
  };

  const short
  parser::yycheck_[] =
  {
      23,    59,     5,    26,    27,    35,     7,     8,    11,     7,
       8,    26,    27,     4,     5,     6,    11,    20,     9,    10,
       3,    18,    19,   115,    15,    16,    17,    28,   120,    24,
      13,    35,    35,    36,    35,    58,   112,    35,     0,    62,
      63,    64,     3,    58,   136,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   131,    79,     7,     8,    82,
      75,    76,     9,    10,   140,    26,    27,     3,    29,    59,
       5,    21,     7,     8,    24,   133,    11,    67,    21,    12,
      13,    24,    12,    13,    11,    20,    24,    22,    66,    67,
     113,   114,   115,    28,    22,    30,   119,   120,    33,   114,
      35,    36,    36,     5,   127,     7,     8,     7,     8,    11,
      35,    11,    35,   136,    12,    35,    35,    11,    20,    11,
      22,    21,    23,    13,    24,     5,     6,     3,    30,     9,
      10,    33,     3,    35,    36,    15,    16,    17,     5,     6,
      25,    36,     9,    10,    21,    25,     3,    12,    15,    16,
      17,     5,     6,    23,    22,     9,    10,    21,    25,    25,
      21,    15,    16,    17,    22,     5,     6,    23,    12,     9,
      10,    25,    12,    32,    23,    15,    16,    17,     5,     6,
      15,     2,     9,    10,    21,    -1,    12,    14,    15,    16,
      17,     5,     6,    -1,    -1,     9,    10,    -1,    -1,    -1,
      -1,    15,    16,    17
  };

  const signed char
  parser::yystos_[] =
  {
       0,    26,    27,    29,    39,    40,    41,    42,    43,    45,
      35,    44,    44,    35,     0,     3,    11,    22,    53,    24,
      40,    35,    46,     5,     7,     8,    11,    20,    28,    30,
      33,    35,    36,    47,    49,    50,    51,    52,    53,    54,
      55,    56,    58,    59,    60,    61,    36,    46,    12,    35,
      56,    35,    35,    56,    60,    56,    60,    35,    11,    11,
       7,     8,    11,    21,    24,    23,     3,     3,     5,     6,
       9,    10,    15,    16,    17,    18,    19,    13,    25,    24,
      12,    12,    21,    24,    57,    60,    35,    48,    50,    55,
      56,    62,    63,    56,    56,    47,    47,    55,    56,    56,
      56,    56,    56,    56,    56,    60,    60,    56,    21,    56,
      56,    36,    12,    24,     3,     4,    12,    25,    23,    14,
      22,    25,    25,    49,    56,    60,    63,    21,    56,    63,
      21,    32,    25,     3,    56,    23,    22,    49,    50,    63,
      12,    23,    49
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    38,    39,    40,    40,    41,    41,    41,    41,    42,
      43,    44,    45,    45,    45,    46,    46,    47,    47,    48,
      48,    49,    49,    49,    49,    49,    50,    50,    50,    50,
      50,    50,    51,    51,    52,    53,    53,    54,    54,    55,
      55,    55,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    57,    57,    58,    59,    59,    59,    60,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    63,    63
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     3,     0,     1,     1,     1,     3,
       2,     4,     2,     5,     9,     0,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     2,     2,
       2,     6,     5,     7,     9,     3,     5,     1,     3,     3,
       5,     9,     3,     3,     3,     3,     1,     3,     1,     2,
       1,     0,     2,     5,     3,     3,     3,     1,     3,     3,
       2,     3,     1,     4,     4,     0,     1,     1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\";\"", "\",\"",
  "\"-\"", "\"+\"", "\"++\"", "\"--\"", "\"*\"", "\"/\"", "\"(\"", "\")\"",
  "\"?\"", "\":\"", "\"<\"", "\">\"", "\"==\"", "\"and\"", "\"or\"",
  "\"not\"", "\"=\"", "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"extern\"",
  "\"def\"", "\"var\"", "\"global\"", "\"if\"", "\"then\"", "\"else\"",
  "\"for\"", "UMINUS", "\"id\"", "\"number\"", "LOWER_THAN_ELSE",
  "$accept", "startsymb", "program", "top", "definition", "external",
  "proto", "globalvar", "idseq", "stmts", "init", "stmt", "assignment",
  "ifstmt", "forstmt", "block", "vardefs", "binding", "exp", "initexp",
  "expif", "relexp", "condexp", "idexp", "optexp", "explist", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,   126,   126,   129,   130,   133,   134,   135,   136,   139,
     142,   145,   148,   149,   150,   153,   155,   158,   161,   164,
     165,   168,   169,   170,   171,   172,   175,   176,   180,   184,
     188,   192,   195,   196,   199,   202,   203,   207,   210,   214,
     215,   216,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   230,   231,   234,   237,   238,   239,   242,   243,   244,
     245,   246,   249,   250,   251,   254,   256,   259,   263
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1899 "src/parser.cpp"

#line 265 "grammar/parser.yy"


void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
