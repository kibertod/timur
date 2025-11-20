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
#line 109 "/home/kibertod/dev/uni/timur/src/parser.ypp"
            { ctx.root = yystack_[0].value.as < std::vector<ast::Class> > (); }
#line 900 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 3: // TypeName: Identifier
#line 113 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::TypeName > () = {yystack_[0].value.as < std::string > (), {}}; }
#line 906 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 4: // TypeName: Identifier LBracket TypeNames RBracket
#line 114 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                             { yylhs.value.as < ast::TypeName > () = {yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::TypeName> > ()}; }
#line 912 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 5: // TypeNames: TypeName
#line 119 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < std::vector<ast::TypeName> > () = {yystack_[0].value.as < ast::TypeName > ()}; }
#line 918 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 6: // TypeNames: TypeNames Comma TypeName
#line 120 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                               { yystack_[2].value.as < std::vector<ast::TypeName> > ().push_back(yystack_[0].value.as < ast::TypeName > ()); yylhs.value.as < std::vector<ast::TypeName> > () = yystack_[2].value.as < std::vector<ast::TypeName> > (); }
#line 924 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 7: // Expression: KThis Access Identifier
#line 125 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                            { yylhs.value.as < ast::Expression > () = {ast::Expression::ThisAccess{ yystack_[0].value.as < std::string > () } }; }
#line 930 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 8: // Expression: Expression Access Identifier
#line 126 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Expression > () = { ast::Expression::MemberAccess{ std::make_shared<ast::Expression>(yystack_[2].value.as < ast::Expression > ()), yystack_[0].value.as < std::string > () } }; }
#line 936 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 9: // Expression: Expression Access Identifier LParen Expressions RParen
#line 127 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                             { yylhs.value.as < ast::Expression > () = { ast::Expression::MethodCall{ std::make_shared<ast::Expression>(yystack_[5].value.as < ast::Expression > ()), yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 942 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 10: // Expression: KThis Access Identifier LParen Expressions RParen
#line 128 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                        { yylhs.value.as < ast::Expression > () = { ast::Expression::ThisCall{ yystack_[3].value.as < std::string > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 948 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 11: // Expression: TypeName LParen Expressions RParen
#line 129 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                         { yylhs.value.as < ast::Expression > () = { ast::Expression::ConstructorCall{ yystack_[3].value.as < ast::TypeName > (), yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 954 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 12: // Expression: Identifier
#line 130 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < ast::Expression > () = { ast::Identifier{yystack_[0].value.as < std::string > ()} }; }
#line 960 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 13: // Expression: Literal
#line 131 "/home/kibertod/dev/uni/timur/src/parser.ypp"
              { yylhs.value.as < ast::Expression > () = { yystack_[0].value.as < ast::Expression::Literal > () }; }
#line 966 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 14: // Expression: LParen Expression RParen
#line 132 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                               { yylhs.value.as < ast::Expression > () = yystack_[1].value.as < ast::Expression > (); }
#line 972 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 15: // Expressions: %empty
#line 136 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Expression> > () = {}; }
#line 978 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 16: // Expressions: Expression
#line 137 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < std::vector<ast::Expression> > () = {yystack_[0].value.as < ast::Expression > ()}; }
#line 984 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 17: // Expressions: Expressions Comma Expression
#line 138 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yystack_[2].value.as < std::vector<ast::Expression> > ().push_back(yystack_[0].value.as < ast::Expression > ()); yylhs.value.as < std::vector<ast::Expression> > () = yystack_[2].value.as < std::vector<ast::Expression> > (); }
#line 990 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 18: // Literal: LitStr
#line 142 "/home/kibertod/dev/uni/timur/src/parser.ypp"
           { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Str, yystack_[0].value.as < std::string > ()}; }
#line 996 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 19: // Literal: LitInt
#line 143 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Int, yystack_[0].value.as < std::string > ()}; }
#line 1002 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 20: // Literal: LitReal
#line 144 "/home/kibertod/dev/uni/timur/src/parser.ypp"
              { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Real, yystack_[0].value.as < std::string > ()}; }
#line 1008 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 21: // Literal: KTrue
#line 145 "/home/kibertod/dev/uni/timur/src/parser.ypp"
            { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Bool, "true"}; }
#line 1014 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 22: // Literal: KFalse
#line 146 "/home/kibertod/dev/uni/timur/src/parser.ypp"
             { yylhs.value.as < ast::Expression::Literal > () = {ast::Expression::Literal::Type::Bool, "false"}; }
#line 1020 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 23: // Variable: KVar Identifier Colon TypeName
#line 150 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Variable > () = {yystack_[0].value.as < ast::TypeName > (), yystack_[2].value.as < std::string > (), {}}; }
#line 1026 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 24: // Variable: KVar Identifier Colon TypeName Assign Expression
#line 151 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                       { yylhs.value.as < ast::Variable > () = {yystack_[2].value.as < ast::TypeName > (), yystack_[4].value.as < std::string > (), yystack_[0].value.as < ast::Expression > ()}; }
#line 1032 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 25: // Statement: KReturn Expression
#line 155 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                       { yylhs.value.as < ast::Statement > () = {ast::Statement::Return{yystack_[0].value.as < ast::Expression > ()}}; }
#line 1038 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 26: // Statement: Expression Assign Expression
#line 156 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                   { yylhs.value.as < ast::Statement > () = {ast::Statement::Assignment{yystack_[2].value.as < ast::Expression > (), yystack_[0].value.as < ast::Expression > ()}}; }
#line 1044 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 27: // Statement: Expression
#line 157 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yylhs.value.as < ast::Statement > () = {yystack_[0].value.as < ast::Expression > ()}; }
#line 1050 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 28: // Statement: Variable
#line 158 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::Statement > () = {yystack_[0].value.as < ast::Variable > ()}; }
#line 1056 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 29: // Statement: KWhile Expression KLoop Statements KEnd
#line 159 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                              { yylhs.value.as < ast::Statement > () = {ast::Statement::While{yystack_[3].value.as < ast::Expression > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1062 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 30: // Statement: KIf Expression KThen Statements ElIfs KEnd
#line 160 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                 { yylhs.value.as < ast::Statement > () = {ast::Statement::If{yystack_[4].value.as < ast::Expression > (), yystack_[2].value.as < std::vector<ast::Statement> > (), yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > (), std::vector<ast::Statement>()}}; }
#line 1068 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 31: // Statement: KIf Expression KThen Statements ElIfs KElse Statements KEnd
#line 161 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                  { yylhs.value.as < ast::Statement > () = {ast::Statement::If{yystack_[6].value.as < ast::Expression > (), yystack_[4].value.as < std::vector<ast::Statement> > (), yystack_[3].value.as < std::vector<ast::Statement::If::ElIf> > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1074 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 32: // Statement: KSuper LParen Expressions RParen
#line 162 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                       { yylhs.value.as < ast::Statement > () = { ast::Statement::SuperCall{ yystack_[1].value.as < std::vector<ast::Expression> > () } }; }
#line 1080 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 33: // Statements: %empty
#line 166 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Statement> > () = {}; }
#line 1086 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 34: // Statements: Statements Statement
#line 167 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                           { yystack_[1].value.as < std::vector<ast::Statement> > ().push_back(yystack_[0].value.as < ast::Statement > ()); yylhs.value.as < std::vector<ast::Statement> > () = yystack_[1].value.as < std::vector<ast::Statement> > (); }
#line 1092 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 35: // ElIf: KElif Expression KThen Statements
#line 171 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                      { yylhs.value.as < ast::Statement::If::ElIf > () = ast::Statement::If::ElIf{yystack_[2].value.as < ast::Expression > (), yystack_[0].value.as < std::vector<ast::Statement> > ()}; }
#line 1098 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 36: // ElIfs: %empty
#line 175 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::Statement::If::ElIf> > () = {}; }
#line 1104 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 37: // ElIfs: ElIfs ElIf
#line 176 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                 { yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > ().push_back(yystack_[0].value.as < ast::Statement::If::ElIf > ()); yylhs.value.as < std::vector<ast::Statement::If::ElIf> > () = yystack_[1].value.as < std::vector<ast::Statement::If::ElIf> > (); }
#line 1110 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 38: // Argument: TypeName Colon Identifier
#line 180 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                              { yylhs.value.as < std::pair<ast::TypeName, ast::Identifier> > () = std::pair<ast::TypeName, ast::Identifier>(yystack_[2].value.as < ast::TypeName > (), yystack_[0].value.as < std::string > ()); }
#line 1116 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 39: // Arguments: %empty
#line 184 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = {}; }
#line 1122 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 40: // Arguments: Argument
#line 185 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = {yystack_[0].value.as < std::pair<ast::TypeName, ast::Identifier> > ()}; }
#line 1128 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 41: // Arguments: Arguments Comma Argument
#line 186 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                               { yystack_[2].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > ().push_back(yystack_[0].value.as < std::pair<ast::TypeName, ast::Identifier> > ()); yylhs.value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > () = yystack_[2].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (); }
#line 1134 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 42: // MemberDeclaration: TypeName Identifier LParen Arguments RParen KIs Statements KEnd
#line 190 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                    { yylhs.value.as < ast::MemberDeclaration > () = {ast::MemberDeclaration::Method{yystack_[6].value.as < std::string > (), yystack_[7].value.as < ast::TypeName > (), yystack_[4].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (), yystack_[1].value.as < std::vector<ast::Statement> > ()}}; }
#line 1140 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 43: // MemberDeclaration: KThis LParen Arguments RParen KIs Statements KEnd
#line 191 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                        { yylhs.value.as < ast::MemberDeclaration > () = {ast::MemberDeclaration::Constructor{ yystack_[4].value.as < std::vector<std::pair<ast::TypeName, ast::Identifier>> > (), yystack_[1].value.as < std::vector<ast::Statement> > () }}; }
#line 1146 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 44: // MemberDeclaration: Variable
#line 192 "/home/kibertod/dev/uni/timur/src/parser.ypp"
               { yylhs.value.as < ast::MemberDeclaration > () = {yystack_[0].value.as < ast::Variable > ()}; }
#line 1152 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 45: // MemberDeclarations: %empty
#line 196 "/home/kibertod/dev/uni/timur/src/parser.ypp"
    { yylhs.value.as < std::vector<ast::MemberDeclaration> > () = {}; }
#line 1158 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 46: // MemberDeclarations: MemberDeclarations MemberDeclaration
#line 197 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                           { yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ().push_back(yystack_[0].value.as < ast::MemberDeclaration > ()); yylhs.value.as < std::vector<ast::MemberDeclaration> > () = yystack_[1].value.as < std::vector<ast::MemberDeclaration> > (); }
#line 1164 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 47: // Class: KClass TypeName KExtends Identifier KIs MemberDeclarations KEnd
#line 201 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                                    { yylhs.value.as < ast::Class > () = {yystack_[5].value.as < ast::TypeName > (), ast::Identifier {yystack_[3].value.as < std::string > ()}, yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ()}; }
#line 1170 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 48: // Class: KClass TypeName KIs MemberDeclarations KEnd
#line 202 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                                                  { yylhs.value.as < ast::Class > () = {yystack_[3].value.as < ast::TypeName > (), {}, yystack_[1].value.as < std::vector<ast::MemberDeclaration> > ()}; }
#line 1176 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 49: // Classes: Class
#line 206 "/home/kibertod/dev/uni/timur/src/parser.ypp"
          { yylhs.value.as < std::vector<ast::Class> > () = {yystack_[0].value.as < ast::Class > ()}; }
#line 1182 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;

  case 50: // Classes: Classes Class
#line 207 "/home/kibertod/dev/uni/timur/src/parser.ypp"
                    { yystack_[1].value.as < std::vector<ast::Class> > ().push_back(yystack_[0].value.as < ast::Class > ()); yylhs.value.as < std::vector<ast::Class> > () = yystack_[1].value.as < std::vector<ast::Class> > (); }
#line 1188 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"
    break;


#line 1192 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"

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









  const signed char parser::yypact_ninf_ = -56;

  const signed char parser::yytable_ninf_ = -4;

  const short
  parser::yypact_[] =
  {
      19,    33,    65,   -56,    19,    11,    38,   -56,   -56,    33,
     -56,    53,   -56,   -19,    16,    63,    33,   -56,    73,   -56,
      71,    85,   -56,   -56,   -56,   -56,    81,    33,    90,   149,
      33,    89,   -56,    18,    33,   -56,   103,   106,   140,    33,
      39,   -12,   -56,   -56,   -56,   144,   118,   -56,   -56,   -11,
     -12,   -56,   -56,   -56,   133,   141,   -56,    52,   -56,   146,
      37,   -12,   147,   -12,   -12,   -56,   145,   -12,    64,   -56,
     -56,    72,   148,   -56,   141,    54,   150,     8,    -6,   -12,
     141,   -12,   -56,   -12,   -56,   -12,   -12,   -56,   -56,    74,
     141,    94,   141,   139,    92,   129,   -56,   -56,   -56,   -56,
     128,   -56,   -12,   -56,   -56,   112,     5,   -56,   -56,   129
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,    49,     2,     3,     0,     1,    50,     0,
      45,     0,     5,     0,     0,     0,     0,     4,     0,    48,
       0,     0,    44,    46,    45,     6,     0,    39,     0,     0,
       0,     0,    40,     0,    39,    47,    23,     0,     0,     0,
       0,     0,    38,    33,    41,     0,     0,    21,    22,    12,
       0,    18,    19,    20,     0,    24,    13,     0,    33,     0,
       0,    15,     0,     0,     0,    43,     0,     0,    27,    28,
      34,     0,     7,    14,    16,     0,     8,     0,     0,    15,
      25,     0,    42,    15,    11,     0,    15,    33,    33,     0,
      26,     0,    17,     0,     0,    36,    32,    10,     9,    29,
       0,    33,     0,    30,    37,     0,     0,    31,    33,    35
  };

  const short
  parser::yypgoto_[] =
  {
     -56,   -56,    20,   -56,   -41,    70,   -56,    10,   -56,   -55,
     -56,   -56,   130,   137,   -56,   151,   169,   -56
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,    54,    13,    68,    75,    56,    69,    70,    57,
     104,   100,    32,    33,    23,    14,     3,     4
  };

  const signed char
  parser::yytable_[] =
  {
      55,    88,    46,    71,    47,    48,    16,    49,    17,    60,
      50,    -3,   108,    87,    62,     9,    51,    52,    53,    18,
      74,     6,    77,    78,    22,    62,    80,    19,    62,    12,
      20,     1,    94,    95,    21,     5,    25,     9,    74,    22,
      90,    38,    74,    39,    92,    74,   105,    31,    10,    21,
      36,    11,     5,   109,    31,    18,    63,    62,    64,    31,
      73,   106,    45,    65,    39,     7,    46,    66,    47,    48,
      67,    49,    15,    24,    50,    18,    63,    84,    64,    85,
      51,    52,    53,    82,    62,    81,    46,    66,    47,    48,
      67,    49,    26,    27,    50,    18,    63,    96,    64,    85,
      51,    52,    53,    99,    28,    30,    46,    66,    47,    48,
      67,    49,    34,    37,    50,    18,    63,    97,    64,    85,
      51,    52,    53,   107,    41,    42,    46,    66,    47,    48,
      67,    49,    18,    63,    50,    64,   101,   102,    59,   103,
      51,    52,    53,    46,    66,    47,    48,    67,    49,    89,
      43,    50,    18,    91,    58,    61,    93,    51,    52,    53,
      35,    62,    98,    20,    85,    72,    76,    79,     5,    44,
      83,    40,    86,     8,     0,    29
  };

  const signed char
  parser::yycheck_[] =
  {
      41,     7,    14,    58,    16,    17,    25,    19,    27,    50,
      22,    22,     7,     5,    20,    26,    28,    29,    30,     3,
      61,     1,    63,    64,    14,    20,    67,    11,    20,     9,
      14,    12,    87,    88,    14,    19,    16,    26,    79,    29,
      81,    23,    83,    25,    85,    86,   101,    27,    10,    29,
      30,    13,    19,   108,    34,     3,     4,    20,     6,    39,
      23,   102,    23,    11,    25,     0,    14,    15,    16,    17,
      18,    19,    19,    10,    22,     3,     4,    23,     6,    25,
      28,    29,    30,    11,    20,    21,    14,    15,    16,    17,
      18,    19,    19,    22,    22,     3,     4,    23,     6,    25,
      28,    29,    30,    11,    19,    24,    14,    15,    16,    17,
      18,    19,    22,    24,    22,     3,     4,    23,     6,    25,
      28,    29,    30,    11,    21,    19,    14,    15,    16,    17,
      18,    19,     3,     4,    22,     6,     8,     9,    20,    11,
      28,    29,    30,    14,    15,    16,    17,    18,    19,    79,
      10,    22,     3,    83,    10,    22,    86,    28,    29,    30,
      11,    20,    23,    14,    25,    19,    19,    22,    19,    39,
      22,    34,    22,     4,    -1,    24
  };

  const signed char
  parser::yystos_[] =
  {
       0,    12,    33,    48,    49,    19,    34,     0,    48,    26,
      10,    13,    34,    35,    47,    19,    25,    27,     3,    11,
      14,    34,    39,    46,    10,    34,    19,    22,    19,    47,
      24,    34,    44,    45,    22,    11,    34,    24,    23,    25,
      45,    21,    19,    10,    44,    23,    14,    16,    17,    19,
      22,    28,    29,    30,    34,    36,    38,    41,    10,    20,
      36,    22,    20,     4,     6,    11,    15,    18,    36,    39,
      40,    41,    19,    23,    36,    37,    19,    36,    36,    22,
      36,    21,    11,    22,    23,    25,    22,     5,     7,    37,
      36,    37,    36,    37,    41,    41,    23,    23,    23,    11,
      43,     8,     9,    11,    42,    41,    36,    11,     7,    41
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    32,    33,    34,    34,    35,    35,    36,    36,    36,
      36,    36,    36,    36,    36,    37,    37,    37,    38,    38,
      38,    38,    38,    39,    39,    40,    40,    40,    40,    40,
      40,    40,    40,    41,    41,    42,    43,    43,    44,    45,
      45,    45,    46,    46,    46,    47,    47,    48,    48,    49,
      49
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     4,     1,     3,     3,     3,     6,
       6,     4,     1,     1,     3,     0,     1,     3,     1,     1,
       1,     1,     1,     4,     6,     2,     3,     1,     1,     5,
       6,     8,     4,     0,     2,     4,     0,     2,     3,     0,
       1,     3,     8,     7,     1,     0,     2,     7,     5,     1,
       2
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "KVar", "KWhile",
  "KLoop", "KIf", "KThen", "KElse", "KElif", "KIs", "KEnd", "KClass",
  "KExtends", "KThis", "KSuper", "KTrue", "KFalse", "KReturn",
  "Identifier", "Access", "Assign", "LParen", "RParen", "Colon", "Comma",
  "LBracket", "RBracket", "LitStr", "LitInt", "LitReal", "Err", "$accept",
  "Program", "TypeName", "TypeNames", "Expression", "Expressions",
  "Literal", "Variable", "Statement", "Statements", "ElIf", "ElIfs",
  "Argument", "Arguments", "MemberDeclaration", "MemberDeclarations",
  "Class", "Classes", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   109,   109,   113,   114,   119,   120,   125,   126,   127,
     128,   129,   130,   131,   132,   136,   137,   138,   142,   143,
     144,   145,   146,   150,   151,   155,   156,   157,   158,   159,
     160,   161,   162,   166,   167,   171,   175,   176,   180,   184,
     185,   186,   190,   191,   192,   196,   197,   201,   202,   206,
     207
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
#line 1580 "/home/kibertod/dev/uni/timur/src/parser.tab.cpp"

#line 210 "/home/kibertod/dev/uni/timur/src/parser.ypp"

namespace yy
{
    void parser::error(const location& loc, const std::string& msg)
    {
        std::cerr << "Error at " 
                  << loc.begin.line << ":" << loc.begin.column
                  << " - " << msg << "\n" << yy::point_err(ctx, loc.begin.line, loc.begin.column);
    }
}
