William Fulton
Recursive Descent Parser 

Description: 
	- Created with C++
	- This project was assigned in my Programming Languages class     
	- We were asked to create a recursive descent parser to recognize languages and create parse trees
	- We were given sample input and output files that were used in testing our parser
	- To check if the parsing was done correctly we examined the differences between output files 
	
*This program uses a lexical analyzer I created to check the rules of the languages* 

	
Language Rules:
	<P> => { <S> }
       
	<S> => ( <J> | <K> | <L> | <M>) ';'

	<J> => combinator [ evaluate ] IDENTIFIER <A>

	<K> => evaluate <A> 

	<L> => dictionary

	<M> => set IDENTIFIER <E>

	<F> => '\' IDENTIFIER '.' <A> |  '{' <F> '}'

	<A> => <I> {<I>} % left associative

	<I> => CIDENTIFIER | IDENTIFIER | '(' <A> ')' |   <F>

	<E> => <T> { ( + | - ) <T> }

	<T> => <R> { ( * | / ) <R> }

	<R> => INTLIT | ( <E> )


To run:
	- Add all files to a similar directory
	- Once added, use the parser.exe file along with the parser.cpp file to compile
	- Use one of the sample input files to create your parser output
	- Once you have your parser output, compare the difference between the proper output and the samples given
	- A successful parsing will show no differences between the two files