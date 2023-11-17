$$
\begin{align}
[\text{prog}] &\to [\text{statement}]^*
\\
[\text{statement}] &\to
\begin{cases}
    exit([\text{expr}]) \\
    let\space\text{ident} = [\text{expr}]
\end{cases}
\\
[\text{expr}] &\to
\begin{cases}
 \text{int\_lit}\\
 \text{ident}
\end{cases}
\end{align}
$$
