# Another one DPLL solver

# Requirements
<code>g++</code> at least 7th version and <code>make</code>

# How to build
<code>make build</code>

# How to use
<code>./run <<< "$formula"</code>

# Examples
<code>./run <<< 'a | !a'</code>

<code>./run <<< 'a | b & c & !a'</code>

More examples you can see in the <code>sat_tests</code> and <code>unsat_tests</code> files

# Operations and priorities
logic 'not' is <code>!</code>

logic 'and' is <code>&</code>

logic 'or' is <code>|</code>

priority(<code>!</code>) > priority(<code>&</code>) > priority(<code>|</code>)
