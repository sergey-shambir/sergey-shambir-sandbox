--------------------------------------------------------------------------------
---------- Premium software for duck driving and online interpreting. ----------
--------------------------------------------------------------------------------

--------------------- Interpreter grammar --------------------------------------
expression ::= <command> | <sequence> | <repetition>
sequence ::= <expression> ‘;’ <expression>
command ::= right | quack | fly
repetition ::= while ‘(’ <variable> ‘)’ { <expression> }
variable ::= [A-Z,a-z]+

---------------------- Code examples -------------------------------------------
1. Rotate right and fly all day, quack at dusk:

    right; while (daylight) fly; quack;

2. Rotate while not faced west

    WHILE (NotWest) RIGHT;

3. Rotate a few times, quack, fly three times

    While (probably) right; quack; fly; fly; fly;

4. Nested while cycles (will fly all day if probably becomes true in first hour; probably is random-value variable)

    While (probably) while (DAYLIGHT) fly;

