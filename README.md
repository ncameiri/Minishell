# Minishell

<pre>

------------------------------------------------------------------------------

Lifetime of a Shell:

A shell does three main things in its lifetime:

	Initialize:
	In this step, a typical shell would read and execute its configuration files.
	These change aspects of the shell’s behavior.

	Interpret:
	Next, the shell reads commands from stdin (which could be interactive, or a file)
	and executes them.

	Terminate: After its commands are executed, the shell executes any shutdown
	commands, frees up any memory, and terminates.


The program needs to loop in order to work. And in the most basic form, the loop
will do 3 things:

	Read:
	Read the command from standard input.

	Parse:
	Separate the command string into a program and arguments.

	Execute:
	Run the parsed command.

------------------------------------------------------------------------------

get_next_line (fd, buffer(**line)).
						Inside of gnl you have a buffer_size.
						You can change it on get_next_line.h

stdin:  Same as fd = 0.	<--- This is what the keyboard writes.
stdout: Same as fd = 1. 	<--- This is what the shell terminal shows.
stderr: Same as fd = 2. 	<--- This is what the shell terminal shows.

------------------------------------------------------------------------------

We don't know which process starts first using fork() that's why we use
wait on the parent so the child goes first and the parent waits until the child
finishes.

------------------------------------------------------------------------------

Commands:

	builtin:

		absolute_path (NO Options, NO Args) // Changes prompt from "minishell$ " to absolute path.
		testing		  (NO Options, NO Args) // Changes subject commands for real builtin commands.

		echo		  (-n Option,  YES Args) // -n  Do not output a trailing newline.
		pwd			  (NO Options, YES Args)
		export		  (NO Options, YES Args)
		unset		  (NO Options, YES Args)
		env			  (NO Options,  NO Args)
		exit		  (NO Options, YES Args)
		cd			  (relative / abs path)

------------------------------------------------------------------------------

$? - Shows the exit status of the last command.

	Example:

		Type ls on the console. It worked.
		Type now $?, the previous exited status was succesful (0).

		Type wqweqwe. It gave an error.
		Type now $?, the previous exited status gave an error (127).

		Value 127 is returned by your shell /bin/bash when any given
		command within your bash script or on bash command line is not
		found in any of the paths defined by PATH system environment variable.
		The solution is to make sure that the command your are using can be
		found within your $PATH.

		https://tldp.org/LDP/abs/html/exitcodes.html

------------------------------------------------------------------------------

Parsing:

	 Quoting:





------------------------------------------------------------------------------


Updates:

	Added reading from executable files. ./a.out or /srcs/exec/exectest etc etc...	



gcc -fsanitize=address -o minishell shell.c ./srcs/utils/*.c ./srcs/parsing/*.c ./srcs/builtin/*c ./srcs/exec/*.c ./get_next_line/*.c libft/libft.a

https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_01

</pre>
