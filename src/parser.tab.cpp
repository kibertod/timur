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





#include "parser.tab.hpp"


// Unqualified %code blocks.
#line 35 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    
    namespace yy
    {
        parser::symbol_type yylex(ParserContext& ctx);
        std::string point_err(ParserContext& ctx, size_t row, size_t col);
    }

#line 54 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"


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
#line 146 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"

  /// Build a parser object.
  parser::parser (ParserContext& ctx_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      ctx (ctx_yyarg)
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
      case symbol_kind::S_Class: // Class
        value.YY_MOVE_OR_COPY< ast::Class > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expression: // Expression
        value.YY_MOVE_OR_COPY< ast::Expression > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Literal: // Literal
        value.YY_MOVE_OR_COPY< ast::Expression::Literal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemberDeclaration: // MemberDeclaration
        value.YY_MOVE_OR_COPY< ast::MemberDeclaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.YY_MOVE_OR_COPY< ast::Statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ElIf: // ElIf
        value.YY_MOVE_OR_COPY< ast::Statement::If::ElIf > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TypeName: // TypeName
        value.YY_MOVE_OR_COPY< ast::TypeName > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.YY_MOVE_OR_COPY< ast::Variable > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Argument: // Argument
        value.YY_MOVE_OR_COPY< std::pair<ast::TypeName, ast::Identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_LitStr: // LitStr
      case symbol_kind::S_LitInt: // LitInt
      case symbol_kind::S_LitReal: // LitReal
      case symbol_kind::S_Err: // Err
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Classes: // Classes
        value.YY_MOVE_OR_COPY< std::vector<ast::Class> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expressions: // Expressions
        value.YY_MOVE_OR_COPY< std::vector<ast::Expression> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemberDeclarations: // MemberDeclarations
        value.YY_MOVE_OR_COPY< std::vector<ast::MemberDeclaration> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ElIfs: // ElIfs
        value.YY_MOVE_OR_COPY< std::vector<ast::Statement::If::ElIf> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statements: // Statements
        value.YY_MOVE_OR_COPY< std::vector<ast::Statement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TypeNames: // TypeNames
        value.YY_MOVE_OR_COPY< std::vector<ast::TypeName> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Arguments: // Arguments
        value.YY_MOVE_OR_COPY< std::vector<std::pair<ast::TypeName, ast::Identifier>> > (YY_MOVE (that.value));
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
      case symbol_kind::S_Class: // Class
        value.move< ast::Class > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expression: // Expression
        value.move< ast::Expression > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Literal: // Literal
        value.move< ast::Expression::Literal > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemberDeclaration: // MemberDeclaration
        value.move< ast::MemberDeclaration > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< ast::Statement > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ElIf: // ElIf
        value.move< ast::Statement::If::ElIf > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TypeName: // TypeName
        value.move< ast::TypeName > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< ast::Variable > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Argument: // Argument
        value.move< std::pair<ast::TypeName, ast::Identifier> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_LitStr: // LitStr
      case symbol_kind::S_LitInt: // LitInt
      case symbol_kind::S_LitReal: // LitReal
      case symbol_kind::S_Err: // Err
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Classes: // Classes
        value.move< std::vector<ast::Class> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Expressions: // Expressions
        value.move< std::vector<ast::Expression> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_MemberDeclarations: // MemberDeclarations
        value.move< std::vector<ast::MemberDeclaration> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_ElIfs: // ElIfs
        value.move< std::vector<ast::Statement::If::ElIf> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Statements: // Statements
        value.move< std::vector<ast::Statement> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TypeNames: // TypeNames
        value.move< std::vector<ast::TypeName> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_Arguments: // Arguments
        value.move< std::vector<std::pair<ast::TypeName, ast::Identifier>> > (YY_MOVE (that.value));
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
      case symbol_kind::S_Class: // Class
        value.copy< ast::Class > (that.value);
        break;

      case symbol_kind::S_Expression: // Expression
        value.copy< ast::Expression > (that.value);
        break;

      case symbol_kind::S_Literal: // Literal
        value.copy< ast::Expression::Literal > (that.value);
        break;

      case symbol_kind::S_MemberDeclaration: // MemberDeclaration
        value.copy< ast::MemberDeclaration > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.copy< ast::Statement > (that.value);
        break;

      case symbol_kind::S_ElIf: // ElIf
        value.copy< ast::Statement::If::ElIf > (that.value);
        break;

      case symbol_kind::S_TypeName: // TypeName
        value.copy< ast::TypeName > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.copy< ast::Variable > (that.value);
        break;

      case symbol_kind::S_Argument: // Argument
        value.copy< std::pair<ast::TypeName, ast::Identifier> > (that.value);
        break;

      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_LitStr: // LitStr
      case symbol_kind::S_LitInt: // LitInt
      case symbol_kind::S_LitReal: // LitReal
      case symbol_kind::S_Err: // Err
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_Classes: // Classes
        value.copy< std::vector<ast::Class> > (that.value);
        break;

      case symbol_kind::S_Expressions: // Expressions
        value.copy< std::vector<ast::Expression> > (that.value);
        break;

      case symbol_kind::S_MemberDeclarations: // MemberDeclarations
        value.copy< std::vector<ast::MemberDeclaration> > (that.value);
        break;

      case symbol_kind::S_ElIfs: // ElIfs
        value.copy< std::vector<ast::Statement::If::ElIf> > (that.value);
        break;

      case symbol_kind::S_Statements: // Statements
        value.copy< std::vector<ast::Statement> > (that.value);
        break;

      case symbol_kind::S_TypeNames: // TypeNames
        value.copy< std::vector<ast::TypeName> > (that.value);
        break;

      case symbol_kind::S_Arguments: // Arguments
        value.copy< std::vector<std::pair<ast::TypeName, ast::Identifier>> > (that.value);
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
      case symbol_kind::S_Class: // Class
        value.move< ast::Class > (that.value);
        break;

      case symbol_kind::S_Expression: // Expression
        value.move< ast::Expression > (that.value);
        break;

      case symbol_kind::S_Literal: // Literal
        value.move< ast::Expression::Literal > (that.value);
        break;

      case symbol_kind::S_MemberDeclaration: // MemberDeclaration
        value.move< ast::MemberDeclaration > (that.value);
        break;

      case symbol_kind::S_Statement: // Statement
        value.move< ast::Statement > (that.value);
        break;

      case symbol_kind::S_ElIf: // ElIf
        value.move< ast::Statement::If::ElIf > (that.value);
        break;

      case symbol_kind::S_TypeName: // TypeName
        value.move< ast::TypeName > (that.value);
        break;

      case symbol_kind::S_Variable: // Variable
        value.move< ast::Variable > (that.value);
        break;

      case symbol_kind::S_Argument: // Argument
        value.move< std::pair<ast::TypeName, ast::Identifier> > (that.value);
        break;

      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_LitStr: // LitStr
      case symbol_kind::S_LitInt: // LitInt
      case symbol_kind::S_LitReal: // LitReal
      case symbol_kind::S_Err: // Err
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_Classes: // Classes
        value.move< std::vector<ast::Class> > (that.value);
        break;

      case symbol_kind::S_Expressions: // Expressions
        value.move< std::vector<ast::Expression> > (that.value);
        break;

      case symbol_kind::S_MemberDeclarations: // MemberDeclarations
        value.move< std::vector<ast::MemberDeclaration> > (that.value);
        break;

      case symbol_kind::S_ElIfs: // ElIfs
        value.move< std::vector<ast::Statement::If::ElIf> > (that.value);
        break;

      case symbol_kind::S_Statements: // Statements
        value.move< std::vector<ast::Statement> > (that.value);
        break;

      case symbol_kind::S_TypeNames: // TypeNames
        value.move< std::vector<ast::TypeName> > (that.value);
        break;

      case symbol_kind::S_Arguments: // Arguments
        value.move< std::vector<std::pair<ast::TypeName, ast::Identifier>> > (that.value);
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
            symbol_type yylookahead (yylex (ctx));
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
      case symbol_kind::S_Class: // Class
        yylhs.value.emplace< ast::Class > ();
        break;

      case symbol_kind::S_Expression: // Expression
        yylhs.value.emplace< ast::Expression > ();
        break;

      case symbol_kind::S_Literal: // Literal
        yylhs.value.emplace< ast::Expression::Literal > ();
        break;

      case symbol_kind::S_MemberDeclaration: // MemberDeclaration
        yylhs.value.emplace< ast::MemberDeclaration > ();
        break;

      case symbol_kind::S_Statement: // Statement
        yylhs.value.emplace< ast::Statement > ();
        break;

      case symbol_kind::S_ElIf: // ElIf
        yylhs.value.emplace< ast::Statement::If::ElIf > ();
        break;

      case symbol_kind::S_TypeName: // TypeName
        yylhs.value.emplace< ast::TypeName > ();
        break;

      case symbol_kind::S_Variable: // Variable
        yylhs.value.emplace< ast::Variable > ();
        break;

      case symbol_kind::S_Argument: // Argument
        yylhs.value.emplace< std::pair<ast::TypeName, ast::Identifier> > ();
        break;

      case symbol_kind::S_Identifier: // Identifier
      case symbol_kind::S_LitStr: // LitStr
      case symbol_kind::S_LitInt: // LitInt
      case symbol_kind::S_LitReal: // LitReal
      case symbol_kind::S_Err: // Err
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_Classes: // Classes
        yylhs.value.emplace< std::vector<ast::Class> > ();
        break;

      case symbol_kind::S_Expressions: // Expressions
        yylhs.value.emplace< std::vector<ast::Expression> > ();
        break;

      case symbol_kind::S_MemberDeclarations: // MemberDeclarations
        yylhs.value.emplace< std::vector<ast::MemberDeclaration> > ();
        break;

      case symbol_kind::S_ElIfs: // ElIfs
        yylhs.value.emplace< std::vector<ast::Statement::If::ElIf> > ();
        break;

      case symbol_kind::S_Statements: // Statements
        yylhs.value.emplace< std::vector<ast::Statement> > ();
        break;

      case symbol_kind::S_TypeNames: // TypeNames
        yylhs.value.emplace< std::vector<ast::TypeName> > ();
        break;

      case symbol_kind::S_Arguments: // Arguments
        yylhs.value.emplace< std::vector<std::pair<ast::TypeName, ast::Identifier>> > ();
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
  case 2: // Program: Classes
#line 111 "/home/kibertod/dev/uni/timur/src/parser.ypp"
            { ctx.root = yystack_[0].value.as < std::vector<ast::Class> > (); }
#line 900 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 3: // TypeName: Identifier
#line 115 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::TypeName > () = {yystack_[0].value.as < std::string > (), {}, false}; }
#line 906 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 4: // TypeName: Identifier Ptr
#line 116 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                     { yylhs.value.as < ast::TypeName > () = {yystack_[1].value.as < std::string > (), {}, true}; }
#line 912 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 5: // TypeName: Identifier LBracket TypeNames RBracket
#line 117 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                             { yylhs.value.as < ast::TypeName > () = {yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::TypeName> > (), false}; }
#line 918 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 6: // TypeName: Identifier LBracket TypeNames RBracket Ptr
#line 118 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                 { yylhs.value.as < ast::TypeName > () = {yystack_[4].value.as < std::string > (), yystack_[2].value.as < std::vector<ast::TypeName> > (), true}; }
#line 924 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 7: // TypeNames: TypeName
#line 123 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < std::vector<ast::TypeName> > () = {yystack_[0].value.as < ast::TypeName > ()}; }
#line 930 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 8: // TypeNames: TypeNames Comma TypeName
#line 124 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                               { yystack_[2].value.as < std::vector<ast::TypeName> > ().push_back(yystack_[0].value.as < ast::TypeName > ()); yylhs.value.as < std::vector<ast::TypeName> > () = yystack_[2].value.as < std::vector<ast::TypeName> > (); }
#line 936 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 9: // Expression: KThis Access Identifier
#line 129 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                            { yylhs.value.as < ast::Expression > () = {ast::Expression::ThisAccess{ yystack_[0].value.as < std::string > () } }; }
#line 942 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 10: // Expression: Expression Access Identifier
#line 130 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Expression > () = { ast::Expression::MemberAccess{ std::make_shared<ast::Expression>(yystack_[2].value.as < ast::Expression > ()), yystack_[0].value.as < std::string > () } }; }
#line 948 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 11: // Expression: Expression Access Identifier LParen Expressions RParen
#line 131 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                             { yylhs.value.as < ast::Expression > () = { ast::Expression::MethodCall{ std::make_shared<ast::Expression>(yystack_[5].value.as < ast::Expression > ()), yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 954 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 12: // Expression: KThis Access Identifier LParen Expressions RParen
#line 132 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                        { yylhs.value.as < ast::Expression > () = { ast::Expression::ThisCall{ yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 960 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 13: // Expression: TypeName LParen Expressions RParen
#line 133 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                         { yylhs.value.as < ast::Expression > () = { ast::Expression::ConstructorCall{ yystack_[3].value.as < ast::TypeName > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 966 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 14: // Expression: Identifier
#line 134 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < ast::Expression > () = { ast::Identifier{yystack_[0].value.as < std::string > ()} }; }
#line 972 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 15: // Expression: Literal
#line 135 "/home/kibertod/dev/uni/timur/src/parser.ypp"
              { yylhs.value.as < ast::Expression > () = { yystack_[0].value.as < ast::Expression::Literal > () }; }
#line 978 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 16: // Expressions: %empty
#line 139 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Expression> > () = {}; }
#line 984 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 17: // Expressions: Expression
#line 140 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < std::vector<ast::Expression> > () = {yystack_[0].value.as < ast::Expression > ()}; }
#line 990 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 18: // Expressions: Expressions Comma Expression
#line 141 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yystack_[2].value.as < std::vector<ast::Expression> > ().push_back(yystack_[0].value.as < ast::Expression > ()); yylhs.value.as < std::vector<ast::Expression> > () = yystack_[2].value.as < std::vector<ast::Expression> > (); }
#line 996 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 19: // Literal: LitStr
#line 145 "/home/kibertod/dev/uni/timur/src/parser.ypp"
           { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Str, yystack_[0].value.as < std::string > ()}; }
#line 1002 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 20: // Literal: LitInt
#line 146 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Int, yystack_[0].value.as < std::string > ()}; }
#line 1008 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 21: // Literal: LitReal
#line 147 "/home/kibertod/dev/uni/timur/src/parser.ypp"
              { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Real, yystack_[0].value.as < std::string > ()}; }
#line 1014 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 22: // Literal: KTrue
#line 148 "/home/kibertod/dev/uni/timur/src/parser.ypp"
            { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Bool, "true"}; }
#line 1020 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 23: // Literal: KFalse
#line 149 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Bool, "false"}; }
#line 1026 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 24: // Variable: KVar Identifier Colon TypeName
#line 153 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Variable > () = {yystack_[0].value.as < ast::TypeName > (), yystack_[2].value.as < std::string > (), {}}; }
#line 1032 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 25: // Variable: KVar Identifier Colon TypeName Assign Expression
#line 154 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                       { yylhs.value.as < ast::Variable > () = {yystack_[2].value.as < ast::TypeName > (), yystack_[4].value.as < std::string > (), yystack_[0].value.as < ast::Expression > ()}; }
#line 1038 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 26: // Statement: KReturn Expression
#line 158 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                       { yylhs.value.as < ast::Statement > () = {ast::Statement::Return{yystack_[0].value.as < ast::Expression > ()}}; }
#line 1044 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 27: // Statement: KReturn KVoid
#line 159 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                    { yylhs.value.as < ast::Statement > () = {ast::Statement::Return{{}}}; }
#line 1050 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 28: // Statement: Expression Assign Expression
#line 160 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Statement > () = {ast::Statement::Assignment{yystack_[2].value.as < ast::Expression > (), yystack_[0].value.as < ast::Expression > ()}}; }
#line 1056 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 29: // Statement: Expression
#line 161 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < ast::Statement > () = {yystack_[0].value.as < ast::Expression > ()}; }
#line 1062 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 30: // Statement: Variable
#line 162 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::Statement > () = {yystack_[0].value.as < ast::Variable > ()}; }
#line 1068 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 31: // Statement: KWhile Expression KLoop Statements KEnd
#line 163 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                              { yylhs.value.as < ast::Statement > () = {ast::Statement::While{yystack_[3].value.as < ast::Expression > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1074 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 32: // Statement: KIf Expression KThen Statements ElIfs KEnd
#line 164 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                 { yylhs.value.as < ast::Statement > () = {ast::Statement::If{yystack_[4].value.as < ast::Expression > (), yystack_[2].value.as < std::vector<ast::Statement> > (), yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > (), std::vector<ast::Statement>()}}; }
#line 1080 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 33: // Statement: KIf Expression KThen Statements ElIfs KElse Statements KEnd
#line 165 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                  { yylhs.value.as < ast::Statement > () = {ast::Statement::If{yystack_[6].value.as < ast::Expression > (), yystack_[4].value.as < std::vector<ast::Statement> > (), yystack_[3].value.as < std::vector<ast::Statement::If::ElIf> > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1086 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 34: // Statement: KSuper LParen Expressions RParen
#line 166 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                       { yylhs.value.as < ast::Statement > () = { ast::Statement::SuperCall{ {}, yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 1092 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 35: // Statement: KSuper Access TypeName LParen Expressions RParen
#line 167 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                       { yylhs.value.as < ast::Statement > () = { ast::Statement::SuperCall{ yystack_[3].value.as < ast::TypeName > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 1098 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 36: // Statements: %empty
#line 171 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Statement> > () = {}; }
#line 1104 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 37: // Statements: Statements Statement
#line 172 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                           { yystack_[1].value.as < std::vector<ast::Statement> > ().push_back(yystack_[0].value.as < ast::Statement > ()); yylhs.value.as < std::vector<ast::Statement> > () = yystack_[1].value.as < std::vector<ast::Statement> > (); }
#line 1110 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 38: // ElIf: KElif Expression KThen Statements
#line 176 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                      { yylhs.value.as < ast::Statement::If::ElIf > () = ast::Statement::If::ElIf{yystack_[2].value.as < ast::Expression > (), yystack_[0].value.as < std::vector<ast::Statement> > ()}; }
#line 1116 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 39: // ElIfs: %empty
#line 180 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Statement::If::ElIf> > () = {}; }
#line 1122 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 40: // ElIfs: ElIfs ElIf
#line 181 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > ().push_back(yystack_[0].value.as < ast::Statement::If::ElIf > ()); yylhs.value.as < std::vector<ast::Statement::If::ElIf> > () = yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > (); }
#line 1128 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 41: // Argument: TypeName Colon Identifier
#line 185 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                              { yylhs.value.as < std::pair<ast::TypeName, ast::Identifier> > () = std::pair<ast::TypeName, ast::Identifier>(yystack_[2].value.as < ast::TypeName > (), yystack_[0].value.as < std::string > ()); }
#line 1134 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 42: // Arguments: %empty
#line 189 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = {}; }
#line 1140 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 43: // Arguments: Argument
#line 190 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = {yystack_[0].value.as < std::pair<ast::TypeName, ast::Identifier> > ()}; }
#line 1146 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 44: // Arguments: Arguments Comma Argument
#line 191 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                               { yystack_[2].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > ().push_back(yystack_[0].value.as < std::pair<ast::TypeName, ast::Identifier> > ()); yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = yystack_[2].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (); }
#line 1152 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 45: // MemberDeclaration: TypeName Identifier LParen Arguments RParen KIs Statements KEnd
#line 195 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                    { yylhs.value.as < ast::MemberDeclaration > () = {ast::MemberDeclaration::Method{yystack_[6].value.as < std::string > (), yystack_[7].value.as < ast::TypeName > (), yystack_[4].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1158 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 46: // MemberDeclaration: KVoid Identifier LParen Arguments RParen KIs Statements KEnd
#line 196 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                   { yylhs.value.as < ast::MemberDeclaration > () = {ast::MemberDeclaration::Method{yystack_[6].value.as < std::string > (), {}, yystack_[4].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1164 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 47: // MemberDeclaration: KThis LParen Arguments RParen KIs Statements KEnd
#line 197 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                        { yylhs.value.as < ast::MemberDeclaration > () = {ast::MemberDeclaration::Constructor{ yystack_[4].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (), yystack_[1].value.as < std::vector<ast::Statement> > () }}; }
#line 1170 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 48: // MemberDeclaration: Variable
#line 198 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::MemberDeclaration > () = {yystack_[0].value.as < ast::Variable > ()}; }
#line 1176 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 49: // MemberDeclarations: %empty
#line 202 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::MemberDeclaration> > () = {}; }
#line 1182 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 50: // MemberDeclarations: MemberDeclarations MemberDeclaration
#line 203 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                           { yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ().push_back(yystack_[0].value.as < ast::MemberDeclaration > ()); yylhs.value.as < std::vector<ast::MemberDeclaration> > () = yystack_[1].value.as < std::vector<ast::MemberDeclaration> > (); }
#line 1188 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 51: // Class: KClass TypeName KExtends TypeNames KIs MemberDeclarations KEnd
#line 207 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                   { yylhs.value.as < ast::Class > () = {yystack_[5].value.as < ast::TypeName > (), yystack_[3].value.as < std::vector<ast::TypeName> > (), yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ()}; }
#line 1194 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 52: // Class: KClass TypeName KIs MemberDeclarations KEnd
#line 208 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                  { yylhs.value.as < ast::Class > () = {yystack_[3].value.as < ast::TypeName > (), {}, yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ()}; }
#line 1200 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 53: // Classes: Class
#line 212 "/home/kibertod/dev/uni/timur/src/parser.ypp"
          { yylhs.value.as < std::vector<ast::Class> > () = {yystack_[0].value.as < ast::Class > ()}; }
#line 1206 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 54: // Classes: Classes Class
#line 213 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                    { yystack_[1].value.as < std::vector<ast::Class> > ().push_back(yystack_[0].value.as < ast::Class > ()); yylhs.value.as < std::vector<ast::Class> > () = yystack_[1].value.as < std::vector<ast::Class> > (); }
#line 1212 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;


#line 1216 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"

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
        std::string msg = YY_("syntax error");
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

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char parser::yypact_ninf_ = -86;

  const signed char parser::yytable_ninf_ = -4;

  const short
  parser::yypact_[] =
  {
       0,    11,    43,   -86,     0,   -16,    45,   -86,   -86,   -86,
      11,   -86,    11,   -86,    55,    49,    31,    11,    27,    35,
      63,   -86,    65,    76,   -86,   -86,   -86,   -86,   -86,    73,
      86,    11,    93,   185,    11,    11,    92,   -86,    80,    11,
     -86,   106,   115,   107,   136,    11,   121,   143,     9,   -86,
     -86,   -86,   156,   -86,   118,   -86,   -86,    82,   -86,   -86,
     -86,   135,   139,   -86,    61,   -86,    83,   154,     9,   178,
       9,     9,   -86,   128,     5,   149,   -86,   -86,   104,   -86,
     162,   139,   148,   179,    39,    -4,    11,     9,   -86,   139,
       9,   -86,     9,   -86,     9,     9,   -86,   -86,   180,   160,
     139,   165,   139,   166,   125,   164,     9,   -86,   -86,   -86,
     -86,   116,   174,   -86,     9,   -86,   -86,   -86,   146,    -3,
     -86,   -86,   164
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,    53,     2,     3,     0,     1,    54,     4,
       0,    49,     0,     7,     0,     0,     0,     0,     5,     0,
       0,    52,     0,     0,    48,    50,    49,     8,     6,     0,
       0,    42,     0,     0,    42,     0,     0,    43,     0,    42,
      51,     0,    24,     0,     0,     0,     0,     0,     0,    41,
      36,    44,     0,    36,     0,    22,    23,    14,    19,    20,
      21,     0,    25,    15,     0,    36,     0,     0,    16,     0,
       0,     0,    47,     0,     0,    29,    30,    37,     0,    46,
       9,    17,     0,    10,     0,     0,     0,    16,    27,    26,
       0,    45,    16,    13,     0,    16,    36,    36,     0,     0,
      28,     0,    18,     0,     0,    39,    16,    34,    12,    11,
      31,     0,     0,    36,     0,    32,    40,    35,     0,     0,
      33,    36,    38
  };

  const short
  parser::yypgoto_[] =
  {
     -86,   -86,    -1,   190,   -20,   -85,   -86,   -14,   -86,   -50,
     -86,   -86,   161,    23,   -86,   181,   204,   -86
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,    61,    14,    75,    82,    63,    76,    77,    64,
     116,   111,    37,    38,    25,    15,     3,     4
  };

  const signed char
  parser::yytable_[] =
  {
       6,    24,    99,    66,    97,   121,     9,   101,    88,    13,
     103,    13,    10,     1,    23,    78,    27,    69,    69,    24,
      54,   112,    55,    56,    54,    57,    55,    56,    62,    57,
      36,     5,    23,    36,    42,    58,    59,    60,    36,    58,
      59,    60,    26,     7,    36,    96,   104,   105,    81,    28,
      84,    85,    19,    20,    89,    29,    11,    41,    17,    12,
      69,    21,    46,   118,    22,    20,    70,    81,    71,     5,
     100,   122,    81,    72,   102,    81,    54,    73,    55,    56,
      74,    57,    17,    30,    18,    98,    81,    20,    70,    31,
      71,    58,    59,    60,   119,    79,    32,    34,    54,    73,
      55,    56,    74,    57,     9,    44,    -3,    45,    20,    70,
      10,    71,    35,    58,    59,    60,    91,    39,    43,    54,
      73,    55,    56,    74,    57,   113,   114,    49,   115,    20,
      70,    47,    71,    45,    58,    59,    60,   110,    48,    67,
      54,    73,    55,    56,    74,    57,    52,    50,    45,    86,
      20,    70,    87,    71,    53,    58,    59,    60,   120,    68,
      69,    54,    73,    55,    56,    74,    57,    65,    20,    70,
      69,    71,    90,    93,    80,    94,    58,    59,    60,    54,
      73,    55,    56,    74,    57,   107,    92,    94,    19,    20,
     108,   109,    94,    94,    58,    59,    60,    40,    83,   117,
      22,    94,    16,    95,   106,     5,    51,    33,     8
  };

  const signed char
  parser::yycheck_[] =
  {
       1,    15,    87,    53,     8,     8,    22,    92,     3,    10,
      95,    12,    28,    13,    15,    65,    17,    21,    21,    33,
      15,   106,    17,    18,    15,    20,    17,    18,    48,    20,
      31,    20,    33,    34,    35,    30,    31,    32,    39,    30,
      31,    32,    11,     0,    45,     6,    96,    97,    68,    22,
      70,    71,     3,     4,    74,    20,    11,    34,    27,    14,
      21,    12,    39,   113,    15,     4,     5,    87,     7,    20,
      90,   121,    92,    12,    94,    95,    15,    16,    17,    18,
      19,    20,    27,    20,    29,    86,   106,     4,     5,    24,
       7,    30,    31,    32,   114,    12,    20,    24,    15,    16,
      17,    18,    19,    20,    22,    25,    24,    27,     4,     5,
      28,     7,    26,    30,    31,    32,    12,    24,    26,    15,
      16,    17,    18,    19,    20,     9,    10,    20,    12,     4,
       5,    25,     7,    27,    30,    31,    32,    12,    23,    21,
      15,    16,    17,    18,    19,    20,    25,    11,    27,    21,
       4,     5,    24,     7,    11,    30,    31,    32,    12,    24,
      21,    15,    16,    17,    18,    19,    20,    11,     4,     5,
      21,     7,    23,    25,    20,    27,    30,    31,    32,    15,
      16,    17,    18,    19,    20,    25,    24,    27,     3,     4,
      25,    25,    27,    27,    30,    31,    32,    12,    20,    25,
      15,    27,    12,    24,    24,    20,    45,    26,     4
  };

  const signed char
  parser::yystos_[] =
  {
       0,    13,    35,    50,    51,    20,    36,     0,    50,    22,
      28,    11,    14,    36,    37,    49,    37,    27,    29,     3,
       4,    12,    15,    36,    41,    48,    11,    36,    22,    20,
      20,    24,    20,    49,    24,    26,    36,    46,    47,    24,
      12,    47,    36,    26,    25,    27,    47,    25,    23,    20,
      11,    46,    25,    11,    15,    17,    18,    20,    30,    31,
      32,    36,    38,    40,    43,    11,    43,    21,    24,    21,
       5,     7,    12,    16,    19,    38,    41,    42,    43,    12,
      20,    38,    39,    20,    38,    38,    21,    24,     3,    38,
      23,    12,    24,    25,    27,    24,     6,     8,    36,    39,
      38,    39,    38,    39,    43,    43,    24,    25,    25,    25,
      12,    45,    39,     9,    10,    12,    44,    25,    43,    38,
      12,     8,    43
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    34,    35,    36,    36,    36,    36,    37,    37,    38,
      38,    38,    38,    38,    38,    38,    39,    39,    39,    40,
      40,    40,    40,    40,    41,    41,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    43,    43,    44,    45,
      45,    46,    47,    47,    47,    48,    48,    48,    48,    49,
      49,    50,    50,    51,    51
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     2,     4,     5,     1,     3,     3,
       3,     6,     6,     4,     1,     1,     0,     1,     3,     1,
       1,     1,     1,     1,     4,     6,     2,     2,     3,     1,
       1,     5,     6,     8,     4,     6,     0,     2,     4,     0,
       2,     3,     0,     1,     3,     8,     8,     7,     1,     0,
       2,     7,     5,     1,     2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "KVoid", "KVar",
  "KWhile", "KLoop", "KIf", "KThen", "KElse", "KElif", "KIs", "KEnd",
  "KClass", "KExtends", "KThis", "KSuper", "KTrue", "KFalse", "KReturn",
  "Identifier", "Access", "Ptr", "Assign", "LParen", "RParen", "Colon",
  "Comma", "LBracket", "RBracket", "LitStr", "LitInt", "LitReal", "Err",
  "$accept", "Program", "TypeName", "TypeNames", "Expression",
  "Expressions", "Literal", "Variable", "Statement", "Statements", "ElIf",
  "ElIfs", "Argument", "Arguments", "MemberDeclaration",
  "MemberDeclarations", "Class", "Classes", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   111,   111,   115,   116,   117,   118,   123,   124,   129,
     130,   131,   132,   133,   134,   135,   139,   140,   141,   145,
     146,   147,   148,   149,   153,   154,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   171,   172,   176,   180,
     181,   185,   189,   190,   191,   195,   196,   197,   198,   202,
     203,   207,   208,   212,   213
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
#line 1616 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"

#line 216 "/home/kibertod/dev/uni/timur/src/parser.ypp"

namespace yy
{
    void parser::error(const location& loc, const std::string& msg)
    {
        std::cerr << "Error at " 
                  << loc.begin.line << ":" << loc.begin.column
                  << " - " << msg << "\n" << yy::point_err(ctx, loc.begin.line, loc.begin.column);
    }
}
