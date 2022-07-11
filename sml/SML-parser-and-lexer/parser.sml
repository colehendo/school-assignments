(***********************************************************)
(*       LISP Parser                                       *)
(*                                                         *)
(*       Cole Henderson                                    *)
(*       Final code, COMP 360                              *)
(*                                                         *)
(***********************************************************)

exception LexerError;
exception ParseOK;
exception ParseError of string;

(***********************************************************)
(* type declarations                                       *)
(***********************************************************)

datatype sign =
    Plus
  | Minus
;

datatype atom =
  T
| NIL
| Int of int
| Ident of string
;

datatype token =
  Lparen
| Rparen
| Dot
| Sign of sign
| Atom of atom
;

datatype sexp =
    AtomExp of atom
  | Sexp of sexp * sexp;


(***********************************************************)
(* printing functions                                      *)
(***********************************************************)

(* function: print_tokens - prints out a token stream  *)
fun print_tokens [] = print("\n")
  | print_tokens (Lparen :: t) = (print("Lparen "); print_tokens(t))
  | print_tokens (Rparen :: t) = (print("Rparen "); print_tokens(t))
  | print_tokens (Dot :: t) = (print("Dot "); print_tokens(t))
  | print_tokens (Sign(Plus) :: t) = (print("Plus "); print_tokens(t))
  | print_tokens (Sign(Minus) :: t) = (print("Minus "); print_tokens(t))
  | print_tokens (Atom(a) :: t) =
  (case a of
   T => (print("Atom(T) "); print_tokens(t))
   | NIL => (print("Atom(NIL) "); print_tokens(t))
   | Int i => (print("Atom(Int(" ^ Int.toString(i) ^ ")) "); print_tokens(t))
   | Ident s => (print("Atom(Ident(" ^ s ^ ")) "); print_tokens(t)))
;

(* function: string_of_op -  converts an operator token to a string *)
fun string_of_op Plus = "+"
 |  string_of_op Minus = "-";


(* function: is_list - predicate function returning true if s-expression is a list *)
fun is_list (Sexp(h, AtomExp(NIL))) = true
 |  is_list (Sexp(h, t)) = is_list t
 |  is_list _ = false;


(* function: string_of_atom - converts a primitive atom to a string *)
fun string_of_atom (T) = print("T")
  | string_of_atom (NIL) = print("NIL")
  | string_of_atom (Int(i)) = print(Int.toString(i))
  | string_of_atom (Ident(s)) = print(s)
;


(* function: print_list - prints an s-expression in list format *)
fun print_list (Sexp(h, AtomExp(NIL))) = (print_sexp h;
                                          print(")"))
  | print_list (Sexp(h, t)) =
      (print_sexp h;
       print(" ");
       print_list t)


(* function: print_sexp - prints an s-expression in either dotted or list format *)
and
    print_sexp s =
      if (is_list s) = true
      then (print("("); print_list s)
      else
        (case s of
         AtomExp(a) => string_of_atom a
         | Sexp(h, t) => (print("(");
                          print_sexp h;
                          print(" . ");
                          print_sexp t;
                          print(")")))
;


(***********************************************************)
(* lexer implementation                                    *)
(***********************************************************)

fun spaces (#" " :: t)  = spaces t
  | spaces (#"\t" :: t) = spaces t
  | spaces (#"\n" :: t) = spaces t
  | spaces l = l
;

fun char_to_int(c) =
   let
      val copt = Int.fromString(Char.toString(c))
   in
      (case copt of
        SOME(vv) => vv
      | NONE => raise LexerError)
   end
;


fun lexid (s, []) = (s, [])
  | lexid (s, h::t) =
      if Char.isAlphaNum(h) then
        lexid(s ^ Char.toString(h), t)
      else
        (s, h::t)
;


fun lexint (v, []) = (v, [])
  | lexint (v, h::t) =
  if Char.isDigit(h) then
     lexint((10*v)+char_to_int(h), t)
  else
     (v, h::t)
;


fun  lexer( #"(" :: t) =   Lparen :: lexer(t)
  |  lexer( #")" :: t) =  Rparen :: lexer(t)
  |  lexer( #"." :: t) =  Dot :: lexer(t)
  |  lexer( #"-" :: t) =  Sign(Minus) :: lexer(t)
  |  lexer( #"+" :: t) =  Sign(Plus) :: lexer(t)
  |  lexer( h::t ) =
        if Char.isAlpha(h) then
          let
             val (idstr,tt) = lexid(Char.toString(h), t)
          in
            (case (String.map Char.toLower idstr) of
                  "nil" => Atom(NIL) :: lexer (tt)
                | "t"   => Atom(T) :: lexer (tt)
                | _   => Atom(Ident(idstr)) :: lexer(tt))
          end
        else if Char.isDigit(h) then
          let
             val (intval, tt) = lexint(char_to_int(h), t)
          in
             Atom(Int(intval)) :: lexer(tt)
          end
        else if Char.isSpace(h) then
          lexer(spaces(t))
        else
          raise LexerError
   |   lexer [] = []
;


(***********************************************************)
(* parser implementation                                   *)
(***********************************************************)

(* function: check_sign - both validates and combines sign and integer token pairs *)
fun check_sign (Sign(Minus)::(Atom(Int(i)))::rest) = (AtomExp(Int(~i)),rest)
 |  check_sign (Sign(Plus)::(Atom(Int(i)))::rest) = (AtomExp(Int(i)),rest)
 |  check_sign _ = raise ParseError "+/- sign may only be used with integer literals";
;

(* function: parse_sexp - top-level parser: takes stream of tokens, returns sexp-tree *)
(* S ::= E *)
fun parse_sexp [] = raise ParseOK
 |  parse_sexp exp = parse_exp exp

(* E ::= atom | '(' X          *)
and
    parse_exp [] = raise ParseOK
  | parse_exp ((Sign(a))::(Atom(Int(i))::ts)) =
      let val (cs, ts) = check_sign ((Sign(a))::(Atom(Int(i)))::ts)
      in
        (cs, ts)
      end
  | parse_exp (Atom(a)::ts) = (AtomExp(a), ts)
  | parse_exp (Lparen::ts) =
      let val (x, xts) = parse_x ts
      in
        (x, xts)
      end

(* X ::= E Y | ')'   *)
and
    parse_x [] = raise ParseOK
  | parse_x (Rparen::ts) = (AtomExp(NIL), ts)
  | parse_x (t::ts) =
      let
        val (e, ets) = parse_exp (t::ts)
        val (y, yts) = parse_y ets
        val (rts) = parse_rparen yts
      in
        (Sexp(e, y), rts)
      end

(* Y ::= '.' E ')' | R ')'    *)
and
    parse_y [] = raise ParseOK
  | parse_y (Dot::ts) = parse_exp ts
  | parse_y (ts) =
      let val (r, rts) = parse_r ts
      in
        (r, rts)
      end

(* R ::= E R | empty  *)
and
    parse_r [] = raise ParseOK
  | parse_r (Atom(a)::ts) =
      let
        val (e, ets) = parse_exp (Atom(a)::ts)
        val (r, rts) = parse_r ets
      in
        (Sexp(e, r), rts)
      end
  | parse_r (Lparen::ts) =
      let
        val (e, ets) = parse_exp (Lparen::ts)
        val (r, rts) = parse_r ets
      in
        (Sexp(e, r), rts)
      end
  | parse_r ((Sign(a))::(Atom(Int(i))::ts)) =
      let
        val (e, ets) = parse_exp ((Sign(a))::(Atom(Int(i))::ts))
        val (r, rts) = parse_r ets
      in
        (Sexp(e, r), rts)
      end
  | parse_r (ts) = (AtomExp(NIL), ts)


and
    parse_rparen (Rparen::ts) = ts
  | parse_rparen _ = raise ParseError "not an Rparen";
;


(*****************************************)
(* helper routines                       *)
(*****************************************)

fun get_sexp [] = (AtomExp(NIL),[])
 |  get_sexp s = parse_sexp s
;

fun next_sexp [] = OS.Process.exit(OS.Process.success)
 | next_sexp s =
   let
      val (e,rest) = get_sexp s
   in
      (print_sexp e;
       print "\n";
       next_sexp rest
       handle ParseError msg => print ("Parse Error: " ^ msg ^ "\n")
            | ParseOK => OS.Process.exit(OS.Process.success))
   end
;

fun reader(copt: char option, is, l) =
  case copt of
    NONE    => (TextIO.closeIn is; l)
  | SOME(c) => reader (TextIO.input1 is, is, (l@[c]))
;


(*****************************************)
(* main                                  *)
(*****************************************)
val args = CommandLine.arguments();
val ins = TextIO.openIn(hd(args));

let
   val (sexp,ts) = get_sexp(lexer(reader(TextIO.input1 ins, ins, [])));
in
   (print_sexp(sexp);
    print "\n";
    next_sexp ts)
end;